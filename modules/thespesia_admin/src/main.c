/* thespesia_admin: Thespesia (Pacific Rosewood) coastal timber tree (v1.0)
 * Thespesia planning, execution, evaluation, timber processing, ornamental market
 * Features: timber quality, coastal wind tolerance, flower production, salt spray resistance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,loc,dbh_cm,timber_gr,flower_ct,wind_tol,salt_res,canopy_d,active;} the_t;
typedef struct{int n_plan,n_exec,n_eval,n_timb,n_mkt,t_dbh,t_timber,t_flower,t_wind,t_salt;} the_state_t;
static the_t theps[N],thees[N-2],thevs[N-4],thets[N-6],thems[N-6]; static the_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(the_t*a,int*cnt,int*sum,int mx,int lc,int db,int tg,int fc,int wt,int sr,int cd){if(*cnt>=mx)return -1;the_t*x=&a[*cnt];x->id=*cnt;x->loc=lc;x->dbh_cm=db;x->timber_gr=tg;x->flower_ct=fc;x->wind_tol=wt;x->salt_res=sr;x->canopy_d=cd;x->active=1;*sum+=db;(*cnt)++;ps("[THE] Thespesia ");pi(*cnt-1);ps(" loc=");pi(lc);ps(" dbh=");pi(db);ps(" tg=");pi(tg);ps(" fl=");pi(fc);ps(" wt=");pi(wt);ps("\n");return *cnt-1;}
int the_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_timb=0;st.n_mkt=0;st.t_dbh=0;st.t_timber=0;st.t_flower=0;st.t_wind=0;st.t_salt=0;for(int i=0;i<N;i++)theps[i].active=0;for(int i=0;i<N-2;i++)thees[i].active=0;for(int i=0;i<N-4;i++)thevs[i].active=0;for(int i=0;i<N-6;i++)thets[i].active=0;for(int i=0;i<N-6;i++)thems[i].active=0;init=1;ps("[THE] Thespesia (Pacific rosewood) initialized\n");return 0;}
/* 1=beachfront 2=coastal_ridge 3=island 4=estuary 5=sheltered_bay */
int the_planning(int lc,int db,int tg,int fc,int wt,int sr,int cd){return add(theps,&st.n_plan,&st.t_dbh,N,lc,db,tg,fc,wt,sr,cd);}
int the_execution(int lc,int db,int tg,int fc,int wt,int sr,int cd){return add(thees,&st.n_exec,&st.t_timber,N-2,lc,db,tg,fc,wt,sr,cd);}
int the_evaluation(int lc,int db,int tg,int fc,int wt,int sr,int cd){return add(thevs,&st.n_eval,&st.t_flower,N-4,lc,db,tg,fc,wt,sr,cd);}
int the_timber(int lc,int db,int tg,int fc,int wt,int sr,int cd){return add(thets,&st.n_timb,&st.t_wind,N-6,lc,db,tg,fc,wt,sr,cd);}
int the_market(int lc,int db,int tg,int fc,int wt,int sr,int cd){return add(thems,&st.n_mkt,&st.t_salt,N-6,lc,db,tg,fc,wt,sr,cd);}
void the_report(void){ps("[THE] Plan: ");pi(st.n_plan);ps(" dbh=");pi(st.t_dbh);ps("\nExec: ");pi(st.n_exec);ps(" timber=");pi(st.t_timber);ps("\nEval: ");pi(st.n_eval);ps(" flower=");pi(st.t_flower);ps("\nTimb: ");pi(st.n_timb);ps(" wind=");pi(st.t_wind);ps("\nMkt: ");pi(st.n_mkt);ps(" salt=");pi(st.t_salt);ps("\n");}
void the_state(void){ps("[THE] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Timb=");pi(st.n_timb);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Thespesia (Pacific Rosewood) Admin Demo ===\n\n");the_init();
ps("Thespesia planning (coastal site)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;the_planning(lc,25+(i*8),(i%4)+1,15+(i*5),(i%5)+1,(i%4)+1,6+(i*2));}
ps("\nThespesia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;the_execution(lc,30+(i*6),(i%3)+2,18+(i*4),(i%4)+2,(i%3)+2,7+(i*2));}
ps("\nThespesia evaluation (timber grade)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;the_evaluation(lc,35+(i*5),(i%3)+2,20+(i*3),(i%3)+2,(i%3)+2,8+(i*1));}
ps("\nThespesia timber processing...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;the_timber(lc,20+(i*7),(i%4)+1,12+(i*4),(i%5)+1,(i%4)+1,5+(i*2));}
ps("\nThespesia ornamental market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;the_market(lc,40+(i*4),(i%3)+2,22+(i*3),(i%3)+2,(i%3)+2,9+(i*1));}
ps("\n");the_report();the_state();ps("\n=== Demo Complete ===\n");return 0;}
