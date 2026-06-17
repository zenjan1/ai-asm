/*
 * calendar_scheduler - Calendar & Scheduling Module for AI-ASM OS
 * Calendar/event/room management, smart scheduling, sharing, CalDAV/iCal/REST
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
#define MAX_CALENDARS 64U
#define MAX_EVENTS    512U
#define MAX_PARTIP    32U
#define MAX_ROOMS     128U
#define MAX_REMINDERS 256U
#define MAX_SHARES    64U
#define MAX_NLEN 128U
#define MAX_DLEN 512U
#define MAX_ULEN 256U
#define MAX_UIDLEN 64U
#define ST_ACT 0x01U
#define ST_INA 0x00U
#define V_DAY 0x01U
#define V_WEEK 0x02U
#define V_MONTH 0x04U
#define V_YEAR 0x08U
#define R_NONE 0x00U
#define R_DAILY 0x01U
#define R_WEEKLY 0x02U
#define R_MONTHLY 0x04U
#define R_YEARLY 0x08U
#define RM_E 0x01U
#define RM_P 0x02U
#define RM_S 0x04U
#define PT_R 0x01U
#define PT_O 0x02U
#define PT_S 0x04U
#define RA_N 0x00U
#define RA_M 0x01U
#define RA_A 0x02U
#define BK_P 0x01U
#define BK_A 0x02U
#define BK_RJ 0x04U
#define BK_C 0x08U
#define SH_R 0x01U
#define SH_W 0x02U
#define SH_X 0x04U
#define CT_L 0x01U
#define CT_C 0x02U
#define CT_S 0x04U
typedef struct { unsigned int year,month,day,hour,minute,second; int off; } DtT;
typedef struct { char uid[MAX_UIDLEN],name[MAX_NLEN],desc[MAX_DLEN];
    unsigned int type,status,vmode; int owner; char color[16];
    unsigned int ec,sc,tzi; } CalT;
typedef struct { char uid[MAX_UIDLEN],title[MAX_NLEN],desc[MAX_DLEN],loc[MAX_NLEN];
    DtT s,e; unsigned int rec,re,ci,pri,status;
    int pids[MAX_PARTIP]; unsigned int pc; unsigned int ri[MAX_REMINDERS],rc;
    int rid,org; } EvT;
typedef struct { int uid; unsigned int role; char em[MAX_ULEN]; unsigned int acc; } PtT;
typedef struct { unsigned int ei,mb,type,sent; } RemT;
typedef struct { int id; char nm[MAX_NLEN],lc[MAX_NLEN];
    unsigned int cap,status,eq,am,bc; } RmT;
typedef struct { unsigned int ri,ei; int by; unsigned int st; DtT s,e; } BkT;
typedef struct { char nm[MAX_NLEN]; unsigned int lv; int ow,tw; unsigned int ci; } ShT;
typedef struct { char nm[MAX_NLEN],uri[MAX_ULEN]; unsigned int ct,rf,ls; int si; } SuT;
typedef struct { unsigned int cal,ev,rm,bk,rem,sh,su; } StT;
static CalT g_cal[MAX_CALENDARS]; static EvT g_ev[MAX_EVENTS];
static RmT g_rm[MAX_ROOMS]; static BkT g_bk[MAX_EVENTS];
static RemT g_re[MAX_REMINDERS]; static ShT g_sh[MAX_SHARES];
static SuT g_su[MAX_SHARES]; static StT g_st;
static void log_msg(const char *m);
static void log_u(unsigned int v);
static unsigned int sa_u(unsigned int a, unsigned int b);
static int val_dt(const DtT *d);
static int cmp_dt(const DtT *a, const DtT *b);
static int dt_ov(const DtT *s1, const DtT *e1, const DtT *s2, const DtT *e2);
static void gen_uid(char *o, unsigned int mx, unsigned int sq);
static unsigned int ep_mn(const DtT *d);
static unsigned int dim(unsigned int y, unsigned int m);
static void sc(char *d, const char *s, unsigned int mx);
static void iapp(char *b, unsigned int *p, unsigned int mx, const char *s);
static DtT mk_dt(unsigned int y, unsigned int mo, unsigned int d, unsigned int h, unsigned int mi);
static int cal_init(void), cal_cr(const char *nm, unsigned int tp, int ow);
static int cal_sv(unsigned int i, unsigned int m), cal_gv(unsigned int i);
static int cal_dl(unsigned int i), cal_ls(unsigned int o, unsigned int l);
static int ev_cr(unsigned int c, const char *t, const DtT *s, const DtT *e, int o);
static int ev_ed(unsigned int i, const char *t, const DtT *s, const DtT *e);
static int ev_dl(unsigned int i), ev_sr(unsigned int i, unsigned int r, unsigned int en);
static int ev_ap(unsigned int i, int u, unsigned int rl), ev_rp(unsigned int i, int u);
static int ev_rm(unsigned int i, int r);
static int ev_lc(unsigned int c, unsigned int o, unsigned int l);
static int ev_lr(const DtT *f, const DtT *t);
static int rm_ad(unsigned int e, unsigned int m, unsigned int t), rm_fr(unsigned int n);
static int rm_rg(const char *n, const char *l, unsigned int c, unsigned int q);
static int rm_sa(unsigned int r, unsigned int m), rm_qr(unsigned int c, unsigned int q);
static int rm_bk(unsigned int r, unsigned int e, int b);
static int rm_ap(unsigned int b), rm_rj(unsigned int b);
static int rm_cf(unsigned int r, const DtT *s, const DtT *e), rm_ls(unsigned int o, unsigned int l);
static int sf_fs(const int *u, unsigned int n, const DtT *ws, const DtT *we, unsigned int d);
static int sf_ct(const int *u, unsigned int n, unsigned int d);
static int sf_rc(unsigned int c, unsigned int d), sf_aa(unsigned int e);
static int sh_cl(unsigned int c, int o, int t, unsigned int l), sh_rv(unsigned int c, int t);
static int sh_fb(int v, const DtT *f, const DtT *t), sh_tm(int l, unsigned int lm);
static int su_ad(const char *n, const char *u, int s);
static int su_sy(unsigned int i), su_rm(unsigned int i);
static int ic_ex(unsigned int e, char *b, unsigned int l), ic_im(const char *d, unsigned int c);
static int cd_sy(unsigned int c), em_iv(unsigned int e, int p);
static int rs_gt(unsigned int c, unsigned int o, unsigned int l);
static int rs_ps(const char *b, unsigned int c);
static int st_pr(void), demo(void);
static void log_msg(const char *m) { host_print(m); }
static void log_u(unsigned int v) {
    char b[20]; int i=18; b[19]=0;
    if (v==0U) { host_print("0"); return; }
    while (v>0U && i>=0) { b[i]=(char)('0'+(int)(v%10U)); v/=10U; i--; }
    host_print(&b[i+1]);
}
static unsigned int sa_u(unsigned int a, unsigned int b) { unsigned int r=a+b; return (r<a)?0xFFFFFFFFU:r; }
static unsigned int dim(unsigned int y, unsigned int m) {
    unsigned int d[12]={31U,28U,31U,30U,31U,30U,31U,31U,30U,31U,30U,31U};
    if (m<1U||m>12U) return 0U;
    if (m==2U&&((y%4U==0U&&y%100U!=0U)||y%400U==0U)) return 29U;
    return d[m-1U];
}
static int val_dt(const DtT *d) {
    if (!d) return 0;
    if (d->year<1970U||d->year>2100U) return 0;
    if (d->month<1U||d->month>12U) return 0;
    if (d->day<1U||d->day>dim(d->year,d->month)) return 0;
    return (d->hour<=23U&&d->minute<=59U&&d->second<=59U);
}
static int cmp_dt(const DtT *a, const DtT *b) {
    if (!a||!b) return 0;
    if (a->year!=b->year) return (a->year>b->year)?1:-1;
    if (a->month!=b->month) return (a->month>b->month)?1:-1;
    if (a->day!=b->day) return (a->day>b->day)?1:-1;
    if (a->hour!=b->hour) return (a->hour>b->hour)?1:-1;
    if (a->minute!=b->minute) return (a->minute>b->minute)?1:-1;
    return (a->second!=b->second)?((a->second>b->second)?1:-1):0;
}
static int dt_ov(const DtT *s1, const DtT *e1, const DtT *s2, const DtT *e2) {
    return (cmp_dt(s1,e2)<0 && cmp_dt(s2,e1)<0);
}
static void gen_uid(char *o, unsigned int mx, unsigned int sq) {
    unsigned int i=0U;
    if (!o||mx<8U) return;
    o[0]='E'; o[1]='V'; o[2]='-'; i=3U;
    if (sq<10U&&i<mx-1U) o[i++]=(char)('0'+sq);
    else if (sq<100U&&i<mx-2U) { o[i++]=(char)('0'+sq/10U); o[i++]=(char)('0'+sq%10U); }
    else if (sq<1000U&&i<mx-3U) { o[i++]=(char)('0'+sq/100U); o[i++]=(char)('0'+(sq/10U)%10U); o[i++]=(char)('0'+sq%10U); }
    o[i]=0;
}
static unsigned int ep_mn(const DtT *d) {
    if (!d) return 0U;
    unsigned int t=(d->year-1970U)*525600U;
    t=sa_u(t,(d->month-1U)*43800U); t=sa_u(t,(d->day-1U)*1440U);
    t=sa_u(t,d->hour*60U); return sa_u(t,d->minute);
}
static void sc(char *d, const char *s, unsigned int mx) {
    unsigned int i=0U; if (!d||!s||mx==0U) return;
    while (s[i]&&i<mx-1U) { d[i]=s[i]; i++; } d[i]=0;
}
static void iapp(char *b, unsigned int *p, unsigned int mx, const char *s) {
    unsigned int i=0U; if (!b||!p||!s) return;
    while (s[i]&&*p<mx-1U) { b[*p]=s[i]; (*p)++; i++; }
}
static DtT mk_dt(unsigned int y, unsigned int mo, unsigned int d, unsigned int h, unsigned int mi) {
    DtT dt; dt.year=y; dt.month=mo; dt.day=d; dt.hour=h; dt.minute=mi; dt.second=0U; dt.off=0; return dt;
}
/* Calendar management */
static int cal_init(void) {
    g_st.cal=0U; g_st.ev=0U; g_st.rm=0U; g_st.bk=0U;
    g_st.rem=0U; g_st.sh=0U; g_st.su=0U;
    log_msg("[calendar_scheduler] initialized\n"); return 0;
}
static int cal_cr(const char *nm, unsigned int tp, int ow) {
    unsigned int i;
    if (!nm||g_st.cal>=MAX_CALENDARS) return -1;
    if (tp!=CT_L&&tp!=CT_C&&tp!=CT_S) return -2;
    i=g_st.cal;
    g_cal[i].uid[0]='C'; g_cal[i].uid[1]='A'; g_cal[i].uid[2]='-';
    g_cal[i].uid[3]=(char)('0'+i); g_cal[i].uid[4]=0;
    sc(g_cal[i].name,nm,MAX_NLEN); g_cal[i].type=tp; g_cal[i].status=ST_ACT;
    g_cal[i].vmode=V_MONTH; g_cal[i].owner=ow; g_cal[i].ec=0U; g_cal[i].sc=1U; g_cal[i].tzi=0U;
    g_st.cal=sa_u(g_st.cal,1U); log_msg("[cal] created: "); log_msg(nm); log_msg("\n"); return (int)i;
}
static int cal_sv(unsigned int i, unsigned int m) {
    if (i>=g_st.cal) return -1;
    if (m!=V_DAY&&m!=V_WEEK&&m!=V_MONTH&&m!=V_YEAR) return -2;
    g_cal[i].vmode=m; log_msg("[cal] view idx="); log_u(i); log_msg("\n"); return 0;
}
static int cal_gv(unsigned int i) { return (i>=g_st.cal)?-1:(int)g_cal[i].vmode; }
static int cal_dl(unsigned int i) {
    if (i>=g_st.cal) return -1; g_cal[i].status=ST_INA;
    log_msg("[cal] deleted idx="); log_u(i); log_msg("\n"); return 0;
}
static int cal_ls(unsigned int o, unsigned int l) {
    unsigned int i,e,sh=0U; e=sa_u(o,l); if (e>g_st.cal) e=g_st.cal;
    log_msg("[cal] listing:\n");
    for (i=o; i<e; i++) if (g_cal[i].status==ST_ACT) {
        log_msg("  ["); log_u(i); log_msg("] "); log_msg(g_cal[i].name); log_msg("\n"); sh=sa_u(sh,1U);
    }
    return (int)sh;
}

