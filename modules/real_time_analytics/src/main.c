#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

static unsigned int my_strlen(const char *s) { unsigned int l=0; while(s[l])l++; return l; }
static void my_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i; for(i=0;i<n&&s[i];i++)d[i]=s[i]; for(;i<n;i++)d[i]='\0';
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[16],rev[16]; int i=0,neg=0; unsigned int uv,j;
    if(v<0){neg=1;uv=(unsigned int)(-v);}else{uv=(unsigned int)v;}
    if(uv==0)buf[i++]='0';
    while(uv>0){buf[i++]='0'+(uv%10);uv/=10;} if(neg)buf[i++]='-'; buf[i]='\0';
    for(j=0;j<(unsigned int)i;j++)rev[j]=buf[i-1-j]; rev[j]='\0'; host_print(rev);
}
static void print_double(double v) {
    int w=(int)v; double f=v-(double)w; if(f<0.0)f=-f;
    print_int(w); host_print("."); print_int((int)(f*10000.0));
}
static double my_sqrt(double x) {
    if(x<=0.0)return 0.0; double g=x/2.0; int i;
    for(i=0;i<50;i++)g=(g+x/g)/2.0; return g;
}
static unsigned int rng_state=42;
static unsigned int rng_next(void){rng_state=rng_state*1103515245u+12345u;return(rng_state>>16)&0x7fff;}
static int str_eq(const char *a,const char *b,unsigned int mx){
    unsigned int i; for(i=0;i<mx;i++){if(a[i]!=b[i])return 0;if(a[i]=='\0')return 1;} return 1;
}

/* 1. Stream Aggregation Windows */
typedef enum{WINDOW_TUMBLING=0,WINDOW_SLIDING,WINDOW_SESSION}window_type_t;
typedef enum{AGG_SUM=0,AGG_AVG,AGG_COUNT,AGG_MIN,AGG_MAX}agg_type_t;
typedef struct{unsigned long long timestamp;char key[32];double value;int type;}event_t;
#define MAX_WIN_EV 128
typedef struct{
    window_type_t type; unsigned long long size_ms,slide_ms,gap_ms;
    event_t events[MAX_WIN_EV]; unsigned int count;
    unsigned long long win_start,win_end; int active;
}window_t;
static window_t *window_create(window_type_t type,unsigned long long sz,unsigned long long sl,unsigned long long gap){
    window_t *w=(window_t*)host_alloc(sizeof(window_t),8);
    if(!w)return(void*)0;
    w->type=type;w->size_ms=sz;w->slide_ms=sl;w->gap_ms=gap;
    w->count=0;w->win_start=0;w->win_end=sz;w->active=1; return w;
}
static int window_assign_event(window_t *w,event_t *ev){
    if(!w||w->count>=MAX_WIN_EV)return 0;
    if(w->type==WINDOW_SESSION){
        if(w->count==0){w->win_start=ev->timestamp;w->win_end=ev->timestamp+w->gap_ms;}
        else if(ev->timestamp>w->win_end)return 0;
        else w->win_end=ev->timestamp+w->gap_ms;
    }
    w->events[w->count]=*ev; w->count++; return 1;
}
static int window_trigger(window_t *w){
    if(!w||w->count==0)return 0;
    if(w->type==WINDOW_TUMBLING)return(w->events[w->count-1].timestamp>=w->win_end);
    if(w->type==WINDOW_SLIDING)return(w->events[w->count-1].timestamp>=w->win_start+w->slide_ms);
    return 0;
}
static double window_compute_aggregate(window_t *w,agg_type_t agg){
    if(!w||w->count==0)return 0.0; double r=w->events[0].value; int i;
    switch(agg){
    case AGG_SUM:case AGG_AVG:{double s=0;for(i=0;i<(int)w->count;i++)s+=w->events[i].value;
        r=(agg==AGG_SUM)?s:s/(double)w->count;break;}
    case AGG_COUNT:r=(double)w->count;break;
    case AGG_MIN:for(i=1;i<(int)w->count;i++)if(w->events[i].value<r)r=w->events[i].value;break;
    case AGG_MAX:for(i=1;i<(int)w->count;i++)if(w->events[i].value>r)r=w->events[i].value;break;}
    return r;
}
static void window_expire(window_t *w,unsigned long long ct){
    if(!w)return; unsigned int j=0;
    unsigned long long co=(ct>w->size_ms)?(ct-w->size_ms):0; int i;
    for(i=0;i<(int)w->count;i++)if(w->events[i].timestamp>=co)w->events[j++]=w->events[i];
    w->count=j;w->win_start=co;w->win_end=ct;
}

