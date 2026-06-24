/* buttercup_admin: Buttercup (Ranunculus) meadow flower management (v1.0)
 * Buttercup meadow, propagation, pest control, display, market
 * Features: petal count, bloom diameter, stem height, tuber count, seed count, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,petal_ct,bloom_dia,stm_ht,tuber_ct,seed_ct,bloom_wk,active;} butc_t;
typedef struct{int n_mead,n_prop,n_pest,n_disp,n_mkt,t_petal,t_bloom,t_stm,t_tuber,t_seed;} butc_state_t;
static butc_t butcps[N],butcpr[N-2],butcpe[N-4],butcds[N-6],butcms[N-6]; static butc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(butc_t*a,int*cnt,int*sum,int mx,int lc,int pc,int bd,int sh,int tc,int sc,int bw){if(*cnt>=mx)return -1;butc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->petal_ct=pc;x->bloom_dia=bd;x->stm_ht=sh;x->tuber_ct=tc;x->seed_ct=sc;x->bloom_wk=bw;x->active=1;*sum+=pc;(*cnt)++;ps("[BUTC] Buttercup ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" pc=");pi(pc);ps(" bd=");pi(bd);ps(" sh=");pi(sh);ps(" tc=");pi(tc);ps(" sc=");pi(sc);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int butc_init(void){if(init)return -1;st.n_mead=0;st.n_prop=0;st.n_pest=0;st.n_disp=0;st.n_mkt=0;st.t_petal=0;st.t_bloom=0;st.t_stm=0;st.t_tuber=0;st.t_seed=0;for(int i=0;i<N;i++)butcps[i].active=0;for(int i=0;i<N-2;i++)butcpr[i].active=0;for(int i=0;i<N-4;i++)butcpe[i].active=0;for(int i=0;i<N-6;i++)butcds[i].active=0;for(int i=0;i<N-6;i++)butcms[i].active=0;init=1;ps("[BUTC] Buttercup initialized\n");return 0;}
int butc_meadow(int lc,int pc,int bd,int sh,int tc,int sc,int bw){return add(butcps,&st.n_mead,&st.t_petal,N,lc,pc,bd,sh,tc,sc,bw);}
int butc_propagation(int lc,int pc,int bd,int sh,int tc,int sc,int bw){return add(butcpr,&st.n_prop,&st.t_bloom,N-2,lc,pc,bd,sh,tc,sc,bw);}
int butc_pest_control(int lc,int pc,int bd,int sh,int tc,int sc,int bw){return add(butcpe,&st.n_pest,&st.t_stm,N-4,lc,pc,bd,sh,tc,sc,bw);}
int butc_display(int lc,int pc,int bd,int sh,int tc,int sc,int bw){return add(butcds,&st.n_disp,&st.t_tuber,N-6,lc,pc,bd,sh,tc,sc,bw);}
int butc_market(int lc,int pc,int bd,int sh,int tc,int sc,int bw){return add(butcms,&st.n_mkt,&st.t_seed,N-6,lc,pc,bd,sh,tc,sc,bw);}
void butc_report(void){ps("[BUTC] Meadow: ");pi(st.n_mead);ps(" Petal=");pi(st.t_petal);ps("\nProp: ");pi(st.n_prop);ps(" Bloom=");pi(st.t_bloom);ps("\nPest: ");pi(st.n_pest);ps(" Stem=");pi(st.t_stm);ps("\nDisplay: ");pi(st.n_disp);ps(" Tuber=");pi(st.t_tuber);ps("\nMkt: ");pi(st.n_mkt);ps(" Seed=");pi(st.t_seed);ps("\n");}
void butc_state(void){ps("[BUTC] Meadow=");pi(st.n_mead);ps(" Prop=");pi(st.n_prop);ps(" Pest=");pi(st.n_pest);ps(" Display=");pi(st.n_disp);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Buttercup Admin Demo ===\n\n");butc_init();
/* 1=meadow 2=pasture 3=lawn 4=roadside 5=woodland_clearing */
ps("Buttercup meadow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,pc=5+(i%8),bd=2+(i%2),sh=15+(i*5),tc=3+(i%5),sc=20+(i*10),bw=16+(i%8);butc_meadow(lc,pc,bd,sh,tc,sc,bw);}
ps("\nButtercup propagation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,pc=6+(i%6),bd=3+(i%2),sh=18+(i*4),tc=4+(i%4),sc=25+(i*8),bw=18+(i%6);butc_propagation(lc,pc,bd,sh,tc,sc,bw);}
ps("\nButtercup pest control...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,pc=7+(i%5),bd=3+(i%2),sh=20+(i*3),tc=5+(i%3),sc=30+(i*6),bw=20+(i%4);butc_pest_control(lc,pc,bd,sh,tc,sc,bw);}
ps("\nButtercup display...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,pc=8+(i%4),bd=4+(i%2),sh=22+(i*3),tc=2+(i%5),sc=15+(i*12),bw=14+(i%7);butc_display(lc,pc,bd,sh,tc,sc,bw);}
ps("\nButtercup market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,pc=5+(i%7),bd=2+(i%3),sh=12+(i*6),tc=6+(i%3),sc=35+(i*5),bw=22+(i%3);butc_market(lc,pc,bd,sh,tc,sc,bw);}
ps("\n");butc_report();butc_state();ps("\n=== Demo Complete ===\n");return 0;}