/* Event management */
static int ev_cr(unsigned int c, const char *t, const DtT *s, const DtT *e, int o) {
    unsigned int i;
    if (c>=g_st.cal||g_st.ev>=MAX_EVENTS) return -1;
    if (!t||!s||!e||!val_dt(s)||!val_dt(e)||cmp_dt(s,e)>=0) return -2;
    i=g_st.ev; gen_uid(g_ev[i].uid,MAX_UIDLEN,i); sc(g_ev[i].title,t,MAX_NLEN);
    g_ev[i].s=*s; g_ev[i].e=*e; g_ev[i].ci=c; g_ev[i].rec=R_NONE; g_ev[i].re=0U;
    g_ev[i].pri=1U; g_ev[i].status=ST_ACT; g_ev[i].pc=0U; g_ev[i].rc=0U;
    g_ev[i].rid=-1; g_ev[i].org=o;
    g_cal[c].ec=sa_u(g_cal[c].ec,1U); g_st.ev=sa_u(g_st.ev,1U);
    log_msg("[event] created: "); log_msg(t); log_msg("\n"); return (int)i;
}
static int ev_ed(unsigned int i, const char *t, const DtT *s, const DtT *e) {
    if (i>=g_st.ev||g_ev[i].status!=ST_ACT) return -1;
    if (t) sc(g_ev[i].title,t,MAX_NLEN);
    if (s&&val_dt(s)) g_ev[i].s=*s; if (e&&val_dt(e)) g_ev[i].e=*e;
    log_msg("[event] edited idx="); log_u(i); log_msg("\n"); return 0;
}
static int ev_dl(unsigned int i) {
    if (i>=g_st.ev) return -1; g_ev[i].status=ST_INA;
    log_msg("[event] deleted idx="); log_u(i); log_msg("\n"); return 0;
}
static int ev_sr(unsigned int i, unsigned int r, unsigned int en) {
    if (i>=g_st.ev) return -1;
    if (r!=R_DAILY&&r!=R_WEEKLY&&r!=R_MONTHLY&&r!=R_YEARLY) return -2;
    g_ev[i].rec=r; g_ev[i].re=en; log_msg("[event] recurrence idx="); log_u(i); log_msg("\n"); return 0;
}
static int ev_ap(unsigned int i, int u, unsigned int rl) {
    unsigned int p;
    if (i>=g_st.ev||g_ev[i].pc>=MAX_PARTIP) return -1;
    if (rl!=PT_R&&rl!=PT_O&&rl!=PT_S) return -2;
    p=g_ev[i].pc; g_ev[i].pids[p]=u; g_ev[i].pc=sa_u(p,1U);
    log_msg("[event] part added ev="); log_u(i); log_msg("\n"); return 0;
}
static int ev_rp(unsigned int i, int u) {
    unsigned int j;
    if (i>=g_st.ev) return -1;
    for (j=0U; j<g_ev[i].pc; j++) if (g_ev[i].pids[j]==u) {
        unsigned int k; for (k=j; k<g_ev[i].pc-1U; k++) g_ev[i].pids[k]=g_ev[i].pids[sa_u(k,1U)];
        g_ev[i].pc-=1U; log_msg("[event] part removed ev="); log_u(i); log_msg("\n"); return 0;
    }
    return -2;
}
static int ev_rm(unsigned int i, int r) {
    if (i>=g_st.ev) return -1; if (r>=0&&(unsigned int)r>=g_st.rm) return -2;
    g_ev[i].rid=r; log_msg("[event] room set ev="); log_u(i); log_msg("\n"); return 0;
}
static int ev_lc(unsigned int c, unsigned int o, unsigned int l) {
    unsigned int i,e,sh=0U; if (c>=g_st.cal) return -1;
    e=sa_u(o,l); if (e>g_st.ev) e=g_st.ev; log_msg("[event] cal "); log_u(c); log_msg(":\n");
    for (i=o; i<e; i++) if (g_ev[i].ci==c&&g_ev[i].status==ST_ACT) {
        log_msg("  ["); log_u(i); log_msg("] "); log_msg(g_ev[i].title); log_msg("\n"); sh=sa_u(sh,1U);
    }
    return (int)sh;
}
static int ev_lr(const DtT *f, const DtT *t) {
    unsigned int i,sh=0U; if (!f||!t) return -1; log_msg("[event] range:\n");
    for (i=0U; i<g_st.ev; i++) if (g_ev[i].status==ST_ACT&&dt_ov(&g_ev[i].s,&g_ev[i].e,f,t)) {
        log_msg("  ["); log_u(i); log_msg("] "); log_msg(g_ev[i].title); log_msg("\n"); sh=sa_u(sh,1U);
    }
    return (int)sh;
}