/* 2. Complex Event Processing */
typedef enum{CEP_NEXT=0,CEP_AND,CEP_NOT,CEP_FOLLOWED_BY}cep_condition_t;
typedef struct{int event_type;cep_condition_t condition;double predicate_value;}pattern_element_t;
#define MAX_PAT_ELEMS 16
typedef struct{pattern_element_t elements[MAX_PAT_ELEMS];unsigned int element_count;
    unsigned long long within_ms;int id;}pattern_t;
typedef struct{int pattern_id;unsigned int match_pos;unsigned long long start_time;int active;}active_match_t;
#define MAX_CEP_PAT 16
#define MAX_ACT_MAT 64
typedef struct{
    pattern_t patterns[MAX_CEP_PAT]; unsigned int pattern_count;
    active_match_t active_matches[MAX_ACT_MAT]; unsigned int match_count;
    int match_found; unsigned long long last_event_time;
}cep_engine_t;
static cep_engine_t g_cep;
static void cep_init(cep_engine_t *e){e->pattern_count=0;e->match_count=0;e->match_found=0;e->last_event_time=0;}
static int cep_register_pattern(cep_engine_t *e,pattern_t *p){
    if(e->pattern_count>=MAX_CEP_PAT)return -1;
    p->id=(int)e->pattern_count; e->patterns[e->pattern_count]=*p; e->pattern_count++; return p->id;
}
static void cep_feed_event(cep_engine_t *e,event_t *ev){
    if(!e)return; e->last_event_time=ev->timestamp; unsigned int i;
    for(i=0;i<e->match_count;i++){
        active_match_t *am=&e->active_matches[i]; if(!am->active)continue;
        pattern_t *p=&e->patterns[am->pattern_id];
        if(ev->timestamp-am->start_time>p->within_ms){am->active=0;continue;}
        unsigned int np=am->match_pos+1;
        if(np<p->element_count){pattern_element_t *pe=&p->elements[np];
            if(pe->event_type==ev->type&&ev->value>=pe->predicate_value)am->match_pos=np;}
    }
    for(i=0;i<e->pattern_count;i++){
        pattern_t *p=&e->patterns[i]; if(p->element_count==0)continue;
        pattern_element_t *f=&p->elements[0];
        if(f->event_type==ev->type&&ev->value>=f->predicate_value&&e->match_count<MAX_ACT_MAT){
            active_match_t *am=&e->active_matches[e->match_count];
            am->pattern_id=(int)i;am->match_pos=0;am->start_time=ev->timestamp;am->active=1;
            e->match_count++;
        }
    }
}
static int cep_check_matches(cep_engine_t *e){
    if(!e)return 0; int comp=0; unsigned int i;
    for(i=0;i<e->match_count;i++){
        active_match_t *am=&e->active_matches[i]; if(!am->active)continue;
        pattern_t *p=&e->patterns[am->pattern_id];
        if(am->match_pos>=p->element_count-1){comp++;am->active=0;}
    }
    e->match_found=(comp>0); return comp;
}
static void cep_cleanup_expired(cep_engine_t *e,unsigned long long ct){
    if(!e)return; unsigned int i;
    for(i=0;i<e->match_count;i++){
        active_match_t *am=&e->active_matches[i]; if(!am->active)continue;
        pattern_t *p=&e->patterns[am->pattern_id];
        if(ct-am->start_time>p->within_ms)am->active=0;
    }
}

/* 3. Time Series Analysis */
#define MAX_TS_CAP 256
typedef struct{unsigned long long timestamps[MAX_TS_CAP];double values[MAX_TS_CAP];
    unsigned int count,capacity;}timeseries_t;
