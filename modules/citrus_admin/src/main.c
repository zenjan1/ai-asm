/* citrus_admin: Citrus (Citrus sinensis) orange fruit tree management (v1.0)
 * Citrus planting, pruning, pollination, harvest, market
 * Features: tree_height_m, fruit_count, fruit_dia_cm, sugar_brix, acid_pct, harvest_month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,frt_ct,frt_dia,sugar_bx,acid_pct,harv_mo,active;} ctr_t;
typedef struct{int n_plant,n_prune,n_poll,n_harv,n_mkt,t_ht,t_frt,t_dia,t_sugar,t_acid;} ctr_state_t;
static ctr_t ctrps[N],ctrds[N-2],ctrpls[N-4],ctrhs[N-6],ctrms[N-6]; static ctr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ctr_t*a,int*cnt,int*sum,int mx,int lc,int th,int fc,int fd,int sb,int ap,int hm){if(*cnt>=mx)return -1;ctr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->frt_ct=fc;x->frt_dia=fd;x->sugar_bx=sb;x->acid_pct=ap;x->harv_mo=hm;x->active=1;*sum+=th;(*cnt)++;ps("[CTR] Citrus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" fc=");pi(fc);ps(" fd=");pi(fd);ps(" sb=");pi(sb);ps(" ap=");pi(ap);ps(" hm=");pi(hm);ps("\n");return *cnt-1;}
int ctr_init(void){if(init)return -1;st.n_plant=0;st.n_prune=0;st.n_poll=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_frt=0;st.t_dia=0;st.t_sugar=0;st.t_acid=0;for(int i=0;i<N;i++)ctrps[i].active=0;for(int i=0;i<N-2;i++)ctrds[i].active=0;for(int i=0;i<N-4;i++)ctrpls[i].active=0;for(int i=0;i<N-6;i++)ctrhs[i].active=0;for(int i=0;i<N-6;i++)ctrms[i].active=0;init=1;ps("[CTR] Citrus initialized\n");return 0;}
int ctr_planting(int lc,int th,int fc,int fd,int sb,int ap,int hm){return add(ctrps,&st.n_plant,&st.t_ht,N,lc,th,fc,fd,sb,ap,hm);}
int ctr_pruning(int lc,int th,int fc,int fd,int sb,int ap,int hm){return add(ctrds,&st.n_prune,&st.t_frt,N-2,lc,th,fc,fd,sb,ap,hm);}
int ctr_pollination(int lc,int th,int fc,int fd,int sb,int ap,int hm){return add(ctrpls,&st.n_poll,&st.t_dia,N-4,lc,th,fc,fd,sb,ap,hm);}
int ctr_harvest(int lc,int th,int fc,int fd,int sb,int ap,int hm){return add(ctrhs,&st.n_harv,&st.t_sugar,N-6,lc,th,fc,fd,sb,ap,hm);}
int ctr_market(int lc,int th,int fc,int fd,int sb,int ap,int hm){return add(ctrms,&st.n_mkt,&st.t_acid,N-6,lc,th,fc,fd,sb,ap,hm);}
void ctr_report(void){ps("[CTR] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nPrune: ");pi(st.n_prune);ps(" Frt=");pi(st.t_frt);ps("\nPoll: ");pi(st.n_poll);ps(" Dia=");pi(st.t_dia);ps("\nHarv: ");pi(st.n_harv);ps(" Sugar=");pi(st.t_sugar);ps("\nMkt: ");pi(st.n_mkt);ps(" Acid=");pi(st.t_acid);ps("\n");}
void ctr_state(void){ps("[CTR] Plant=");pi(st.n_plant);ps(" Prune=");pi(st.n_prune);ps(" Poll=");pi(st.n_poll);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Citrus Admin Demo ===\n\n");ctr_init();
/* 1=orchard 2=greenhouse 3=garden 4=terrace 5=market */
ps("Citrus planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=2+(i*2),fc=20+(i*10),fd=6+(i%4),sb=10+(i*2),ap=1+(i%5),hm=11+(i%3);ctr_planting(lc,th,fc,fd,sb,ap,hm);}
ps("\nCitrus pruning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=3+(i*2),fc=25+(i*8),fd=7+(i%3),sb=11+(i*2),ap=2+(i%4),hm=12+(i%2);ctr_pruning(lc,th,fc,fd,sb,ap,hm);}
ps("\nCitrus pollination...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=4+(i*2),fc=30+(i*7),fd=8+(i%3),sb=12+(i*2),ap=3+(i%3),hm=1+(i%3);ctr_pollination(lc,th,fc,fd,sb,ap,hm);}
ps("\nCitrus harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=1+(i*3),fc=15+(i*12),fd=5+(i%5),sb=9+(i*3),ap=1+(i%6),hm=10+(i%3);ctr_harvest(lc,th,fc,fd,sb,ap,hm);}
ps("\nCitrus market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=5+(i*2),fc=35+(i*6),fd=9+(i%2),sb=13+(i*2),ap=4+(i%3),hm=2+(i%2);ctr_market(lc,th,fc,fd,sb,ap,hm);}
ps("\n");ctr_report();ctr_state();ps("\n=== Demo Complete ===\n");return 0;}