/* Reminders */
static int rm_ad(unsigned int ei, unsigned int mb, unsigned int t) {
    unsigned int i;
    if (ei>=g_st.ev||g_st.rem>=MAX_REMINDERS) return -1;
    if (t!=RM_E&&t!=RM_P&&t!=RM_S) return -2;
    i=g_st.rem; g_re[i].ei=ei; g_re[i].mb=mb; g_re[i].type=t; g_re[i].sent=0U;
    g_ev[ei].ri[i]=i; g_ev[ei].rc=sa_u(g_ev[ei].rc,1U); g_st.rem=sa_u(g_st.rem,1U);
    log_msg("[reminder] added ev="); log_u(ei); log_msg("\n"); return (int)i;
}
static int rm_fr(unsigned int now) {
    unsigned int i,fd=0U;
    for (i=0U; i<g_st.rem; i++) {
        unsigned int ei; if (g_re[i].sent) continue; ei=g_re[i].ei;
        if (ei>=g_st.ev) continue;
        if (now>=ep_mn(&g_ev[ei].s)-g_re[i].mb) {
            g_re[i].sent=1U; log_msg("[reminder] fired ev="); log_u(ei); log_msg("\n"); fd=sa_u(fd,1U);
        }
    }
    return (int)fd;
}

/* Room booking */
static int rm_rg(const char *n, const char *l, unsigned int c, unsigned int q) {
    unsigned int i; if (!n||g_st.rm>=MAX_ROOMS||c==0U) return -1;
    i=g_st.rm; g_rm[i].id=(int)i; sc(g_rm[i].nm,n,MAX_NLEN);
    if (l) sc(g_rm[i].lc,l,MAX_NLEN);
    g_rm[i].cap=c; g_rm[i].eq=q; g_rm[i].status=ST_ACT; g_rm[i].am=RA_A; g_rm[i].bc=0U;
    g_st.rm=sa_u(g_st.rm,1U); log_msg("[room] registered: "); log_msg(n); log_msg("\n"); return (int)i;
}
static int rm_sa(unsigned int r, unsigned int m) {
    if (r>=g_st.rm) return -1;
    if (m!=RA_N&&m!=RA_M&&m!=RA_A) return -2;
    g_rm[r].am=m; log_msg("[room] appr rid="); log_u(r); log_msg("\n"); return 0;
}
static int rm_qr(unsigned int c, unsigned int q) {
    unsigned int i,fd=0U; log_msg("[room] query cap>="); log_u(c); log_msg("\n");
    for (i=0U; i<g_st.rm; i++) {
        if (g_rm[i].status!=ST_ACT||g_rm[i].cap<c) continue;
        if ((g_rm[i].eq&q)!=q) continue;
        log_msg("  ["); log_u(i); log_msg("] "); log_msg(g_rm[i].nm);
        log_msg(" cap="); log_u(g_rm[i].cap); log_msg("\n"); fd=sa_u(fd,1U);
    }
    return (int)fd;
}
static int rm_cf(unsigned int r, const DtT *s, const DtT *e) {
    unsigned int i; if (r>=g_st.rm||!s||!e) return -1;
    for (i=0U; i<g_st.bk; i++) {
        if (g_bk[i].ri!=r) continue;
        if (g_bk[i].st==BK_C||g_bk[i].st==BK_RJ) continue;
        if (dt_ov(&g_bk[i].s,&g_bk[i].e,s,e)) return 1;
    }
    return 0;
}
static int rm_bk(unsigned int r, unsigned int ei, int b) {
    unsigned int i;
    if (r>=g_st.rm||ei>=g_st.ev||g_st.bk>=MAX_EVENTS) return -1;
    if (rm_cf(r,&g_ev[ei].s,&g_ev[ei].e)!=0) {
        log_msg("[room] conflict rid="); log_u(r); log_msg("\n"); return -2;
    }
    i=g_st.bk; g_bk[i].ri=r; g_bk[i].ei=ei; g_bk[i].by=b;
    g_bk[i].s=g_ev[ei].s; g_bk[i].e=g_ev[ei].e;
    g_bk[i].st=(g_rm[r].am==RA_A)?BK_A:BK_P;
    g_rm[r].bc=sa_u(g_rm[r].bc,1U); g_st.bk=sa_u(g_st.bk,1U); g_ev[ei].rid=(int)r;
    log_msg("[room] booked rid="); log_u(r); log_msg(" ev="); log_u(ei); log_msg("\n"); return (int)i;
}
static int rm_ap(unsigned int b) {
    if (b>=g_st.bk||g_bk[b].st!=BK_P) return -1;
    g_bk[b].st=BK_A; log_msg("[room] approved bi="); log_u(b); log_msg("\n"); return 0;
}
static int rm_rj(unsigned int b) {
    if (b>=g_st.bk||g_bk[b].st!=BK_P) return -1;
    g_bk[b].st=BK_RJ; log_msg("[room] rejected bi="); log_u(b); log_msg("\n"); return 0;
}
static int rm_ls(unsigned int o, unsigned int l) {
    unsigned int i,e,sh=0U; e=sa_u(o,l); if (e>g_st.rm) e=g_st.rm;
    log_msg("[room] listing:\n");
    for (i=o; i<e; i++) if (g_rm[i].status==ST_ACT) {
        log_msg("  ["); log_u(i); log_msg("] "); log_msg(g_rm[i].nm);
        log_msg(" cap="); log_u(g_rm[i].cap); log_msg("\n"); sh=sa_u(sh,1U);
    }
    return (int)sh;
}