static timeseries_t *ts_create(unsigned int cap){
    if(cap>MAX_TS_CAP)cap=MAX_TS_CAP;
    timeseries_t *t=(timeseries_t*)host_alloc(sizeof(timeseries_t),8);
    if(!t)return(void*)0; t->count=0;t->capacity=cap; return t;
}
static int ts_append(timeseries_t *t,unsigned long long ts,double v){
    if(!t||t->count>=t->capacity)return 0;
    t->timestamps[t->count]=ts;t->values[t->count]=v;t->count++; return 1;
}
static double ts_sma(timeseries_t *t,unsigned int w){
    if(!t||t->count<w||w==0)return 0.0; double s=0; unsigned int i;
    for(i=t->count-w;i<t->count;i++)s+=t->values[i]; return s/(double)w;
}
static double ts_ema(timeseries_t *t,unsigned int w,double alpha){
    if(!t||t->count==0)return 0.0;
    if(alpha<=0.0)alpha=2.0/((double)w+1.0); double e=t->values[0]; unsigned int i;
    for(i=1;i<t->count;i++)e=alpha*t->values[i]+(1.0-alpha)*e; return e;
}
typedef struct{double slope,intercept,r_squared;}trend_result_t;
static trend_result_t ts_detect_trend(timeseries_t *t){
    trend_result_t r;r.slope=0;r.intercept=0;r.r_squared=0;
    if(!t||t->count<2)return r;
    double sx=0,sy=0,sxx=0,sxy=0; unsigned int n=t->count,i;
    for(i=0;i<n;i++){double x=(double)i,y=t->values[i];sx+=x;sy+=y;sxx+=x*x;sxy+=x*y;}
    double dn=(double)n,dn2=dn*sxx-sx*sx; if(dn2==0.0)return r;
    r.slope=(dn*sxy-sx*sy)/dn2; r.intercept=(sy-r.slope*sx)/dn;
    double my=sy/dn,st=0,sr=0;
    for(i=0;i<n;i++){double y=t->values[i],yp=r.intercept+r.slope*(double)i;
        st+=(y-my)*(y-my);sr+=(y-yp)*(y-yp);}
    r.r_squared=(st>0)?(1.0-sr/st):0.0; return r;
}
typedef struct{double trend,seasonal,residual;}decomposition_t;
static decomposition_t ts_seasonal_decompose(timeseries_t *t,unsigned int period){
    decomposition_t d;d.trend=0;d.seasonal=0;d.residual=0;
    if(!t||t->count<period*2||period==0)return d;
    double tot=0; unsigned int i;
    for(i=0;i<t->count;i++)tot+=t->values[i]; d.trend=tot/(double)t->count;
    double ss[32]; unsigned int sc[32];
    for(i=0;i<period&&i<32;i++){ss[i]=0;sc[i]=0;}
    for(i=0;i<t->count;i++){unsigned int p=i%period;
        if(p<32){ss[p]+=t->values[i];sc[p]++;}}
    unsigned int lp=(t->count-1)%period;
    if(lp<32&&sc[lp]>0)d.seasonal=ss[lp]/(double)sc[lp]-d.trend;
    d.residual=t->values[t->count-1]-d.trend-d.seasonal; return d;
}
#define MAX_ANOM 32
typedef struct{unsigned int indices[MAX_ANOM];double scores[MAX_ANOM];unsigned int count;}anomaly_result_t;
static anomaly_result_t ts_detect_anomalies(timeseries_t *t,double zt){
    anomaly_result_t r;r.count=0; if(!t||t->count<2)return r;
    double s=0; unsigned int i;
    for(i=0;i<t->count;i++)s+=t->values[i]; double m=s/(double)t->count,vs=0;
    for(i=0;i<t->count;i++){double d=t->values[i]-m;vs+=d*d;}
    double sd=my_sqrt(vs/(double)t->count); if(sd==0.0)return r;
    for(i=0;i<t->count&&r.count<MAX_ANOM;i++){
        double z=(t->values[i]-m)/sd; if(z<0)z=-z;
        if(z>zt){r.indices[r.count]=i;r.scores[r.count]=z;r.count++;}
    } return r;
}

