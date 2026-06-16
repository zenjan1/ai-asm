/* self_improving_ai: Self-improving AI with meta-learning and auto-optimization */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_META     16
#define MAX_EXP      32
#define MAX_HP       8
#define MAX_ARCH     6
#define MAX_DIST     16
#define MAX_GOAL     12
#define MAX_WEAK     8
#define NAME_LEN     32
#define DESC_LEN     64
#define BUF_SIZE     512
/* Safety */
#define MAX_IMPROVE_RATE 150
#define MIN_PERF_FLOOR   10
#define MAX_OPT_CYCLES   100
#define MAX_ARCH_MUT     5

/* Task difficulty / performance levels */
#define TASK_MED 1, TASK_HARD 2
#undef TASK_MED
#define TASK_MED   1
#define TASK_HARD  2
#define PERF_FAIR  1, PERF_GOOD 2, PERF_EXCEL 3
#undef PERF_FAIR
#define PERF_FAIR  1
#define PERF_GOOD  2
#define PERF_EXCEL 3

typedef struct { int id; char nm[NAME_LEN]; int diff, adapt, attempts, success, strats, meta; } meta_task_t;
typedef struct { int id, perf, spd, acc, eff, rob; char weak[MAX_WEAK][NAME_LEN]; int wcnt, conf, cyc; } assessment_t;
typedef struct { char nm[NAME_LEN]; int cur, best, lo, hi, step, sens, tuned; } hyperparam_t;
typedef struct { int id; char desc[DESC_LEN]; int lay, w, perf, cost, fit, active; } arch_t;
typedef struct { int id, task, outcome, reward, lesson, ts, reuse; } experience_t;
typedef struct { int src, tgt, type, success; char desc[DESC_LEN]; } distill_t;
typedef struct { int id; char nm[NAME_LEN]; int type, target, cur, pri, deadline, done, cycs; } goal_t;
typedef struct { int rate, floor, cycs, runaway, violations, budget, arch_mut, safe; } safety_t;

static meta_task_t  meta[MAX_META];       static int n_meta = 0;
static experience_t exps[MAX_EXP];        static int n_exps = 0;
static hyperparam_t hps[MAX_HP];          static int n_hps = 0;
static arch_t       archs[MAX_ARCH];      static int n_archs = 0;
static distill_t    dists[MAX_DIST];      static int n_dists = 0;
static goal_t       goals[MAX_GOAL];      static int n_goals = 0;
static assessment_t assess;
static safety_t     safe;
static int cycle = 0, total_improve = 0;

/* String utils */
static int my_strlen(const char *s){int l=0;while(s[l])l++;return l;}
static int my_strcmp(const char *a,const char *b){while(*a&&*b){if(*a!=*b)return *a-*b;a++;b++;}return *a-*b;}
static void my_strcpy(char *d,const char *s){while(*s)*d++=*s++;*d='\0';}
static void my_strncpy(char *d,const char *s,int n){int i=0;while(i<n-1&&s[i]){d[i]=s[i];i++;}d[i]='\0';}

/* Print utils */
static void ps(const char *s){host_print(s);}
static void pi(int v){char b[32];int p=0;if(v<0){b[p++]='-';v=-v;}if(v==0)b[p++]='0';else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}}host_print(b);}
static void pn(void){ps("\n");}
static void ph(const char *t){ps("=== ");ps(t);ps(" ===");pn();}

/* Meta-learning */
static void meta_add(int id,const char *nm,int diff){if(n_meta>=MAX_META)return;meta_task_t *t=&meta[n_meta++];t->id=id;my_strncpy(t->nm,nm,NAME_LEN);t->diff=diff;t->adapt=50;t->attempts=0;t->success=0;t->strats=0;t->meta=0;}
static void meta_learn(int tid,int ok,int strat){for(int i=0;i<n_meta;i++){meta_task_t *t=&meta[i];if(t->id!=tid)continue;t->attempts++;if(ok){t->success=(t->success*(t->attempts-1)+100)/t->attempts;t->strats|=(1<<strat);int sc=0;for(int s=0;s<8;s++)if(t->strats&(1<<s))sc++;t->adapt=50+sc*12;if(t->adapt>100)t->adapt=100;}else{t->success=(t->success*(t->attempts-1))/t->attempts;}t->meta=(t->adapt+t->success)/2;}}
static int meta_adapt(int tid){for(int i=0;i<n_meta;i++){if(meta[i].id!=tid)continue;if(meta[i].adapt>80)return 2;if(meta[i].adapt>60)return 5;if(meta[i].adapt>30)return 10;return 20;}return 20;}
static void meta_run(void){ph("Meta-Learning");ps("Tasks: ");pi(n_meta);pn();for(int i=0;i<n_meta;i++){meta_task_t *t=&meta[i];ps("  [");ps(t->nm);ps("] adapt=");pi(t->adapt);ps("% ok=");pi(t->success);ps("% meta=");pi(t->meta);ps(" est=");pi(meta_adapt(t->id));pn();}}

