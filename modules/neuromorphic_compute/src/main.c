/*
 * neuromorphic_compute: Neuromorphic Computing WASM Module (v1.0)
 *
 * Spiking Neural Networks (LIF), event-driven computing, STDP learning,
 * sensor fusion, neuromorphic hardware simulation (Loihi, TrueNorth).
 */

#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Constants */
#define MAX_NEURONS   128
#define MAX_SYNAPSES  512
#define MAX_SPIKES    128
#define MAX_EVENTS    256
#define MAX_SENSORS   8
#define HW_GENERIC    0
#define HW_LOIHI      1
#define HW_TRUENORTH  2
#define EVT_SPIKE     0
#define EVT_SENSOR    1
#define EVT_REWARD    2
#define EVT_RESET     3
#define NEURON_EXC    0
#define NEURON_INH    1

/* Q16.16 fixed-point */
#define FP_SH     16
#define FP_ONE    (1 << FP_SH)
#define FP_HALF   (1 << (FP_SH - 1))
#define FP_INT(x) ((x) << FP_SH)
#define FP_MUL(a,b) (((long long)(a)*(b)) >> FP_SH)
#define FP_DIV(a,b) (((long long)(a) << FP_SH) / (b))

/* LIF neuron */
typedef struct {
    int v, thresh, reset, leak, ref, current, last_spike, ntype, fired;
} lif_neuron_t;

/* Synapse with STDP traces */
typedef struct {
    int src, dst, weight, delay, pre_tr, post_tr;
} synapse_t;

/* Spike event */
typedef struct { int idx, ts, mag; } spike_t;

/* Async event */
typedef struct { int type, chan, val, ts; } event_t;

/* SNN network */
typedef struct {
    int ncount, scount, spkcount, tstep, hw;
    lif_neuron_t neurons[MAX_NEURONS];
    synapse_t    synapses[MAX_SYNAPSES];
    spike_t      spikes[MAX_SPIKES];
} snn_t;

/* Event queue */
typedef struct { event_t ev[MAX_EVENTS]; int head, tail, cnt, over; } eventq_t;

/* STDP parameters */
typedef struct { int ap, am, tau_p, tau_m, wmax, wmin, reward; } stdp_t;

/* Sensor channel */
typedef struct { int id, modality, rate, sample, ltime, active; } sensor_ch_t;

/* Sensor array */
typedef struct {
    int cnt;
    sensor_ch_t ch[MAX_SENSORS];
    int corr[MAX_SENSORS * MAX_SENSORS];
} sensor_arr_t;

static snn_t net;
static eventq_t evq;
static stdp_t stdp;
static sensor_arr_t sensors;

/* --- Utilities --- */

static int my_strlen(const char *s) { int n=0; while(s[n]) n++; return n; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *a==*b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}
static void ps(const char *s) { host_print(s); }
static void pn(void) { ps("\n"); }

static void pi(int v) {
    char b[16]; int i=0;
    if (v<0) { ps("-"); v=-v; }
    if (v==0) { ps("0"); return; }
    while (v>0 && i<15) { b[i++]='0'+(v%10); v/=10; }
    char r[16]; int j=0;
    while (j<i) { r[j]=b[i-1-j]; j++; }
    r[j]='\0'; ps(r);
}

static void pf(int v) {
    int ip = v >> FP_SH;
    int fr = (v & ((1<<FP_SH)-1)) * 1000 >> FP_SH;
    pi(ip); ps(".");
    if (fr<100) ps("0"); if (fr<10) ps("0");
    pi(fr);
}

/* --- LIF Neuron Model --- */

static void lif_init(lif_neuron_t *n, int thr, int lk, int nt) {
    n->v=0; n->thresh=thr; n->reset=0; n->leak=lk;
    n->ref=0; n->current=0; n->last_spike=-1000;
    n->ntype=nt; n->fired=0;
}

static int lif_step(lif_neuron_t *n, int dt, int t) {
    n->fired = 0;
    if (n->ref > 0) { n->ref -= dt; n->v = n->reset; return 0; }
    int lk_cur = FP_MUL(n->leak, n->v - n->reset);
    n->v = n->v + FP_MUL(n->current - lk_cur, dt);
    n->current = 0;
    if (n->v >= n->thresh) {
        n->fired=1; n->last_spike=t; n->v=n->reset; n->ref=FP_INT(2);
        return 1;
    }
    return 0;
}