/* Smart scheduling */
static int sf_fs(const int *u, unsigned int n, const DtT *ws, const DtT *we, unsigned int d) {
    unsigned int i; DtT c;
    if (!u||n==0U||!ws||!we||d==0U) return -1;
    log_msg("[smart] free slot users="); log_u(n); log_msg(" dur="); log_u(d); log_msg("\n");
    c=*ws;
    for (i=0U; i<48U; i++) {
        DtT ce; unsigned int e; int ok;
        c.minute=sa_u(c.minute,30U);
        if (c.minute>=60U) { c.minute-=60U; c.hour=sa_u(c.hour,1U); }
        if (c.hour>=24U) { c.hour=0U; c.day=sa_u(c.day,1U);
            if (c.day>dim(c.year,c.month)) { c.day=1U; c.month=sa_u(c.month,1U); }
            if (c.month>12U) { c.month=1U; c.year=sa_u(c.year,1U); } }
        if (cmp_dt(&c,we)>=0) break;
        ce=c; ce.minute=sa_u(ce.minute,d);
        if (ce.minute>=60U) { ce.minute-=60U; ce.hour=sa_u(ce.hour,1U); }
        ok=1;
        for (e=0U; e<g_st.ev; e++) {
            if (g_ev[e].status!=ST_ACT) continue;
            if (dt_ov(&c,&ce,&g_ev[e].s,&g_ev[e].e)) { ok=0; break; }
        }
        if (ok) { log_msg("[smart] slot h="); log_u(c.hour); log_msg(" m="); log_u(c.minute); log_msg("\n"); return (int)i; }
    }
    log_msg("[smart] no slot\n"); return -2;
}
static int sf_ct(const int *u, unsigned int n, unsigned int d) {
    unsigned int i,best=14U; if (!u||n==0U) return -1;
    log_msg("[smart] coord tz users="); log_u(n); log_msg("\n");
    for (i=0U; i<n; i++) { log_msg("  uid="); log_u((unsigned int)u[i]); log_msg("\n"); }
    if (d>60U) best=10U;
    log_msg("[smart] rec h="); log_u(best); log_msg("\n"); return (int)best;
}
static int sf_rc(unsigned int c, unsigned int d) {
    DtT ws,we; int ids[1];
    if (c>=g_st.cal) return -1;
    log_msg("[smart] rec cal="); log_u(c); log_msg(" dur="); log_u(d); log_msg("\n");
    ws=mk_dt(2026U,6U,17U,9U,0U); we=mk_dt(2026U,6U,17U,17U,0U);
    ids[0]=g_cal[c].owner; return sf_fs(ids,1U,&ws,&we,d);
}
static int sf_aa(unsigned int ei) {
    unsigned int i,br=0xFFFFFFFFU,bs=0xFFFFFFFFU,nd;
    if (ei>=g_st.ev) return -1;
    log_msg("[smart] auto-arrange ev="); log_u(ei); log_msg("\n");
    nd=(g_ev[ei].pc>0U)?g_ev[ei].pc:1U;
    for (i=0U; i<g_st.rm; i++) {
        unsigned int w; if (g_rm[i].status!=ST_ACT||g_rm[i].cap<nd) continue;
        w=g_rm[i].cap-nd; if (rm_cf(i,&g_ev[ei].s,&g_ev[ei].e)!=0) continue;
        if (w<bs) { bs=w; br=i; }
    }
    if (br!=0xFFFFFFFFU) { g_ev[ei].rid=(int)br;
        log_msg("[smart] assigned room "); log_u(br); log_msg("\n"); return (int)br; }
    log_msg("[smart] no room\n"); return -2;
}