/* 4. OLAP Query Engine */
typedef enum{OLAP_DOUBLE=0,OLAP_INT,OLAP_STRING}olap_data_type_t;
#define MAX_OLAP_VAL 128
#define MAX_OLAP_COL 8
typedef struct{char name[32];olap_data_type_t data_type;double values[MAX_OLAP_VAL];
    double min,max,sum;unsigned int count;}olap_column_t;
typedef struct{olap_column_t columns[MAX_OLAP_COL];unsigned int column_count,row_count;}olap_table_t;
static olap_table_t *olap_create_table(void){
    olap_table_t *t=(olap_table_t*)host_alloc(sizeof(olap_table_t),8);
    if(!t)return(void*)0; t->column_count=0;t->row_count=0; return t;
}
static int olap_add_column(olap_table_t *t,const char *name,olap_data_type_t dt){
    if(!t||t->column_count>=MAX_OLAP_COL)return -1;
    olap_column_t *c=&t->columns[t->column_count];
    my_strncpy(c->name,name,31);c->data_type=dt;c->min=1e18;c->max=-1e18;c->sum=0;c->count=0;
    t->column_count++; return(int)t->column_count-1;
}
static int olap_append_row(olap_table_t *t,double vals[]){
    if(!t||t->row_count>=MAX_OLAP_VAL)return 0; unsigned int c;
    for(c=0;c<t->column_count;c++){
        t->columns[c].values[t->row_count]=vals[c];
        if(vals[c]<t->columns[c].min)t->columns[c].min=vals[c];
        if(vals[c]>t->columns[c].max)t->columns[c].max=vals[c];
        t->columns[c].sum+=vals[c];t->columns[c].count++;
    } t->row_count++; return 1;
}
static void olap_scan(olap_table_t *t){
    if(!t)return; unsigned int c,r;
    for(c=0;c<t->column_count;c++){olap_column_t *col=&t->columns[c];
        col->min=1e18;col->max=-1e18;col->sum=0;
        for(r=0;r<t->row_count;r++){double v=col->values[r];
            if(v<col->min)col->min=v;if(v>col->max)col->max=v;col->sum+=v;}
    }
}
#define MAX_OLAP_GRP 32
typedef struct{double group_value,agg_result;}olap_group_result_t;
static int olap_aggregate(olap_table_t *t,int gc,int ac,olap_group_result_t res[]){
    if(!t||gc>=(int)t->column_count||ac>=(int)t->column_count)return 0;
    olap_column_t *gcol=&t->columns[gc],*acol=&t->columns[ac];
    double grps[MAX_OLAP_GRP],sums[MAX_OLAP_GRP];
    unsigned int cnts[MAX_OLAP_GRP],gcnt=0,r,g;
    for(r=0;r<t->row_count;r++){double gv=gcol->values[r];int f=-1;
        for(g=0;g<gcnt;g++)if(grps[g]==gv){f=(int)g;break;}
        if(f<0&&gcnt<MAX_OLAP_GRP){grps[gcnt]=gv;sums[gcnt]=0;cnts[gcnt]=0;
            f=(int)gcnt;gcnt++;}
        if(f>=0){sums[f]+=acol->values[r];cnts[f]++;}}
    for(g=0;g<gcnt;g++){res[g].group_value=grps[g];
        res[g].agg_result=(cnts[g]>0)?(sums[g]/(double)cnts[g]):0;}
    return(int)gcnt;
}
#define HLL_REGS 64
static unsigned int olap_hyperloglog_estimate(double vals[],unsigned int cnt){
    if(cnt==0)return 0; unsigned char regs[HLL_REGS]; unsigned int i;
    for(i=0;i<HLL_REGS;i++)regs[i]=0;
    for(i=0;i<cnt;i++){unsigned int h=(unsigned int)(vals[i]*2654435761.0);
        unsigned int b=h%HLL_REGS,rm=h/HLL_REGS; unsigned char rho=1;
        while(rm>0&&(rm&1)==0){rho++;rm>>=1;}
        if(rho>regs[b])regs[b]=rho;}
    double alpha=0.7213/(1.0+1.079/(double)HLL_REGS),sum=0;
    for(i=0;i<HLL_REGS;i++)sum+=1.0/(double)(1U<<regs[i]);
    return(unsigned int)(alpha*(double)HLL_REGS*(double)HLL_REGS/sum);
}
static double olap_tdigest_quantile(double vals[],unsigned int cnt,double q){
    if(cnt==0)return 0.0; if(q<0.0)q=0.0; if(q>1.0)q=1.0;
    double sorted[MAX_OLAP_VAL]; unsigned int i,j,n=(cnt>MAX_OLAP_VAL)?MAX_OLAP_VAL:cnt;
    for(i=0;i<n;i++){sorted[i]=vals[i];
        for(j=i;j>0&&sorted[j]<sorted[j-1];j--){double tmp=sorted[j];sorted[j]=sorted[j-1];sorted[j-1]=tmp;}}
    double idx=q*(double)(n-1); unsigned int lo=(unsigned int)idx,hi=lo+1;
    if(hi>=n)return sorted[n-1]; double fr=idx-(double)lo;
    return sorted[lo]*(1.0-fr)+sorted[hi]*fr;
}
#define MAX_MV 8
typedef struct{char name[32],query[128];unsigned int refresh_ms;
    unsigned long long last_refresh;double cached_result;int active;}materialized_view_t;