/* --- SNN Operations --- */

static void snn_init(snn_t *s, int hw) {
    s->ncount=0; s->scount=0; s->spkcount=0; s->tstep=0; s->hw=hw;
}

static int snn_add_neuron(snn_t *s, int thr, int lk, int nt) {
    if (s->ncount >= MAX_NEURONS) return -1;
    lif_init(&s->neurons[s->ncount], thr, lk, nt);
    return s->ncount++;
}

static int snn_add_syn(snn_t *s, int src, int dst, int w, int d) {
    if (s->scount>=MAX_SYNAPSES || src<0 || src>=s->ncount || dst<0 || dst>=s->ncount)
        return -1;
    int i = s->scount++;
    s->synapses[i].src=src; s->synapses[i].dst=dst;
    s->synapses[i].weight=w; s->synapses[i].delay=d;
    s->synapses[i].pre_tr=0; s->synapses[i].post_tr=0;
    return i;
}

static void snn_inject(snn_t *s, int idx, int cur) {
    if (idx>=0 && idx<s->ncount) s->neurons[idx].current += cur;
}

static void snn_propagate(snn_t *s) {
    for (int i=0; i<s->scount; i++) {
        synapse_t *syn = &s->synapses[i];
        if (s->neurons[syn->src].fired) {
            int sign = (s->neurons[syn->src].ntype == NEURON_INH) ? -1 : 1;
            s->neurons[syn->dst].current += FP_MUL(syn->weight, FP_INT(sign));
        }
    }
}

static void snn_record(snn_t *s, int idx) {
    if (s->spkcount < MAX_SPIKES) {
        s->spikes[s->spkcount].idx = idx;
        s->spikes[s->spkcount].ts  = s->tstep;
        s->spikes[s->spkcount].mag = FP_ONE;
        s->spkcount++;
    }
}

static void snn_step(snn_t *s, int dt) {
    s->spkcount = 0;
    for (int i=0; i<s->ncount; i++)
        if (lif_step(&s->neurons[i], dt, s->tstep)) snn_record(s, i);
    snn_propagate(s);
    s->tstep += dt;
}

/* --- Event-driven processing --- */

static void evq_init(eventq_t *q) { q->head=q->tail=q->cnt=q->over=0; }

static int evq_push(eventq_t *q, int type, int ch, int val, int ts) {
    if (q->cnt >= MAX_EVENTS) { q->over++; return -1; }
    q->ev[q->tail].type=type; q->ev[q->tail].chan=ch;
    q->ev[q->tail].val=val; q->ev[q->tail].ts=ts;
    q->tail = (q->tail+1) % MAX_EVENTS; q->cnt++;
    return 0;
}

static int evq_pop(eventq_t *q, event_t *out) {
    if (q->cnt <= 0) return -1;
    *out = q->ev[q->head];
    q->head = (q->head+1) % MAX_EVENTS; q->cnt--;
    return 0;
}

static int evq_process(snn_t *s, eventq_t *q, int max) {
    int done=0; event_t e;
    while (done<max && evq_pop(q, &e)==0) {
        switch (e.type) {
        case EVT_SENSOR: case EVT_SPIKE:
            snn_inject(s, e.chan, e.val); break;
        case EVT_REWARD: stdp.reward = e.val; break;
        case EVT_RESET:  snn_init(s, s->hw); break;
        }
        done++;
    }
    return done;
}

/* --- STDP Learning --- */

static void stdp_init(stdp_t *p) {
    p->ap=FP_INT(1)/100; p->am=FP_INT(1)/100;
    p->tau_p=FP_INT(20); p->tau_m=FP_INT(20);
    p->wmax=FP_ONE; p->wmin=0; p->reward=FP_ONE;
}