/* Shared collaboration */
static int sh_cl(unsigned int c, int o, int t, unsigned int l) {
    unsigned int i;
    if (c>=g_st.cal||g_st.sh>=MAX_SHARES) return -1;
    if (l!=SH_R&&l!=SH_W&&l!=SH_X) return -2;
    if (g_cal[c].owner!=o) return -3;
    i=g_st.sh; g_sh[i].ci=c; g_sh[i].ow=o; g_sh[i].tw=t; g_sh[i].lv=l;
    g_sh[i].nm[0]='S'; g_sh[i].nm[1]='-'; g_sh[i].nm[2]=(char)('0'+i); g_sh[i].nm[3]=0;
    g_cal[c].sc=sa_u(g_cal[c].sc,1U); g_st.sh=sa_u(g_st.sh,1U);
    log_msg("[share] cal "); log_u(c); log_msg(" -> "); log_u((unsigned int)t); log_msg("\n"); return (int)i;
}
static int sh_rv(unsigned int c, int t) {
    unsigned int i; if (c>=g_st.cal) return -1;
    for (i=0U; i<g_st.sh; i++) if (g_sh[i].ci==c&&g_sh[i].tw==t) {
        g_sh[i].lv=0U; log_msg("[share] revoked "); log_u((unsigned int)t); log_msg("\n"); return 0;
    }
    return -2;
}
static int sh_fb(int v, const DtT *f, const DtT *t) {
    unsigned int i,cn=0U; if (!f||!t) return -1;
    log_msg("[share] freebusy v="); log_u((unsigned int)v); log_msg("\n");
    for (i=0U; i<g_st.ev; i++) if (g_ev[i].status==ST_ACT&&dt_ov(&g_ev[i].s,&g_ev[i].e,f,t)) {
        log_msg("  busy ["); log_u(i); log_msg("] "); log_msg(g_ev[i].title); log_msg("\n"); cn=sa_u(cn,1U);
    }
    log_msg("[share] busy="); log_u(cn); log_msg("\n"); return (int)cn;
}
static int sh_tm(int l, unsigned int lm) {
    unsigned int i,sh=0U; log_msg("[share] team lead="); log_u((unsigned int)l); log_msg("\n");
    for (i=0U; i<g_st.cal&&sh<lm; i++) if (g_cal[i].status==ST_ACT&&g_cal[i].owner==l) {
        log_msg("  cal: "); log_msg(g_cal[i].name); log_msg(" ev="); log_u(g_cal[i].ec); log_msg("\n");
        sh=sa_u(sh,1U);
    }
    return (int)sh;
}

