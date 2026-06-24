/* asparagus_admin: Asparagus (Fern) ornamental spear crop management (v1.0)
 * Asparagus planning, planting, evaluation, spear harvest, market
 * Features: spear length, fern frond count, stalk count, root age, harvest week, grade
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spear_len,fern_fr,stalk_ct,root_age,harv_wk,grade,active;} asp_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_spear,t_fern,t_stalk,t_root,t_grade;} asp_state_t;
static asp_t aspps[N],aspes[N-2],aspvs[N-4],asphv[N-6],aspms[N-6]; static asp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asp_t*a,int*cnt,int*sum,int mx,int lc,int sl,int ff,int sc,int ra,int hw,int gr){if(*cnt>=mx)return -1;asp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spear_len=sl;x->fern_fr=ff;x->stalk_ct=sc;x->root_age=ra;x->harv_wk=hw;x->grade=gr;x->active=1;*sum+=sl;(*cnt)++;ps("[ASP] Asparagus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sl=");pi(sl);ps(" ff=");pi(ff);ps(" sc=");pi(sc);ps(" ra=");pi(ra);ps(" hw=");pi(hw);ps(" gr=");pi(gr);ps("\n");return *cnt-1;}
int asp_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_spear=0;st.t_fern=0;st.t_stalk=0;st.t_root=0;st.t_grade=0;for(int i=0;i<N;i++)aspps[i].active=0;for(int i=0;i<N-2;i++)aspes[i].active=0;for(int i=0;i<N-4;i++)aspvs[i].active=0;for(int i=0;i<N-6;i++)asphv[i].active=0;for(int i=0;i<N-6;i++)aspms[i].active=0;init=1;ps("[ASP] Asparagus initialized\n");return 0;}
/* 1=vegetable_farm 2=home_garden 3=container 4=ornamental 5=cutting */
int asp_planning(int lc,int sl,int ff,int sc,int ra,int hw,int gr){return add(aspps,&st.n_plan,&st.t_spear,N,lc,sl,ff,sc,ra,hw,gr);}
int asp_execution(int lc,int sl,int ff,int sc,int ra,int hw,int gr){return add(aspes,&st.n_exec,&st.t_fern,N-2,lc,sl,ff,sc,ra,hw,gr);}
int asp_evaluation(int lc,int sl,int ff,int sc,int ra,int hw,int gr){return add(aspvs,&st.n_eval,&st.t_stalk,N-4,lc,sl,ff,sc,ra,hw,gr);}
int asp_spear_harvest(int lc,int sl,int ff,int sc,int ra,int hw,int gr){return add(asphv,&st.n_harv,&st.t_root,N-6,lc,sl,ff,sc,ra,hw,gr);}
int asp_market(int lc,int sl,int ff,int sc,int ra,int hw,int gr){return add(aspms,&st.n_mkt,&st.t_grade,N-6,lc,sl,ff,sc,ra,hw,gr);}
void asp_report(void){ps("[ASP] Plan: ");pi(st.n_plan);ps(" spear=");pi(st.t_spear);ps("\nExec: ");pi(st.n_exec);ps(" fern=");pi(st.t_fern);ps("\nEval: ");pi(st.n_eval);ps(" stalk=");pi(st.t_stalk);ps("\nHarv: ");pi(st.n_harv);ps(" root=");pi(st.t_root);ps("\nMkt: ");pi(st.n_mkt);ps(" grade=");pi(st.t_grade);ps("\n");}
void asp_state(void){ps("[ASP] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Asparagus (Fern) Admin Demo ===\n\n");asp_init();
ps("Asparagus planning (spear crop layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;asp_planning(lc,15+(i*3),8+(i%5),4+(i%3),1+(i%4),14+(i%6),(i%3)+1);}
ps("\nAsparagus execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;asp_execution(lc,18+(i*2),10+(i%4),5+(i%3),2+(i%3),16+(i%5),(i%3)+1);}
ps("\nAsparagus evaluation (fern check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;asp_evaluation(lc,20+(i*2),12+(i%3),6+(i%2),3+(i%2),18+(i%4),(i%2)+2);}
ps("\nAsparagus spear harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;asp_spear_harvest(lc,12+(i*3),6+(i%4),3+(i%3),1+(i%3),12+(i%5),(i%3)+1);}
ps("\nAsparagus vegetable market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;asp_market(lc,22+(i*2),14+(i%2),7+(i%2),4+(i%2),20+(i%3),(i%2)+3);}
ps("\n");asp_report();asp_state();ps("\n=== Demo Complete ===\n");return 0;}