static void stdp_update(snn_t *s, stdp_t *p) {
    int decay = FP_DIV(FP_INT(19), FP_INT(20));
    for (int i=0; i<s->scount; i++) {
        synapse_t *syn = &s->synapses[i];
        syn->pre_tr  = FP_MUL(syn->pre_tr, decay);
        syn->post_tr = FP_MUL(syn->post_tr, decay);
        if (s->neurons[syn->src].fired) {
            syn->pre_tr += FP_ONE;
            syn->weight -= FP_MUL(FP_MUL(p->am, syn->post_tr), p->reward);
        }
        if (s->neurons[syn->dst].fired) {
            syn->post_tr += FP_ONE;
            syn->weight += FP_MUL(FP_MUL(p->ap, syn->pre_tr), p->reward);
        }
        if (syn->weight > p->wmax) syn->weight = p->wmax;
        if (syn->weight < p->wmin) syn->weight = p->wmin;
    }
}

/* --- Sensor fusion --- */

static void sens_init(sensor_arr_t *sa) {
    sa->cnt = 0;
    for (int i=0; i<MAX_SENSORS*MAX_SENSORS; i++) sa->corr[i] = 0;
}

static int sens_reg(sensor_arr_t *sa, int mod, int rate) {
    if (sa->cnt >= MAX_SENSORS) return -1;
    int i = sa->cnt++;
    sa->ch[i].id=i; sa->ch[i].modality=mod; sa->ch[i].rate=rate;
    sa->ch[i].sample=0; sa->ch[i].ltime=-1000; sa->ch[i].active=1;
    return i;
}

static void sens_push(sensor_arr_t *sa, int idx, int val, int ts) {
    if (idx<0 || idx>=sa->cnt) return;
    sa->ch[idx].sample = val; sa->ch[idx].ltime = ts;
}

static int sens_correlate(sensor_arr_t *sa, int a, int b) {
    if (a<0||a>=sa->cnt||b<0||b>=sa->cnt) return 0;
    int diff = sa->ch[a].sample - sa->ch[b].sample;
    if (diff < 0) diff = -diff;
    int c = FP_DIV(FP_ONE, FP_ONE + diff);
    sa->corr[a*MAX_SENSORS + b] = c;
    return c;
}

/* --- Hardware simulation --- */

static const char *hw_name(int h) {
    if (h==HW_LOIHI) return "Intel Loihi";
    if (h==HW_TRUENORTH) return "IBM TrueNorth";
    return "Generic SNN";
}

/* Loihi: adaptive threshold with dendritic dynamics */
static int loihi_step(snn_t *s, int dt) {
    int spk = 0;
    for (int i=0; i<s->ncount; i++) {
        lif_neuron_t *n = &s->neurons[i];
        int adapt = FP_MUL(n->thresh, FP_INT(1)/100);
        if (n->fired) n->thresh += adapt;
        else n->thresh += FP_MUL(FP_INT(1)-n->thresh, FP_INT(1)/200);
        if (lif_step(n, dt, s->tstep)) spk++;
    }
    snn_propagate(s); return spk;
}

/* TrueNorth: stochastic threshold */
static int tn_step(snn_t *s, int dt) {
    int spk = 0;
    for (int i=0; i<s->ncount; i++) {
        lif_neuron_t *n = &s->neurons[i];
        int noise = ((s->tstep*2654435761u + i*2246822519u) >> 16) & 0xFFFF;
        int orig = n->thresh;
        n->thresh += (noise - FP_HALF) / 32;
        if (lif_step(n, dt, s->tstep)) spk++;
        n->thresh = orig;
    }
    snn_propagate(s); return spk;
}

static int hw_step(snn_t *s, int dt) {
    if (s->hw == HW_LOIHI) return loihi_step(s, dt);
    if (s->hw == HW_TRUENORTH) return tn_step(s, dt);
    snn_step(s, dt); return s->spkcount;
}

/* --- Applications --- */