/* Self-assessment */
static void assess_init(int c){assess.id=c;assess.cyc=c;assess.spd=50;assess.acc=50;assess.eff=50;assess.rob=50;assess.wcnt=0;assess.conf=50;assess.perf=PERF_FAIR;}
static void assess_weak(int sc,const char *nm){if(sc<40&&assess.wcnt<MAX_WEAK){my_strncpy(assess.weak[assess.wcnt],nm,NAME_LEN);assess.wcnt++;}}
static void assess_compute(void){int s=assess.spd+assess.acc+assess.eff+assess.rob,a=s/4;assess.perf=(a>=85)?PERF_EXCEL:(a>=65)?PERF_GOOD:PERF_FAIR;assess.conf=(cycle>10)?80:(cycle>5)?60:30+cycle*5;assess.wcnt=0;assess_weak(assess.spd,"speed");assess_weak(assess.acc,"accuracy");assess_weak(assess.eff,"efficiency");assess_weak(assess.rob,"robustness");}
static const char *pn_perf(int p){return(p==PERF_EXCEL)?"EXCELLENT":(p==PERF_GOOD)?"GOOD":"FAIR";}
static void assess_run(void){assess_compute();ph("Self-Assessment");ps("Cycle: ");pi(assess.cyc);pn();ps("Overall: ");ps(pn_perf(assess.perf));pn();ps("  Speed: ");pi(assess.spd);ps(" Acc: ");pi(assess.acc);ps(" Eff: ");pi(assess.eff);ps(" Rob: ");pi(assess.rob);pn();ps("Confidence: ");pi(assess.conf);ps("% Weaknesses: ");pi(assess.wcnt);pn();for(int i=0;i<assess.wcnt;i++){ps("  - ");ps(assess.weak[i]);pn();}}

/* Auto-optimization */
static void hp_add(const char *nm,int init,int lo,int hi,int step){if(n_hps>=MAX_HP)return;hyperparam_t *h=&hps[n_hps++];my_strncpy(h->nm,nm,NAME_LEN);h->cur=init;h->best=init;h->lo=lo;h->hi=hi;h->step=step;h->sens=50;h->tuned=0;}
static void hp_tune(int idx,int delta){if(idx<0||idx>=n_hps)return;hyperparam_t *h=&hps[idx];h->sens=(delta>5)?90:(delta>2)?70:50;if(delta>0)h->best=h->cur;else if(delta<0){h->cur=h->best-h->step;if(h->cur<h->lo)h->cur=h->lo;}else{h->cur+=h->step;if(h->cur>h->hi)h->cur=h->hi;}h->tuned++;}
static void arch_add(int id,const char *desc,int lay,int w){if(n_archs>=MAX_ARCH)return;arch_t *a=&archs[n_archs++];a->id=id;my_strncpy(a->desc,desc,DESC_LEN);a->lay=lay;a->w=w;a->perf=(lay*10+w)/2;if(a->perf>100)a->perf=100;a->cost=lay*w/10;a->fit=a->perf-a->cost/2;a->active=1;}
static int arch_best(void){int bi=-1,bf=-999;for(int i=0;i<n_archs;i++){if(!archs[i].active)continue;if(archs[i].fit>bf){bf=archs[i].fit;bi=i;}}return bi;}
static void optim_run(void){ph("Auto-Optimization");ps("Hyperparams: ");pi(n_hps);pn();for(int i=0;i<n_hps;i++){hyperparam_t *h=&hps[i];ps("  ");ps(h->nm);ps(" cur=");pi(h->cur);ps(" best=");pi(h->best);ps(" sens=");pi(h->sens);pn();}int b=arch_best();ps("Best arch: ");if(b>=0){ps(archs[b].desc);ps(" fit=");pi(archs[b].fit);}else ps("none");pn();}