static materialized_view_t g_mviews[MAX_MV]; static unsigned int g_mvcnt=0;
static int olap_materialized_view_define(const char *name,const char *query,unsigned int ri){
    if(g_mvcnt>=MAX_MV)return -1; materialized_view_t *mv=&g_mviews[g_mvcnt];
    my_strncpy(mv->name,name,31);my_strncpy(mv->query,query,127);
    mv->refresh_ms=ri;mv->last_refresh=0;mv->cached_result=0;mv->active=1; g_mvcnt++;
    return(int)g_mvcnt-1;
}

/* 5. Data Ingestion */
#define MAX_TNAME 32
#define MAX_TPART 8
#define MAX_PMSG 64
#define MAX_TOPS 8
typedef struct{char data[64];unsigned long long timestamp;unsigned int size;}message_t;
typedef struct{message_t messages[MAX_PMSG];unsigned int count;}partition_t;
typedef struct{char name[MAX_TNAME];partition_t partitions[MAX_TPART];
    unsigned int partition_count;unsigned long long offset;int active;}topic_t;
typedef struct{int topic_idx;unsigned int partition,current_offset;
    char group_id[32];unsigned long long committed_offset;int active;}consumer_t;
static topic_t g_topics[MAX_TOPS]; static unsigned int g_tcnt=0; static int g_init=0;
static void ingest_init(void){
    g_tcnt=0;g_init=1; unsigned int i; for(i=0;i<MAX_TOPS;i++)g_topics[i].active=0;
}
static int ingest_create_topic(const char *name,unsigned int parts){
    if(!g_init||g_tcnt>=MAX_TOPS)return -1; if(parts>MAX_TPART)parts=MAX_TPART;
    topic_t *t=&g_topics[g_tcnt]; my_strncpy(t->name,name,MAX_TNAME-1);
    t->partition_count=parts;t->offset=0;t->active=1;
    unsigned int i; for(i=0;i<parts;i++)t->partitions[i].count=0; g_tcnt++;
    return(int)g_tcnt-1;
}
static int ingest_produce(int ti,unsigned int p,const char *data){
    if(ti<0||ti>=(int)g_tcnt)return 0; topic_t *t=&g_topics[ti];
    if(!t->active||p>=t->partition_count)return 0;
    partition_t *pt=&t->partitions[p]; if(pt->count>=MAX_PMSG)return 0;
    message_t *m=&pt->messages[pt->count];
    my_strncpy(m->data,data,63);m->size=my_strlen(data);m->timestamp=t->offset++;
    pt->count++; return 1;
}
static int ingest_consume(consumer_t *c,message_t *out,unsigned int mx){
    if(!c||c->topic_idx<0||c->topic_idx>=(int)g_tcnt)return 0;
    topic_t *t=&g_topics[c->topic_idx];
    if(!t->active||c->partition>=t->partition_count)return 0;
    partition_t *p=&t->partitions[c->partition]; unsigned int n=0;
    while(c->current_offset<p->count&&n<mx){out[n]=p->messages[c->current_offset];
        c->current_offset++;n++;} return(int)n;
}
static int ingest_commit_offset(consumer_t *c){if(!c)return 0;c->committed_offset=c->current_offset;return 1;}
static int ingest_backpressure_check(consumer_t *c){
    if(!c||c->topic_idx<0||c->topic_idx>=(int)g_tcnt)return 0;
    topic_t *t=&g_topics[c->topic_idx];
    if(!t->active||c->partition>=t->partition_count)return 0;
    partition_t *p=&t->partitions[c->partition];
    return((p->count-c->current_offset)>MAX_PMSG/2)?1:0;
}