/* Pattern recognition: 3-layer SNN (8-16-4) */
static void test_pattern(void) {
    ps("[pattern] 3-layer SNN (8-16-4)\n");
    snn_t net; snn_init(&net, HW_LOIHI);
    for (int i=0; i<8; i++) snn_add_neuron(&net, FP_INT(1), FP_INT(1)/10, NEURON_EXC);
    int hid = net.ncount;
    for (int i=0; i<16; i++) snn_add_neuron(&net, FP_INT(1), FP_INT(1)/10, NEURON_EXC);
    int out = net.ncount;
    for (int i=0; i<4; i++) snn_add_neuron(&net, FP_INT(1), FP_INT(1)/10, NEURON_INH);
    for (int i=0; i<8; i++)
        for (int j=0; j<16; j++)
            snn_add_syn(&net, i, hid+j, FP_ONE/4 + (int)((i*7+j*13)%8)*(FP_ONE/32), 1);
    for (int i=0; i<16; i++)
        for (int j=0; j<4; j++)
            snn_add_syn(&net, hid+i, out+j, FP_ONE/4 + (int)((i*11+j*17)%6)*(FP_ONE/32), 1);
    ps("  Neurons: "); pi(net.ncount); ps(", Synapses: "); pi(net.scount); pn();
    int total = 0;
    for (int s=0; s<50; s++) {
        snn_inject(&net, s%8, FP_INT(2));
        total += hw_step(&net, FP_ONE);
        stdp_update(&net, &stdp);
    }
    ps("  50 steps, spikes: "); pi(total); pn();
    for (int i=0; i<4; i++) {
        ps("  out["); pi(i); ps("] V="); pf(net.neurons[out+i].v); pn();
    }
}

/* Time-series prediction: recurrent chain */
static void test_timeseries(void) {
    ps("[timeseries] Recurrent chain (16 neurons)\n");
    snn_t net; snn_init(&net, HW_GENERIC);
    for (int i=0; i<16; i++) snn_add_neuron(&net, FP_INT(1), FP_INT(1)/15, NEURON_EXC);
    for (int i=0; i<15; i++) snn_add_syn(&net, i, i+1, FP_ONE/2, 1);
    for (int i=1; i<16; i++) snn_add_syn(&net, i, i-1, FP_ONE/8, 2);
    ps("  Synapses: "); pi(net.scount); pn();
    evq_init(&evq);
    for (int s=0; s<64; s++) {
        int phase = (s*200) & 0xFFF;
        int val = (phase < 2048) ? FP_INT(1)+FP_ONE/2 : FP_ONE/2;
        evq_push(&evq, EVT_SENSOR, 0, val, s);
        evq_process(&net, &evq, 10);
        snn_step(&net, FP_ONE);
        stdp_update(&net, &stdp);
    }
    ps("  64 steps done. Final states:\n");
    for (int i=0; i<16; i+=4) {
        ps("    n["); pi(i); ps("] V="); pf(net.neurons[i].v); pn();
    }
}

/* Anomaly detection: sensor fusion + lateral inhibition */
static void test_anomaly(void) {
    ps("[anomaly] Sensor fusion + lateral inhibition\n");
    sens_init(&sensors);
    int s0 = sens_reg(&sensors, 0, 100);
    int s1 = sens_reg(&sensors, 1, 1000);
    int s2 = sens_reg(&sensors, 2, 50);
    ps("  Sensors: "); pi(sensors.cnt); pn();
    snn_t net; snn_init(&net, HW_TRUENORTH);
    for (int i=0; i<12; i++) snn_add_neuron(&net, FP_INT(1), FP_INT(1)/12, NEURON_EXC);
    for (int i=0; i<12; i++)
        for (int j=0; j<12; j++)
            if (i!=j) snn_add_syn(&net, i, j, -(FP_ONE/10), 1);
    int anom = 0;
    for (int s=0; s<40; s++) {
        sens_push(&sensors, s0, FP_ONE/2 + s*(FP_ONE/80), s);
        sens_push(&sensors, s1, FP_ONE/3, s);
        sens_push(&sensors, s2, FP_ONE/4 + s*(FP_ONE/100), s);
        snn_inject(&net, s%12, FP_ONE/2);
        int spk = hw_step(&net, FP_ONE);
        int c = sens_correlate(&sensors,s0,s1) + sens_correlate(&sensors,s1,s2)
              + sens_correlate(&sensors,s0,s2);
        if (c < FP_ONE/2) anom++;
        if (s%10==9) {
            ps("  step="); pi(s); ps(" spk="); pi(spk);
            ps(" corr="); pf(c); pn();
        }
    }
    ps("  Anomaly score: "); pi(anom); pn();
}