/* Experience accumulation */
static void exp_add(int tid,int ok,int rew,int les){if(n_exps>=MAX_EXP)return;experience_t *e=&exps[n_exps++];e->id=n_exps;e->task=tid;e->outcome=ok;e->reward=rew;e->lesson=les;e->ts=cycle;e->reuse=0;}
static int exp_recall(int tid){int bl=-1,br=-999;for(int i=0;i<n_exps;i++){if(exps[i].task!=tid)continue;if(exps[i].reward>br){br=exps[i].reward;bl=exps[i].lesson;exps[i].reuse++;}}return bl;}
static void dist_add(int s,int t,int tp,const char *d){if(n_dists>=MAX_DIST)return;distill_t *di=&dists[n_dists++];di->src=s;di->tgt=t;di->type=tp;di->success=50;my_strncpy(di->desc,d,DESC_LEN);}
static void dist_upd(int i,int ok){if(i<0||i>=n_dists)return;dists[i].success=(dists[i].success*7+ok*3)/10;}
static void exp_run(void){ph("Experience");ps("Records: ");pi(n_exps);ps(" Distilled: ");pi(n_dists);pn();for(int i=0;i<n_exps&&i<5;i++){if(exps[i].reuse>0){ps("  E");pi(exps[i].id);ps(" t=");pi(exps[i].task);ps(" r=");pi(exps[i].reward);ps(" x");pi(exps[i].reuse);pn();}}for(int i=0;i<n_dists&&i<4;i++){ps("  D: ");ps(dists[i].desc);ps(" =");pi(dists[i].success);ps("%");pn();}}

/* Self-goal-setting */
static void goal_add_g(const char *nm,int tp,int tgt,int pri,int dl){if(n_goals>=MAX_GOAL)return;goal_t *g=&goals[n_goals++];g->id=n_goals;my_strncpy(g->nm,nm,NAME_LEN);g->type=tp;g->target=tgt;g->cur=0;g->pri=pri;g->deadline=cycle+dl;g->done=0;g->cycs=0;}
static const char *goal_tn(int t){return(t==0)?"explore":(t==1)?"exploit":(t==2)?"balance":"recover";}
static void goal_auto(void){if(assess.wcnt==0)return;for(int w=0;w<assess.wcnt;w++){const char *wn=assess.weak[w];if(my_strcmp(wn,"speed")==0)goal_add_g("fix_speed",1,70,7,20);else if(my_strcmp(wn,"accuracy")==0)goal_add_g("fix_accuracy",2,70,8,15);else if(my_strcmp(wn,"efficiency")==0)goal_add_g("fix_efficiency",0,70,6,25);else if(my_strcmp(wn,"robustness")==0)goal_add_g("fix_robustness",0,70,9,10);}}
static void goal_update(void){for(int i=0;i<n_goals;i++){goal_t *g=&goals[i];if(g->done)continue;g->cycs++;g->cur=(g->cycs*5+g->pri*2);if(g->cur>100)g->cur=100;if(g->cur>=g->target)g->done=1;if(cycle>g->deadline&&!g->done)g->type=3;}}
static void goal_run(void){goal_update();ph("Goals");ps("Active: ");pi(n_goals);pn();for(int i=0;i<n_goals;i++){goal_t *g=&goals[i];ps("  [");ps(g->done?"DONE":"ACTV");ps("] ");ps(g->nm);ps(" ");ps(goal_tn(g->type));ps(" ");pi(g->cur);ps("/");pi(g->target);ps(" p=");pi(g->pri);pn();}}

/* Safety constraints */
static void safe_init(void){safe.rate=0;safe.floor=MIN_PERF_FLOOR;safe.cycs=0;safe.runaway=0;safe.violations=0;safe.budget=MAX_OPT_CYCLES;safe.arch_mut=0;safe.safe=0;}
static int safe_improve(int d){if(d>MAX_IMPROVE_RATE){safe.violations++;safe.safe=1;ps("[SAFETY] Rate ");pi(d);ps("%>max ");pi(MAX_IMPROVE_RATE);ps("% CLAMP\n");return MAX_IMPROVE_RATE;}return d;}
static int safe_budget(void){if(safe.budget<=0){safe.safe=1;ps("[SAFETY] Budget exhausted\n");return 0;}safe.budget--;return 1;}
static int safe_archmut(void){if(safe.arch_mut>=MAX_ARCH_MUT){safe.violations++;ps("[SAFETY] Arch mutation limit\n");return 0;}safe.arch_mut++;return 1;}
static void safe_detect(void){if(safe.violations>5){safe.runaway=1;safe.safe=1;ps("[SAFETY] RUNAWAY: violations!\n");}if(total_improve>500&&cycle<10){safe.runaway=1;safe.safe=1;ps("[SAFETY] RUNAWAY: too fast!\n");}}
static void safe_run(void){safe_detect();ph("Safety");ps("Safe: ");ps(safe.safe?"ON":"off");ps(" Runaway: ");ps(safe.runaway?"YES":"no");pn();ps("Violations: ");pi(safe.violations);ps(" Budget: ");pi(safe.budget);ps(" ArchMut: ");pi(safe.arch_mut);ps("/");pi(MAX_ARCH_MUT);ps(" Rate: ");pi(safe.rate);ps("%\n");}