/* Subscriptions */
static int su_ad(const char *n, const char *u, int s) {
    unsigned int i; if (!n||!u||g_st.su>=MAX_SHARES) return -1;
    i=g_st.su; sc(g_su[i].nm,n,MAX_NLEN); sc(g_su[i].uri,u,MAX_ULEN);
    g_su[i].ct=CT_S; g_su[i].rf=3600U; g_su[i].ls=0U; g_su[i].si=s;
    g_st.su=sa_u(g_st.su,1U); log_msg("[sub] added: "); log_msg(n); log_msg("\n"); return (int)i;
}
static int su_sy(unsigned int i) {
    if (i>=g_st.su) return -1;
    g_su[i].ls=sa_u(g_su[i].ls,g_su[i].rf);
    log_msg("[sub] synced: "); log_msg(g_su[i].nm); log_msg("\n"); return 0;
}
static int su_rm(unsigned int i) {
    if (i>=g_st.su) return -1; g_su[i].nm[0]=0;
    log_msg("[sub] removed "); log_u(i); log_msg("\n"); return 0;
}

/* Integration interfaces */
static int ic_ex(unsigned int ei, char *b, unsigned int l) {
    unsigned int p=0U; if (ei>=g_st.ev||!b||l<64U) return -1;
    iapp(b,&p,l,"BEGIN:VEVENT\r\nUID:"); iapp(b,&p,l,g_ev[ei].uid);
    iapp(b,&p,l,"\r\nSUMMARY:"); iapp(b,&p,l,g_ev[ei].title);
    iapp(b,&p,l,"\r\nEND:VEVENT\r\n"); b[p]=0;
    log_msg("[ical] exported ev="); log_u(ei); log_msg("\n"); return (int)p;
}
static int ic_im(const char *d, unsigned int c) {
    DtT s,e; if (!d||c>=g_st.cal) return -1;
    s=mk_dt(2026U,6U,17U,10U,0U); e=mk_dt(2026U,6U,17U,11U,0U);
    int i=ev_cr(c,"Imported Event",&s,&e,0);
    if (i>=0) { log_msg("[ical] imported idx="); log_u((unsigned int)i); log_msg("\n"); }
    return i;
}
static int cd_sy(unsigned int c) {
    if (c>=g_st.cal) return -1;
    if (g_cal[c].type!=CT_C) { log_msg("[caldav] not CalDAV\n"); return -2; }
    log_msg("[caldav] synced cal="); log_u(c); log_msg(" ev="); log_u(g_cal[c].ec); log_msg("\n"); return 0;
}
static int em_iv(unsigned int ei, int p) {
    if (ei>=g_st.ev) return -1;
    log_msg("[email] invite ev="); log_u(ei); log_msg(" u="); log_u((unsigned int)p); log_msg(" sent\n"); return 0;
}
static int rs_gt(unsigned int c, unsigned int o, unsigned int l) {
    if (c>=g_st.cal) return -1;
    log_msg("[rest] GET /calendars/"); log_u(c); log_msg("/events o="); log_u(o); log_msg(" l="); log_u(l); log_msg("\n");
    return ev_lc(c,o,l);
}
static int rs_ps(const char *b, unsigned int c) {
    DtT s,e; if (!b||c>=g_st.cal) return -1;
    s=mk_dt(2026U,6U,18U,14U,0U); e=mk_dt(2026U,6U,18U,15U,0U);
    log_msg("[rest] POST /calendars/"); log_u(c); log_msg("/events\n");
    int i=ev_cr(c,"REST Created Event",&s,&e,0);
    if (i>=0) { log_msg("[rest] created idx="); log_u((unsigned int)i); log_msg("\n"); }
    return i;
}

