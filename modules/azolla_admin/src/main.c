/* azolla_admin: Azolla (Water Fern) aquatic surface management (v1.0)
 * Azolla planning, seeding, evaluation, harvest, market
 * Features: frond size, thickness, green color, growth rate, nitrogen fixation, water depth
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,frond_sz,thick,color_gr,growth_rt,n_fix,water_dp,active;} azol_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_frond,t_thick,t_color,t_growth,t_nfix;} azol_state_t;
static azol_t azolps[N],azoles[N-2],azolvs[N-4],azolhv[N-6],azolms[N-6]; static azol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(azol_t*a,int*cnt,int*sum,int mx,int lc,int fs,int th,int cg,int gr,int nf,int wd){if(*cnt>=mx)return -1;azol_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->frond_sz=fs;x->thick=th;x->color_gr=cg;x->growth_rt=gr;x->n_fix=nf;x->water_dp=wd;x->active=1;*sum+=fs;(*cnt)++;ps("[AZOL] Azolla ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fs=");pi(fs);ps(" th=");pi(th);ps(" cg=");pi(cg);ps(" gr=");pi(gr);ps(" nf=");pi(nf);ps("\n");return *cnt-1;}
int azol_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_frond=0;st.t_thick=0;st.t_color=0;st.t_growth=0;st.t_nfix=0;for(int i=0;i<N;i++)azolps[i].active=0;for(int i=0;i<N-2;i++)azoles[i].active=0;for(int i=0;i<N-4;i++)azolvs[i].active=0;for(int i=0;i<N-6;i++)azolhv[i].active=0;for(int i=0;i<N-6;i++)azolms[i].active=0;init=1;ps("[AZOL] Azolla initialized\n");return 0;}
/* 1=pond 2=paddy_field 3=lagoon 4=container 5=biofilter */
int azol_planning(int lc,int fs,int th,int cg,int gr,int nf,int wd){return add(azolps,&st.n_plan,&st.t_frond,N,lc,fs,th,cg,gr,nf,wd);}
int azol_seeding(int lc,int fs,int th,int cg,int gr,int nf,int wd){return add(azoles,&st.n_exec,&st.t_thick,N-2,lc,fs,th,cg,gr,nf,wd);}
int azol_evaluation(int lc,int fs,int th,int cg,int gr,int nf,int wd){return add(azolvs,&st.n_eval,&st.t_color,N-4,lc,fs,th,cg,gr,nf,wd);}
int azol_harvest(int lc,int fs,int th,int cg,int gr,int nf,int wd){return add(azolhv,&st.n_harv,&st.t_growth,N-6,lc,fs,th,cg,gr,nf,wd);}
int azol_market(int lc,int fs,int th,int cg,int gr,int nf,int wd){return add(azolms,&st.n_mkt,&st.t_nfix,N-6,lc,fs,th,cg,gr,nf,wd);}
void azol_report(void){ps("[AZOL] Plan: ");pi(st.n_plan);ps(" frond=");pi(st.t_frond);ps("\nSeed: ");pi(st.n_exec);ps(" thick=");pi(st.t_thick);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nHarv: ");pi(st.n_harv);ps(" growth=");pi(st.t_growth);ps("\nMkt: ");pi(st.n_mkt);ps(" nfix=");pi(st.t_nfix);ps("\n");}
void azol_state(void){ps("[AZOL] Plan=");pi(st.n_plan);ps(" Seed=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Azolla (Water Fern) Admin Demo ===\n\n");azol_init();
ps("Azolla planning (pond layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;azol_planning(lc,3+(i*2),2+(i%4),50+(i*5),8+(i*3),20+(i*6),15+(i*4));}
ps("\nAzolla seeding...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;azol_seeding(lc,4+(i*2),3+(i%3),55+(i*4),10+(i*3),22+(i*5),18+(i*3));}
ps("\nAzolla evaluation (coverage check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;azol_evaluation(lc,5+(i*2),3+(i%3),60+(i*3),12+(i*2),24+(i%4),20+(i*3));}
ps("\nAzolla harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;azol_harvest(lc,2+(i*2),2+(i%4),45+(i*5),7+(i*3),18+(i%6),12+(i*4));}
ps("\nAzolla aquatic fern market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;azol_market(lc,6+(i*2),4+(i%2),65+(i*3),14+(i*2),26+(i%4),22+(i%3));}
ps("\n");azol_report();azol_state();ps("\n=== Demo Complete ===\n");return 0;}
