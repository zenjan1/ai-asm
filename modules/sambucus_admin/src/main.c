/* sambucus_admin: Sambucus (Elderberry) orchard and medicinal production (v1.0)
 * Sambucus planning, execution, evaluation, cane management, medicinal market
 * Features: cultivar selection, berry yield, flower harvest, cane pruning, anthocyanin tracking
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,berry_kg,flower_cl,cane_ct,antho_mg,prune_yr,disease,active;} sam_t;
typedef struct{int n_plan,n_exec,n_eval,n_cane,n_mkt,t_berry,t_flower,t_cane,t_antho,t_prune;} sam_state_t;
static sam_t samps[N],sames[N-2],samvs[N-4],samcs[N-6],samms[N-6]; static sam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sam_t*a,int*cnt,int*sum,int mx,int cv,int bk,int fc,int cc,int am,int py,int ds){if(*cnt>=mx)return -1;sam_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->berry_kg=bk;x->flower_cl=fc;x->cane_ct=cc;x->antho_mg=am;x->prune_yr=py;x->disease=ds;x->active=1;*sum+=bk;(*cnt)++;ps("[SAM] Sambucus ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" bry=");pi(bk);ps(" fl=");pi(fc);ps(" cn=");pi(cc);ps(" an=");pi(am);ps("\n");return *cnt-1;}
int sam_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_cane=0;st.n_mkt=0;st.t_berry=0;st.t_flower=0;st.t_cane=0;st.t_antho=0;st.t_prune=0;for(int i=0;i<N;i++)samps[i].active=0;for(int i=0;i<N-2;i++)sames[i].active=0;for(int i=0;i<N-4;i++)samvs[i].active=0;for(int i=0;i<N-6;i++)samcs[i].active=0;for(int i=0;i<N-6;i++)samms[i].active=0;init=1;ps("[SAM] Sambucus (elderberry) initialized\n");return 0;}
/* 1=nigra(Black) 2=canadensis(American) 3=rubra(Red) 4=cerulea(Blue) 5=racemosa */
int sam_planning(int cv,int bk,int fc,int cc,int am,int py,int ds){return add(samps,&st.n_plan,&st.t_berry,N,cv,bk,fc,cc,am,py,ds);}
int sam_execution(int cv,int bk,int fc,int cc,int am,int py,int ds){return add(sames,&st.n_exec,&st.t_flower,N-2,cv,bk,fc,cc,am,py,ds);}
int sam_evaluation(int cv,int bk,int fc,int cc,int am,int py,int ds){return add(samvs,&st.n_eval,&st.t_cane,N-4,cv,bk,fc,cc,am,py,ds);}
int sam_cane(int cv,int bk,int fc,int cc,int am,int py,int ds){return add(samcs,&st.n_cane,&st.t_antho,N-6,cv,bk,fc,cc,am,py,ds);}
int sam_market(int cv,int bk,int fc,int cc,int am,int py,int ds){return add(samms,&st.n_mkt,&st.t_prune,N-6,cv,bk,fc,cc,am,py,ds);}
void sam_report(void){ps("[SAM] Plan: ");pi(st.n_plan);ps(" bry=");pi(st.t_berry);ps("\nExec: ");pi(st.n_exec);ps(" fl=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" cane=");pi(st.t_cane);ps("\nCane: ");pi(st.n_cane);ps(" antho=");pi(st.t_antho);ps("\nMkt: ");pi(st.n_mkt);ps(" prune=");pi(st.t_prune);ps("\n");}
void sam_state(void){ps("[SAM] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Cane=");pi(st.n_cane);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Sambucus (Elderberry) Admin Demo ===\n\n");sam_init();
ps("Sambucus planning (orchard layout)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;sam_planning(cv,8+(i*3),12+(i%6),15+(i*4),420+(i*35),2020+(i%5),(i%3));}
ps("\nSambucus execution (flower harvest)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;sam_execution(cv,10+(i*2),14+(i%5),18+(i*3),450+(i*30),2021+(i%4),(i%3));}
ps("\nSambucus evaluation (berry quality)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;sam_evaluation(cv,12+(i*3),16+(i%4),20+(i*3),480+(i*25),2022+(i%3),(i%2));}
ps("\nSambucus cane management (pruning)...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;sam_cane(cv,6+(i*2),10+(i%5),12+(i*3),400+(i*30),2023+(i%2),1);}
ps("\nSambucus medicinal market...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;sam_market(cv,14+(i*3),18+(i%4),22+(i*2),500+(i*20),2024,(i%3));}
ps("\n");sam_report();sam_state();ps("\n=== Demo Complete ===\n");return 0;}