/* 6. Real-time Dashboard */
#define MAX_DMET 16
#define MAX_DALT 16
#define MAX_MHIST 64
#define MAX_MNAME 32
#define MAX_MUNIT 16
#define MAX_WURL 128
typedef struct{char name[MAX_MNAME];double current_value;double history[MAX_MHIST];
    unsigned int history_count;char unit[MAX_MUNIT];unsigned long long last_update;}dashboard_metric_t;
typedef enum{ALERT_THRESHOLD=0,ALERT_RATE,ALERT_CRON}alert_type_t;
typedef struct{char metric_name[MAX_MNAME];alert_type_t type;double threshold;
    char webhook_url[MAX_WURL];int triggered;}alert_rule_t;
typedef struct{char name[64];dashboard_metric_t metrics[MAX_DMET];unsigned int metric_count;
    alert_rule_t alerts[MAX_DALT];unsigned int alert_count;}dashboard_t;
static dashboard_t *dashboard_create(const char *name){
    dashboard_t *d=(dashboard_t*)host_alloc(sizeof(dashboard_t),8);
    if(!d)return(void*)0; my_strncpy(d->name,name,63);d->metric_count=0;d->alert_count=0; return d;
}
static int dashboard_add_metric(dashboard_t *d,const char *name,const char *unit){
    if(!d||d->metric_count>=MAX_DMET)return -1;
    dashboard_metric_t *m=&d->metrics[d->metric_count];
    my_strncpy(m->name,name,MAX_MNAME-1);my_strncpy(m->unit,unit,MAX_MUNIT-1);
    m->current_value=0;m->history_count=0;m->last_update=0; d->metric_count++;
    return(int)d->metric_count-1;
}
static int dashboard_update_metric(dashboard_t *d,const char *mn,double v){
    if(!d)return 0; unsigned int i;
    for(i=0;i<d->metric_count;i++){dashboard_metric_t *m=&d->metrics[i];
        if(str_eq(m->name,mn,MAX_MNAME)){m->current_value=v;
            if(m->history_count<MAX_MHIST){m->history[m->history_count]=v;m->history_count++;}
            return 1;}} return 0;
}
static int dashboard_register_alert(dashboard_t *d,alert_rule_t *r){
    if(!d||d->alert_count>=MAX_DALT)return -1;
    d->alerts[d->alert_count]=*r;d->alerts[d->alert_count].triggered=0; d->alert_count++;
    return(int)d->alert_count-1;
}
static int dashboard_evaluate_alerts(dashboard_t *d){
    if(!d)return 0; int tc=0,fnd; unsigned int a,m;
    for(a=0;a<d->alert_count;a++){alert_rule_t *r=&d->alerts[a];fnd=0;
        for(m=0;m<d->metric_count&&!fnd;m++){dashboard_metric_t *mt=&d->metrics[m];
            if(!str_eq(r->metric_name,mt->name,MAX_MNAME))continue; fnd=1;
            if(r->type==ALERT_THRESHOLD){
                if(mt->current_value>r->threshold){r->triggered=1;tc++;}
                else r->triggered=0;}}} return tc;
}
static double dashboard_compute_rate(dashboard_metric_t *m,unsigned int tw){
    if(!m||m->history_count<2||tw==0)return 0.0;
    unsigned int s=0; if(m->history_count>tw)s=m->history_count-tw;
    unsigned int sp=m->history_count-1-s; if(sp==0)return 0.0;
    return(m->history[m->history_count-1]-m->history[s])/(double)sp;
}

