/* cocoa_admin: Cocoa (Theobroma cacao) cacao tree management (v1.0)
 * Cocoa planting, fermentation, drying, harvest, market
 * Features: tree_height_m, pod_count, bean_wt_g, butter_pct, ferment_days, harvest_month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,pod_ct,bean_wt,butter_pct,ferm_dys,harv_mo,active;} ccoa_t;
typedef struct{int n_plant,n_ferm,n_dry,n_harv,n_mkt,t_ht,t_pod,t_bean,t_butter,t_ferm;} ccoa_state_t;
static ccoa_t ccops[N],ccofs[N-2],ccods[N-4],ccohs[N-6],ccoms[N-6]; static ccoa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ccoa_t*a,int*cnt,int*sum,int mx,int lc,int th,int pc,int bw,int bp,int fd,int hm){if(*cnt>=mx)return -1;ccoa_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->pod_ct=pc;x->bean_wt=bw;x->butter_pct=bp;x->ferm_dys=fd;x->harv_mo=hm;x->active=1;*sum+=th;(*cnt)++;ps("[CCOA] Cocoa ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" pc=");pi(pc);ps(" bw=");pi(bw);ps(" bp=");pi(bp);ps(" fd=");pi(fd);ps(" hm=");pi(hm);ps("\n");return *cnt-1;}
int ccoa_init(void){if(init)return -1;st.n_plant=0;st.n_ferm=0;st.n_dry=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_pod=0;st.t_bean=0;st.t_butter=0;st.t_ferm=0;for(int i=0;i<N;i++)ccops[i].active=0;for(int i=0;i<N-2;i++)ccofs[i].active=0;for(int i=0;i<N-4;i++)ccods[i].active=0;for(int i=0;i<N-6;i++)ccohs[i].active=0;for(int i=0;i<N-6;i++)ccoms[i].active=0;init=1;ps("[CCOA] Cocoa initialized\n");return 0;}
int ccoa_planting(int lc,int th,int pc,int bw,int bp,int fd,int hm){return add(ccops,&st.n_plant,&st.t_ht,N,lc,th,pc,bw,bp,fd,hm);}
int ccoa_fermentation(int lc,int th,int pc,int bw,int bp,int fd,int hm){return add(ccofs,&st.n_ferm,&st.t_pod,N-2,lc,th,pc,bw,bp,fd,hm);}
int ccoa_drying(int lc,int th,int pc,int bw,int bp,int fd,int hm){return add(ccods,&st.n_dry,&st.t_bean,N-4,lc,th,pc,bw,bp,fd,hm);}
int ccoa_harvest(int lc,int th,int pc,int bw,int bp,int fd,int hm){return add(ccohs,&st.n_harv,&st.t_butter,N-6,lc,th,pc,bw,bp,fd,hm);}
int ccoa_market(int lc,int th,int pc,int bw,int bp,int fd,int hm){return add(ccoms,&st.n_mkt,&st.t_ferm,N-6,lc,th,pc,bw,bp,fd,hm);}
void ccoa_report(void){ps("[CCOA] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nFerm: ");pi(st.n_ferm);ps(" Pod=");pi(st.t_pod);ps("\nDry: ");pi(st.n_dry);ps(" Bean=");pi(st.t_bean);ps("\nHarv: ");pi(st.n_harv);ps(" Butter=");pi(st.t_butter);ps("\nMkt: ");pi(st.n_mkt);ps(" Ferm=");pi(st.t_ferm);ps("\n");}
void ccoa_state(void){ps("[CCOA] Plant=");pi(st.n_plant);ps(" Ferm=");pi(st.n_ferm);ps(" Dry=");pi(st.n_dry);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cocoa Admin Demo ===\n\n");ccoa_init();
/* 1=plantation 2=orchard 3=greenhouse 4=terrace 5=market */
ps("Cocoa planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=4+(i*2),pc=20+(i*10),bw=1+(i%3),bp=40+(i*3),fd=5+(i%4),hm=10+(i%4);ccoa_planting(lc,th,pc,bw,bp,fd,hm);}
ps("\nCocoa fermentation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=5+(i*2),pc=25+(i*8),bw=1+(i%2),bp=42+(i*3),fd=6+(i%3),hm=11+(i%3);ccoa_fermentation(lc,th,pc,bw,bp,fd,hm);}
ps("\nCocoa drying...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=6+(i*2),pc=30+(i*7),bw=2+(i%2),bp=44+(i*2),fd=7+(i%3),hm=12+(i%2);ccoa_drying(lc,th,pc,bw,bp,fd,hm);}
ps("\nCocoa harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=3+(i*3),pc=15+(i*12),bw=1+(i%4),bp=38+(i*4),fd=4+(i%5),hm=9+(i%5);ccoa_harvest(lc,th,pc,bw,bp,fd,hm);}
ps("\nCocoa market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=7+(i*2),pc=35+(i*6),bw=2+(i%2),bp=46+(i*2),fd=8+(i%2),hm=1+(i%3);ccoa_market(lc,th,pc,bw,bp,fd,hm);}
ps("\n");ccoa_report();ccoa_state();ps("\n=== Demo Complete ===\n");return 0;}
