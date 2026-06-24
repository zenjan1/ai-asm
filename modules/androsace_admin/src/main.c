/* androsace_admin: Androsace (Rock Jasmine) alpine cushion plant management (v1.0)
 * Androsace planning, planting, evaluation, alpine trial, market
 * Features: rosette diameter, flower count, petal color, leaf rosette density, altitude range, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,rosette_dia,flower_ct,petal_color,leaf_ros,alt_range,bloom_wk,active;} adr_t;
typedef struct{int n_plan,n_exec,n_eval,n_alp,n_mkt,t_rosette,t_flower,t_petal,t_leaf,t_alt;} adr_state_t;
static adr_t adrps[N],adres[N-2],adrvs[N-4],adral[N-6],adrms[N-6]; static adr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(adr_t*a,int*cnt,int*sum,int mx,int lc,int rd,int fc,int pc,int lr,int ar,int bw){if(*cnt>=mx)return -1;adr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->rosette_dia=rd;x->flower_ct=fc;x->petal_color=pc;x->leaf_ros=lr;x->alt_range=ar;x->bloom_wk=bw;x->active=1;*sum+=rd;(*cnt)++;ps("[ADR] Androsace ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" rd=");pi(rd);ps(" fc=");pi(fc);ps(" pc=");pi(pc);ps(" lr=");pi(lr);ps(" ar=");pi(ar);ps("\n");return *cnt-1;}
int adr_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_alp=0;st.n_mkt=0;st.t_rosette=0;st.t_flower=0;st.t_petal=0;st.t_leaf=0;st.t_alt=0;for(int i=0;i<N;i++)adrps[i].active=0;for(int i=0;i<N-2;i++)adres[i].active=0;for(int i=0;i<N-4;i++)adrvs[i].active=0;for(int i=0;i<N-6;i++)adral[i].active=0;for(int i=0;i<N-6;i++)adrms[i].active=0;init=1;ps("[ADR] Androsace initialized\n");return 0;}
/* 1=alpine 2=rock_garden 3=crevice 4=trough 5=container */
int adr_planning(int lc,int rd,int fc,int pc,int lr,int ar,int bw){return add(adrps,&st.n_plan,&st.t_rosette,N,lc,rd,fc,pc,lr,ar,bw);}
int adr_execution(int lc,int rd,int fc,int pc,int lr,int ar,int bw){return add(adres,&st.n_exec,&st.t_flower,N-2,lc,rd,fc,pc,lr,ar,bw);}
int adr_evaluation(int lc,int rd,int fc,int pc,int lr,int ar,int bw){return add(adrvs,&st.n_eval,&st.t_petal,N-4,lc,rd,fc,pc,lr,ar,bw);}
int adr_alpine_trial(int lc,int rd,int fc,int pc,int lr,int ar,int bw){return add(adral,&st.n_alp,&st.t_leaf,N-6,lc,rd,fc,pc,lr,ar,bw);}
int adr_market(int lc,int rd,int fc,int pc,int lr,int ar,int bw){return add(adrms,&st.n_mkt,&st.t_alt,N-6,lc,rd,fc,pc,lr,ar,bw);}
void adr_report(void){ps("[ADR] Plan: ");pi(st.n_plan);ps(" rosette=");pi(st.t_rosette);ps("\nExec: ");pi(st.n_exec);ps(" flower=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" petal=");pi(st.t_petal);ps("\nAlp: ");pi(st.n_alp);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" alt=");pi(st.t_alt);ps("\n");}
void adr_state(void){ps("[ADR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Alp=");pi(st.n_alp);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Androsace (Rock Jasmine) Admin Demo ===\n\n");adr_init();
ps("Androsace planning (alpine layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;adr_planning(lc,5+(i*2),8+(i%6),(i%7)+1,4+(i%3),2000+(i*200),16+(i%8));}
ps("\nAndrosace execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;adr_execution(lc,6+(i*2),10+(i%5),(i%7)+1,5+(i%3),2200+(i*150),18+(i%6));}
ps("\nAndrosace evaluation (cushion check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;adr_evaluation(lc,7+(i*2),12+(i%4),(i%6)+2,6+(i%2),2500+(i*100),20+(i%5));}
ps("\nAndrosace alpine trial...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;adr_alpine_trial(lc,4+(i*2),6+(i%4),(i%5)+1,3+(i%3),1800+(i*200),14+(i%7));}
ps("\nAndrosace alpine plant market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;adr_market(lc,8+(i*2),14+(i%3),(i%6)+2,7+(i%2),2800+(i*100),22+(i%4));}
ps("\n");adr_report();adr_state();ps("\n=== Demo Complete ===\n");return 0;}