/* 7. Tests and Entry Point */
static void test_stream_aggregation(void){
    print_str("[Stream Aggregation] Tumbling window\n");
    window_t *w=window_create(WINDOW_TUMBLING,1000,0,0);
    if(!w){print_str("ERR:window_create\n");return;}
    unsigned int i;
    for(i=0;i<10;i++){event_t ev;ev.timestamp=(unsigned long long)(i*120);
        my_strncpy(ev.key,"sensor_1",31);ev.value=20.0+(double)(rng_next()%100);ev.type=1;
        window_assign_event(w,&ev);}
    print_str("  Events:");print_int((int)w->count);
    print_str(" SUM=");print_double(window_compute_aggregate(w,AGG_SUM));
    print_str(" AVG=");print_double(window_compute_aggregate(w,AGG_AVG));
    print_str(" MIN=");print_double(window_compute_aggregate(w,AGG_MIN));
    print_str(" MAX=");print_double(window_compute_aggregate(w,AGG_MAX));
    print_str("\n  Triggered:");print_int(window_trigger(w));
    window_expire(w,2000);print_str(" AfterExp:");print_int((int)w->count);print_str("\n");
}
static void test_cep(void){
    print_str("[CEP] Pattern matching\n"); cep_init(&g_cep);
    pattern_t pat;pat.element_count=2;pat.within_ms=5000;
    pat.elements[0].event_type=1;pat.elements[0].condition=CEP_FOLLOWED_BY;pat.elements[0].predicate_value=50.0;
    pat.elements[1].event_type=2;pat.elements[1].condition=CEP_NEXT;pat.elements[1].predicate_value=60.0;
    print_str("  PatternID:");print_int(cep_register_pattern(&g_cep,&pat));
    event_t e1;e1.timestamp=1000;e1.type=1;e1.value=75.0;my_strncpy(e1.key,"e1",31);
    cep_feed_event(&g_cep,&e1);
    event_t e2;e2.timestamp=2000;e2.type=2;e2.value=80.0;my_strncpy(e2.key,"e2",31);
    cep_feed_event(&g_cep,&e2);
    print_str(" Matches:");print_int(cep_check_matches(&g_cep));
    cep_cleanup_expired(&g_cep,10000);print_str(" done\n");
}
static void test_timeseries(void){
    print_str("[TimeSeries] Analysis\n");
    timeseries_t *ts=ts_create(64);
    if(!ts){print_str("ERR:ts_create\n");return;}
    unsigned int i;
    for(i=0;i<20;i++)ts_append(ts,(unsigned long long)(i*1000),100.0+(double)(rng_next()%50)+(double)i*2.0);
    print_str("  PTS:");print_int((int)ts->count);
    print_str(" SMA5:");print_double(ts_sma(ts,5));
    print_str(" EMA5:");print_double(ts_ema(ts,5,0.0));
    trend_result_t tr=ts_detect_trend(ts);
    print_str(" slope:");print_double(tr.slope);print_str(" R2:");print_double(tr.r_squared);
    decomposition_t dc=ts_seasonal_decompose(ts,4);
    print_str(" trend:");print_double(dc.trend);print_str(" seasonal:");print_double(dc.seasonal);
    anomaly_result_t an=ts_detect_anomalies(ts,2.0);
    print_str(" anom:");print_int((int)an.count);print_str("\n");
}
static void test_olap(void){
    print_str("[OLAP] Columnar queries\n");
    olap_table_t *t=olap_create_table();
    if(!t){print_str("ERR:olap_create\n");return;}
    olap_add_column(t,"region",OLAP_INT);olap_add_column(t,"sales",OLAP_DOUBLE);
    unsigned int i;
    for(i=0;i<10;i++){double r[2];r[0]=(double)((i%3)+1);r[1]=100.0+(double)(rng_next()%500);
        olap_append_row(t,r);}
    print_str("  Rows:");print_int((int)t->row_count);
    olap_scan(t);
    print_str(" Col0:");print_double(t->columns[0].min);print_str("-");print_double(t->columns[0].max);
    print_str(" Col1:");print_double(t->columns[1].min);print_str("-");print_double(t->columns[1].max);
    olap_group_result_t gr[MAX_OLAP_GRP]; int ng=olap_aggregate(t,0,1,gr);
    print_str(" Groups:");print_int(ng);
    for(i=0;i<(unsigned int)ng;i++){print_str(" G");print_double(gr[i].group_value);
        print_str("=");print_double(gr[i].agg_result);}
    double hv[]={1.0,2.0,3.0,1.0,2.0,5.0,3.0,7.0,8.0,1.0};
    print_str(" HLL:");print_int((int)olap_hyperloglog_estimate(hv,10));
    double tv[]={10.0,20.0,30.0,40.0,50.0,60.0,70.0,80.0,90.0,100.0};
    print_str(" P95:");print_double(olap_tdigest_quantile(tv,10,0.95));
    print_str(" MV:");print_int(olap_materialized_view_define("mv_s","SELECT AVG(s)",5000));
    print_str("\n");
}
static void test_ingestion(void){
    print_str("[Ingestion] Pub/Sub\n"); ingest_init();
    int tid=ingest_create_topic("events",2); print_str("  TopicID:");print_int(tid);
    unsigned int i;
    for(i=0;i<5;i++){char msg[64];my_strncpy(msg,"evt_",63);
        unsigned int l=my_strlen(msg);msg[l]=(char)(i+'0');msg[l+1]='\0';
        ingest_produce(tid,0,msg);}
    consumer_t c;c.topic_idx=tid;c.partition=0;c.current_offset=0;
    c.committed_offset=0;c.active=1;my_strncpy(c.group_id,"g1",31);
    message_t msgs[10]; print_str(" Cons:");print_int(ingest_consume(&c,msgs,10));
    print_str(" Commit:");print_int(ingest_commit_offset(&c));
    print_str(" BP:");print_int(ingest_backpressure_check(&c));print_str("\n");
}
static void test_dashboard(void){
    print_str("[Dashboard] Metrics+Alerts\n");
    dashboard_t *d=dashboard_create("main");
    if(!d){print_str("ERR:dash_create\n");return;}
    int ci=dashboard_add_metric(d,"cpu","%");
    int mi=dashboard_add_metric(d,"mem","MB");
    print_str("  Metrics:");print_int((int)d->metric_count);
    unsigned int i;
    for(i=0;i<10;i++){
        dashboard_update_metric(d,"cpu",30.0+(double)(rng_next()%60));
        dashboard_update_metric(d,"mem",1024.0+(double)(rng_next()%512));}
    print_str(" CPU:");print_double(d->metrics[ci].current_value);
    print_str(" MEM:");print_double(d->metrics[mi].current_value);
    alert_rule_t r;my_strncpy(r.metric_name,"cpu",MAX_MNAME-1);
    r.type=ALERT_THRESHOLD;r.threshold=80.0;
    my_strncpy(r.webhook_url,"http://localhost/alert",MAX_WURL-1);
    print_str(" AlertID:");print_int(dashboard_register_alert(d,&r));
    print_str(" Trig:");print_int(dashboard_evaluate_alerts(d));
    print_str(" Rate:");print_double(dashboard_compute_rate(&d->metrics[ci],5));print_str("\n");
}

__attribute__((export_name("_start")))
void _start(void){
    char ab[256]; unsigned int i;
    for(i=0;i<256;i++)ab[i]=0;
    int argc=host_get_argv(0,256),help=0,tests=1;
    if(argc>0){for(i=0;i+1<256;i++){
        if(ab[i]=='-'&&ab[i+1]=='h')help=1;
        if(ab[i]=='-'&&ab[i+1]=='t')tests=1;}}
    print_str("=== Real-Time Analytics v59.0 - Stream Processing Engine ===\n\n");
    if(help){print_str("Usage: real_time_analytics [-h] [-t]\n");
        print_str("  -h  Show help\n  -t  Run tests\n\n");}
    if(tests){print_str("--- Comprehensive demo ---\n\n");
        test_stream_aggregation();test_cep();test_timeseries();
        test_olap();test_ingestion();test_dashboard();}
    print_str("\n=== All analytics modules initialized successfully ===\n");
    host_exit(0);
}