static void run_tests(void) {
    ps("=== Neuromorphic Compute Test Suite ===\n\n");
    stdp_init(&stdp);
    ps("--- Pattern Recognition ---\n"); test_pattern(); pn();
    ps("--- Time-Series Prediction ---\n"); test_timeseries(); pn();
    ps("--- Anomaly Detection ---\n"); test_anomaly(); pn();
    ps("=== All tests completed ===\n");
}

/* --- Entry point --- */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help=0, test=0, info=0, bench=0;
    unsigned int pos = 0;
    char *ap = (char *)buf;
    while (pos < 512 && ap[pos]) pos++; /* skip argv[0] */
    pos++;
    while (pos < 512 && ap[pos]) {
        char *a = &ap[pos];
        if (my_strcmp(a,"-h")==0 || my_strcmp(a,"--help")==0) help=1;
        else if (my_strcmp(a,"-t")==0 || my_strcmp(a,"--test")==0) test=1;
        else if (my_strcmp(a,"-i")==0 || my_strcmp(a,"--info")==0) info=1;
        else if (my_strcmp(a,"-b")==0 || my_strcmp(a,"--bench")==0) bench=1;
        while (pos<512 && ap[pos]) pos++;
        pos++;
    }

    ps("Neuromorphic Compute v1.0 - SNN Engine\n");

    if (help) {
        ps("Usage: neuromorphic_compute [options]\n");
        ps("  -h, --help    Show this help message\n");
        ps("  -t, --test    Run full test suite\n");
        ps("  -i, --info    Show module information\n");
        ps("  -b, --bench   Run performance benchmark\n");
        return;
    }
    if (info) {
        ps("Module: neuromorphic_compute v1.0\n");
        ps("Capabilities:\n");
        ps("  - LIF Spiking Neural Networks\n");
        ps("  - Event-driven sparse activation\n");
        ps("  - STDP online learning with reward modulation\n");
        ps("  - Multi-modal sensor fusion\n");
        ps("  - Intel Loihi compartment simulation\n");
        ps("  - IBM TrueNorth stochastic neuron simulation\n");
        ps("  - Pattern recognition, time-series, anomaly detection\n");
        ps("Fixed-point format: Q16.16\n");
        return;
    }
    if (bench) {
        ps("Benchmark: 200-step SNN (32 neurons)...\n");
        stdp_init(&stdp);
        snn_t bn; snn_init(&bn, HW_LOIHI);
        for (int i=0; i<32; i++)
            snn_add_neuron(&bn, FP_INT(1), FP_INT(1)/10,
                           (i<24) ? NEURON_EXC : NEURON_INH);
        for (int i=0; i<32; i++)
            for (int j=0; j<8; j++)
                snn_add_syn(&bn, i, (i+j+1)%32, FP_ONE/5, 1);
        int total=0;
        for (int i=0; i<200; i++) {
            snn_inject(&bn, i%32, FP_INT(2));
            total += hw_step(&bn, FP_ONE);
            stdp_update(&bn, &stdp);
        }
        ps("Done. Spikes: "); pi(total); ps(", Synapses: "); pi(bn.scount); pn();
        return;
    }
    if (test) { run_tests(); return; }

    /* Default: quick LIF demo */
    ps("No options given. Use -h for help, -t for tests.\n");
    ps("Quick demo: LIF neuron with constant current\n");
    stdp_init(&stdp);
    lif_neuron_t demo;
    lif_init(&demo, FP_INT(1), FP_INT(1)/10, NEURON_EXC);
    int spk_cnt = 0;
    for (int s=0; s<30; s++) {
        demo.current += FP_INT(2);
        int sp = lif_step(&demo, FP_ONE, s);
        if (sp) spk_cnt++;
        ps("  t="); pi(s); ps(" V="); pf(demo.v);
        if (sp) ps(" [SPIKE]");
        pn();
    }
    ps("Demo spikes: "); pi(spk_cnt); pn();
}
