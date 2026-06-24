/* cherry_admin: Cherry (Prunus avium) fruit tree orchard management (v1.0)
 * Cherry planting, pruning, pollination, harvest, market
 * Features: tree_height_m, fruit_yield_kg, bloom_week, sugar_brix, pit_diameter_mm, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,frt_yd,bloom_wk,sugar_bx,pit_dia,harv_wk,active;} chry_t;
typedef struct{int n_plant,n_prune,n_poll,n_harv,n_mkt,t_tree,t_frt,t_bloom,t_sugar,t_pit;} chry_state_t;
static chry_t chryps[N],chrypr[N-2],chrypo[N-4],chryhs[N-6],chryms[N-6]; static chry_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chry_t*a,int*cnt,int*sum,int mx,int lc,int th,int fy,int bw,int sb,int pd,int hw){if(*cnt>=mx)return -1;chry_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->frt_yd=fy;x->bloom_wk=bw;x->sugar_bx=sb;x->pit_dia=pd;x->harv_wk=hw;x->active=1;*sum+=th;(*cnt)++;ps("[CHRY] Cherry ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" fy=");pi(fy);ps(" bw=");pi(bw);ps(" sb=");pi(sb);ps(" pd=");pi(pd);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int chry_init(void){if(init)return -1;st.n_plant=0;st.n_prune=0;st.n_poll=0;st.n_harv=0;st.n_mkt=0;st.t_tree=0;st.t_frt=0;st.t_bloom=0;st.t_sugar=0;st.t_pit=0;for(int i=0;i<N;i++)chryps[i].active=0;for(int i=0;i<N-2;i++)chrypr[i].active=0;for(int i=0;i<N-4;i++)chrypo[i].active=0;for(int i=0;i<N-6;i++)chryhs[i].active=0;for(int i=0;i<N-6;i++)chryms[i].active=0;init=1;ps("[CHRY] Cherry initialized\n");return 0;}
int chry_planting(int lc,int th,int fy,int bw,int sb,int pd,int hw){return add(chryps,&st.n_plant,&st.t_tree,N,lc,th,fy,bw,sb,pd,hw);}
int chry_pruning(int lc,int th,int fy,int bw,int sb,int pd,int hw){return add(chrypr,&st.n_prune,&st.t_frt,N-2,lc,th,fy,bw,sb,pd,hw);}
int chry_pollination(int lc,int th,int fy,int bw,int sb,int pd,int hw){return add(chrypo,&st.n_poll,&st.t_bloom,N-4,lc,th,fy,bw,sb,pd,hw);}
int chry_harvest(int lc,int th,int fy,int bw,int sb,int pd,int hw){return add(chryhs,&st.n_harv,&st.t_sugar,N-6,lc,th,fy,bw,sb,pd,hw);}
int chry_market(int lc,int th,int fy,int bw,int sb,int pd,int hw){return add(chryms,&st.n_mkt,&st.t_pit,N-6,lc,th,fy,bw,sb,pd,hw);}
void chry_report(void){ps("[CHRY] Plant: ");pi(st.n_plant);ps(" Tree=");pi(st.t_tree);ps("\nPrune: ");pi(st.n_prune);ps(" Frt=");pi(st.t_frt);ps("\nPoll: ");pi(st.n_poll);ps(" Bloom=");pi(st.t_bloom);ps("\nHarv: ");pi(st.n_harv);ps(" Sugar=");pi(st.t_sugar);ps("\nMkt: ");pi(st.n_mkt);ps(" Pit=");pi(st.t_pit);ps("\n");}
void chry_state(void){ps("[CHRY] Plant=");pi(st.n_plant);ps(" Prune=");pi(st.n_prune);ps(" Poll=");pi(st.n_poll);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cherry Admin Demo ===\n\n");chry_init();
/* 1=orchard 2=home_garden 3=commercial 4=u-pick 5=organic */
ps("Cherry planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=3+(i%5),fy=10+(i*5),bw=12+(i%4),sb=12+(i%3),pit_dia=8+(i%3),hw=20+(i%4);chry_planting(lc,th,fy,bw,sb,pit_dia,hw);}
ps("\nCherry pruning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=4+(i%4),fy=15+(i*4),bw=14+(i%3),sb=14+(i%2),pit_dia=9+(i%2),hw=22+(i%3);chry_pruning(lc,th,fy,bw,sb,pit_dia,hw);}
ps("\nCherry pollination...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=5+(i%3),fy=20+(i*3),bw=16+(i%2),sb=16+(i%2),pit_dia=10+(i%2),hw=24+(i%2);chry_pollination(lc,th,fy,bw,sb,pit_dia,hw);}
ps("\nCherry harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=2+(i%6),fy=8+(i*6),bw=10+(i%5),sb=10+(i%4),pit_dia=7+(i%4),hw=18+(i%5);chry_harvest(lc,th,fy,bw,sb,pit_dia,hw);}
ps("\nCherry market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=6+(i%3),fy=25+(i*3),bw=18+(i%2),sb=18+(i%2),pit_dia=11+(i%2),hw=26+(i%2);chry_market(lc,th,fy,bw,sb,pit_dia,hw);}
ps("\n");chry_report();chry_state();ps("\n=== Demo Complete ===\n");return 0;}