/* Improvement cycle */
static void run_cycle(void){
    cycle++;safe.cycs++;
    ps("--- Cycle #");pi(cycle);ps(" ---\n");
    assess_init(cycle);
    assess.spd=40+cycle*3;if(assess.spd>95)assess.spd=95;
    assess.acc=35+cycle*4;if(assess.acc>98)assess.acc=98;
    assess.eff=30+cycle*2;if(assess.eff>90)assess.eff=90;
    assess.rob=45+cycle*2;if(assess.rob>92)assess.rob=92;
    if(!safe_budget())return;
    for(int i=0;i<n_meta;i++)meta_learn(meta[i].id,cycle%3!=0,cycle%4);
    for(int i=0;i<n_meta;i++){int l=exp_recall(meta[i].id);if(l>=0)meta[i].strats|=(1<<l);}
    for(int i=0;i<n_hps;i++)hp_tune(i,(cycle%2==0)?3:-1);
    if(cycle%5==0&&safe_archmut()){arch_add(100+cycle,"search",2+(cycle/5)%6,16+(cycle%4)*8);}
    exp_add(cycle%n_meta,(cycle%4!=0)?1:0,(cycle%4!=0)?(10+cycle):-5,cycle%8);
    if(cycle%3==0&&n_dists<MAX_DIST){dist_add(0,1,cycle%3,"xfer");dist_upd(n_dists-1,60+cycle*2);}
    int d=cycle*5;total_improve+=d;safe.rate=safe_improve(d);
    goal_auto();
    meta_run();assess_run();optim_run();exp_run();goal_run();safe_run();
}

/* Init */
static void init(void){
    safe_init();
    meta_add(0,"pattern_rec",TASK_MED);meta_add(1,"seq_predict",TASK_HARD);
    meta_add(2,"anomaly_det",TASK_MED);meta_add(3,"opt_search",TASK_HARD);
    hp_add("learn_rate",50,1,100,5);hp_add("explore_rate",30,5,80,3);
    hp_add("mem_cap",64,16,256,16);hp_add("batch_sz",32,8,128,8);
    arch_add(1,"shallow_wide",2,64);arch_add(2,"deep_narrow",8,16);arch_add(3,"balanced",4,32);
    exp_add(0,1,15,0);exp_add(1,0,-5,1);exp_add(2,1,20,2);
    dist_add(0,1,0,"pat_to_pred");dist_add(1,2,1,"det_to_opt");
    goal_add_g("baseline",0,50,5,10);goal_add_g("mastery",2,80,8,30);
}

static void help(void){ps("self_improving_ai v1.0\n");ps("Features: meta-learn, self-assess, auto-opt, experience, goals, safety\n");ps("  -h  Help\n  -t N  Run N cycles (default 3)\n");}

static int parse_args(int ac,char **av){int cy=3;for(int i=0;i<ac;i++){if(my_strcmp(av[i],"-h")==0){help();return-1;}if(my_strcmp(av[i],"-t")==0&&i+1<ac){const char *s=av[i+1];int v=0;while(*s>='0'&&*s<='9'){v=v*10+(*s-'0');s++;}if(v>0&&v<=50)cy=v;i++;}}return cy;}

void _start(void){
    unsigned int buf=host_alloc(BUF_SIZE,16);
    int alen=host_get_argv(buf,BUF_SIZE);
    char *bp=(char *)buf;
    char *av[16];int ac=0;
    if(alen>0){av[ac++]=bp;for(int i=0;i<alen&&ac<16;i++){if(bp[i]=='\n'||bp[i]=='\0'){bp[i]='\0';if(i+1<alen&&bp[i+1]!='\0')av[ac++]=&bp[i+1];}}}
    int cy=parse_args(ac,av);
    if(cy<0){host_exit(0);return;}
    ps("╔══════════════════════════════════════════╗\n");
    ps("║  Self-Improving AI System v1.0           ║\n");
    ps("║  Meta-Learning + Auto-Optimization       ║\n");
    ps("╚══════════════════════════════════════════╝\n\n");
    ps("Cycles: ");pi(cy);ps(" Buf: ");pi(BUF_SIZE);ps("B @");pi(buf);pn();
    init();
    for(int c=0;c<cy;c++){run_cycle();if(safe.runaway){ps("[HALT] Runaway\n");break;}pn();}
    ph("Final Summary");
    ps("Cycles: ");pi(cycle);ps(" Improve: ");pi(total_improve);pn();
    ps("Experiences: ");pi(n_exps);ps(" Distilled: ");pi(n_dists);pn();
    int ach=0;for(int i=0;i<n_goals;i++)if(goals[i].done)ach++;
    ps("Goals: ");pi(ach);ps("/");pi(n_goals);pn();
    ps("Violations: ");pi(safe.violations);ps(" Runaway: ");ps(safe.runaway?"YES":"no");pn();
    ps("Performance: ");ps(pn_perf(assess.perf));pn();
    ps("Complete.\n");
    host_exit(0);
}
