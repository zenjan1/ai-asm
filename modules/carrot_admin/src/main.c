/* carrot_admin: Carrot (Daucus carota) root vegetable cultivation management (v1.0)
 * Carrot planting, thinning, weeding, harvest, market
 * Features: root length cm, root diameter cm, top height cm, sugar brix, seed count, harvest week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,root_ln,root_dia,top_ht,sugar_bx,seed_ct,harv_wk,active;} carr_t;
typedef struct{int n_plant,n_thin,n_weed,n_harv,n_mkt,t_root,t_dia,t_top,t_sugar,t_seed;} carr_state_t;
static carr_t carrps[N],carrts[N-2],carrws[N-4],carrhs[N-6],carrms[N-6]; static carr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(carr_t*a,int*cnt,int*sum,int mx,int lc,int rl,int rd,int th,int sb,int sc,int hw){if(*cnt>=mx)return -1;carr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->root_ln=rl;x->root_dia=rd;x->top_ht=th;x->sugar_bx=sb;x->seed_ct=sc;x->harv_wk=hw;x->active=1;*sum+=rl;(*cnt)++;ps("[CARR] Carrot ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" rl=");pi(rl);ps(" rd=");pi(rd);ps(" th=");pi(th);ps(" sb=");pi(sb);ps(" sc=");pi(sc);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int carr_init(void){if(init)return -1;st.n_plant=0;st.n_thin=0;st.n_weed=0;st.n_harv=0;st.n_mkt=0;st.t_root=0;st.t_dia=0;st.t_top=0;st.t_sugar=0;st.t_seed=0;for(int i=0;i<N;i++)carrps[i].active=0;for(int i=0;i<N-2;i++)carrts[i].active=0;for(int i=0;i<N-4;i++)carrws[i].active=0;for(int i=0;i<N-6;i++)carrhs[i].active=0;for(int i=0;i<N-6;i++)carrms[i].active=0;init=1;ps("[CARR] Carrot initialized\n");return 0;}
int carr_planting(int lc,int rl,int rd,int th,int sb,int sc,int hw){return add(carrps,&st.n_plant,&st.t_root,N,lc,rl,rd,th,sb,sc,hw);}
int carr_thinning(int lc,int rl,int rd,int th,int sb,int sc,int hw){return add(carrts,&st.n_thin,&st.t_dia,N-2,lc,rl,rd,th,sb,sc,hw);}
int carr_weeding(int lc,int rl,int rd,int th,int sb,int sc,int hw){return add(carrws,&st.n_weed,&st.t_top,N-4,lc,rl,rd,th,sb,sc,hw);}
int carr_harvest(int lc,int rl,int rd,int th,int sb,int sc,int hw){return add(carrhs,&st.n_harv,&st.t_sugar,N-6,lc,rl,rd,th,sb,sc,hw);}
int carr_market(int lc,int rl,int rd,int th,int sb,int sc,int hw){return add(carrms,&st.n_mkt,&st.t_seed,N-6,lc,rl,rd,th,sb,sc,hw);}
void carr_report(void){ps("[CARR] Plant: ");pi(st.n_plant);ps(" Root=");pi(st.t_root);ps("\nThin: ");pi(st.n_thin);ps(" Dia=");pi(st.t_dia);ps("\nWeed: ");pi(st.n_weed);ps(" Top=");pi(st.t_top);ps("\nHarv: ");pi(st.n_harv);ps(" Sugar=");pi(st.t_sugar);ps("\nMkt: ");pi(st.n_mkt);ps(" Seed=");pi(st.t_seed);ps("\n");}
void carr_state(void){ps("[CARR] Plant=");pi(st.n_plant);ps(" Thin=");pi(st.n_thin);ps(" Weed=");pi(st.n_weed);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Carrot Admin Demo ===\n\n");carr_init();
/* 1=field 2=garden 3=raised_bed 4=greenhouse 5=organic */
ps("Carrot planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,rl=15+(i*5),rd=2+(i%3),th=20+(i*5),sb=6+(i%2),sc=50+(i*20),hw=12+(i%4);carr_planting(lc,rl,rd,th,sb,sc,hw);}
ps("\nCarrot thinning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,rl=18+(i*4),rd=3+(i%2),th=25+(i*4),sb=7+(i%2),sc=55+(i*15),hw=14+(i%3);carr_thinning(lc,rl,rd,th,sb,sc,hw);}
ps("\nCarrot weeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,rl=20+(i*3),rd=3+(i%2),th=30+(i*3),sb=8+(i%2),sc=60+(i*12),hw=16+(i%2);carr_weeding(lc,rl,rd,th,sb,sc,hw);}
ps("\nCarrot harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,rl=12+(i*6),rd=2+(i%3),th=18+(i*6),sb=5+(i%3),sc=45+(i*25),hw=10+(i%5);carr_harvest(lc,rl,rd,th,sb,sc,hw);}
ps("\nCarrot market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,rl=22+(i*3),rd=4+(i%2),th=35+(i*3),sb=9+(i%2),sc=65+(i*10),hw=18+(i%2);carr_market(lc,rl,rd,th,sb,sc,hw);}
ps("\n");carr_report();carr_state();ps("\n=== Demo Complete ===\n");return 0;}
