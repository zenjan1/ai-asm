/* tilia_admin: Tilia (Linden/Basswood) ornamental and honey production tree (v1.0)
 * Tilia planning, execution, evaluation, beekeeping support, timber market
 * Features: tree height, flower density for bees, canopy spread, bark quality, urban planting
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,species,ht_m,flower_d,canopy_m,bark_gr,urban_idx,age_yr,active;} til_t;
typedef struct{int n_plan,n_exec,n_eval,n_bee,n_mkt,t_ht,t_flower,t_canopy,t_bark,t_urban;} til_state_t;
static til_t tilps[N],tiles[N-2],tilvs[N-4],tilbs[N-6],tilms[N-6]; static til_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(til_t*a,int*cnt,int*sum,int mx,int sp,int ht,int fd,int cn,int bk,int ui,int ay){if(*cnt>=mx)return -1;til_t*x=&a[*cnt];x->id=*cnt;x->species=sp;x->ht_m=ht;x->flower_d=fd;x->canopy_m=cn;x->bark_gr=bk;x->urban_idx=ui;x->age_yr=ay;x->active=1;*sum+=ht;(*cnt)++;ps("[TIL] Tilia ");pi(*cnt-1);ps(" sp=");pi(sp);ps(" ht=");pi(ht);ps(" fl=");pi(fd);ps(" cn=");pi(cn);ps(" bk=");pi(bk);ps("\n");return *cnt-1;}
int til_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_bee=0;st.n_mkt=0;st.t_ht=0;st.t_flower=0;st.t_canopy=0;st.t_bark=0;st.t_urban=0;for(int i=0;i<N;i++)tilps[i].active=0;for(int i=0;i<N-2;i++)tiles[i].active=0;for(int i=0;i<N-4;i++)tilvs[i].active=0;for(int i=0;i<N-6;i++)tilbs[i].active=0;for(int i=0;i<N-6;i++)tilms[i].active=0;init=1;ps("[TIL] Tilia (linden tree) initialized\n");return 0;}
/* 1=cordata 2=vulgaris 3=platyphyllos 4=tomentosa 5=americana */
int til_planning(int sp,int ht,int fd,int cn,int bk,int ui,int ay){return add(tilps,&st.n_plan,&st.t_ht,N,sp,ht,fd,cn,bk,ui,ay);}
int til_execution(int sp,int ht,int fd,int cn,int bk,int ui,int ay){return add(tiles,&st.n_exec,&st.t_flower,N-2,sp,ht,fd,cn,bk,ui,ay);}
int til_evaluation(int sp,int ht,int fd,int cn,int bk,int ui,int ay){return add(tilvs,&st.n_eval,&st.t_canopy,N-4,sp,ht,fd,cn,bk,ui,ay);}
int til_beekeeping(int sp,int ht,int fd,int cn,int bk,int ui,int ay){return add(tilbs,&st.n_bee,&st.t_bark,N-6,sp,ht,fd,cn,bk,ui,ay);}
int til_market(int sp,int ht,int fd,int cn,int bk,int ui,int ay){return add(tilms,&st.n_mkt,&st.t_urban,N-6,sp,ht,fd,cn,bk,ui,ay);}
void til_report(void){ps("[TIL] Plan: ");pi(st.n_plan);ps(" ht=");pi(st.t_ht);ps("\nExec: ");pi(st.n_exec);ps(" fl=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" cn=");pi(st.t_canopy);ps("\nBee: ");pi(st.n_bee);ps(" bark=");pi(st.t_bark);ps("\nMkt: ");pi(st.n_mkt);ps(" urban=");pi(st.t_urban);ps("\n");}
void til_state(void){ps("[TIL] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Bee=");pi(st.n_bee);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tilia (Linden Tree) Admin Demo ===\n\n");til_init();
ps("Tilia planning (avenue layout)...\n");
for(int i=0;i<N;i++){int sp=(i%5)+1;til_planning(sp,12+(i*3),25+(i*8),8+(i*2),(i%4)+1,(i%5)+1,5+(i*3));}
ps("\nTilia execution (planting)...\n");
for(int i=0;i<N-2;i++){int sp=(i%4)+2;til_execution(sp,14+(i*2),28+(i*7),9+(i*2),(i%3)+2,(i%4)+1,6+(i*2));}
ps("\nTilia evaluation (bloom assessment)...\n");
for(int i=0;i<N-4;i++){int sp=(i%3)+1;til_evaluation(sp,16+(i*2),30+(i*6),10+(i*1),(i%3)+2,(i%3)+2,7+(i*2));}
ps("\nTilia beekeeping support...\n");
for(int i=0;i<N-6;i++){int sp=(i%5)+1;til_beekeeping(sp,10+(i*3),22+(i*7),7+(i*2),(i%4)+1,(i%5)+1,4+(i*3));}
ps("\nTilia timber market...\n");
for(int i=0;i<N-6;i++){int sp=(i%4)+1;til_market(sp,18+(i*2),32+(i*5),11+(i*1),(i%3)+2,(i%3)+2,8+(i*2));}
ps("\n");til_report();til_state();ps("\n=== Demo Complete ===\n");return 0;}