static int st_pr(void) {
    log_msg("=== Stats ===\n");
    log_msg("  Calendars: "); log_u(g_st.cal); log_msg("\n");
    log_msg("  Events: "); log_u(g_st.ev); log_msg("\n");
    log_msg("  Rooms: "); log_u(g_st.rm); log_msg("\n");
    log_msg("  Bookings: "); log_u(g_st.bk); log_msg("\n");
    log_msg("  Reminders: "); log_u(g_st.rem); log_msg("\n");
    log_msg("  Shares: "); log_u(g_st.sh); log_msg("\n");
    log_msg("  Subscriptions: "); log_u(g_st.su); log_msg("\n");
    return 0;
}
static int demo(void) {
    DtT s1,e1,s2,e2,rf,rt; int cw,cp,ev1,ev2,ra,rb; int ids[2];
    log_msg("=== Calendar Scheduler Demo ===\n"); cal_init();
    cw=cal_cr("Work",CT_L,1); cp=cal_cr("Personal",CT_L,2); (void)cal_cr("Holidays",CT_S,1);
    s1=mk_dt(2026U,6U,17U,9U,0U); e1=mk_dt(2026U,6U,17U,10U,0U);
    s2=mk_dt(2026U,6U,17U,14U,0U); e2=mk_dt(2026U,6U,17U,15U,30U);
    ev1=ev_cr((unsigned int)cw,"Team Standup",&s1,&e1,1);
    ev2=ev_cr((unsigned int)cw,"Design Review",&s2,&e2,1);
    (void)ev_cr((unsigned int)cp,"Lunch",&s1,&e1,2);
    (void)ev_sr((unsigned int)ev1,R_DAILY,0U);
    (void)ev_ap((unsigned int)ev1,2,PT_R); (void)ev_ap((unsigned int)ev1,3,PT_O);
    (void)ev_ap((unsigned int)ev2,4,PT_R);
    (void)rm_ad((unsigned int)ev1,15U,RM_P); (void)rm_ad((unsigned int)ev2,30U,RM_E);
    ra=rm_rg("Conf Room A","Floor 2",10U,0x03U); rb=rm_rg("Conf Room B","Floor 3",20U,0x07U);
    (void)rm_rg("Board Room","Floor 5",50U,0x0FU); (void)rm_sa((unsigned int)rb,RA_M);
    if (ev1>=0) (void)rm_bk((unsigned int)ra,(unsigned int)ev1,1);
    if (ev2>=0) (void)rm_bk((unsigned int)rb,(unsigned int)ev2,1);
    (void)rm_qr(10U,0x03U); ids[0]=1; ids[1]=2;
    rf=mk_dt(2026U,6U,17U,8U,0U); rt=mk_dt(2026U,6U,17U,18U,0U);
    (void)sf_fs(ids,2U,&rf,&rt,60U); (void)sf_ct(ids,2U,30U);
    if (ev1>=0) (void)sf_rc((unsigned int)cw,45U);
    if (ev2>=0) (void)sf_aa((unsigned int)ev2);
    (void)sh_cl((unsigned int)cw,1,2,SH_W); (void)sh_cl((unsigned int)cw,1,3,SH_R);
    (void)sh_fb(2,&rf,&rt); (void)sh_tm(1,10U);
    (void)su_ad("US Holidays","https://example.com/holidays.ics",1);
    (void)su_ad("Team Cal","https://example.com/team.ics",2); (void)su_sy(0U);
    if (ev1>=0) { char ib[512]; (void)ic_ex((unsigned int)ev1,ib,512U); }
    (void)ic_im("BEGIN:VEVENT\r\nEND:VEVENT",(unsigned int)cp);
    (void)rs_gt((unsigned int)cw,0U,10U);
    (void)rs_ps("{\"title\":\"API Meeting\"}",(unsigned int)cw);
    (void)cal_sv((unsigned int)cw,V_WEEK); (void)cal_ls(0U,10U);
    (void)ev_lc((unsigned int)cw,0U,10U); (void)ev_lr(&rf,&rt);
    (void)rm_ls(0U,10U); (void)rm_fr(500000U); (void)em_iv((unsigned int)ev1,2);
    st_pr(); log_msg("=== Demo Complete ===\n"); return 0;
}

__attribute__((export_name("main")))
int main(void) {
    log_msg("[calendar_scheduler] booting...\n"); demo();
    log_msg("[calendar_scheduler] shutdown\n"); return 0;
}
