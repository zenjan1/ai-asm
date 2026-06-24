/* cypress_admin: Cypress (Cupressus sempervirens) conifer tree management (v1.0)
 * Cypress planting, pruning, shaping, harvest, market
 * Features: tree_height_m, trunk_dia_cm, cone_count, foliage_idx, growth_rate, prune_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,trk_dia,cn_ct,fol_idx,gr_rate,prn_yr,active;} cypr_t;
typedef struct{int n_plant,n_prune,n_shape,n_harv,n_mkt,t_ht,t_dia,t_cn,t_fol,t_gr;} cypr_state_t;
static cypr_t cyprps[N],cyprds[N-2],cyprsh[N-4],cyprhv[N-6],cyprmk[N-6]; static cypr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cypr_t*a,int*cnt,int*sum,int mx,int lc,int th,int td,int cc,int fi,int gr,int py){if(*cnt>=mx)return -1;cypr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->trk_dia=td;x->cn_ct=cc;x->fol_idx=fi;x->gr_rate=gr;x->prn_yr=py;x->active=1;*sum+=th;(*cnt)++;ps("[CYPR] Cypress ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" td=");pi(td);ps(" cc=");pi(cc);ps(" fi=");pi(fi);ps(" gr=");pi(gr);ps(" py=");pi(py);ps("\n");return *cnt-1;}
int cypr_init(void){if(init)return -1;st.n_plant=0;st.n_prune=0;st.n_shape=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_cn=0;st.t_fol=0;st.t_gr=0;for(int i=0;i<N;i++)cyprps[i].active=0;for(int i=0;i<N-2;i++)cyprds[i].active=0;for(int i=0;i<N-4;i++)cyprsh[i].active=0;for(int i=0;i<N-6;i++)cyprhv[i].active=0;for(int i=0;i<N-6;i++)cyprmk[i].active=0;init=1;ps("[CYPR] Cypress initialized\n");return 0;}
int cypr_planting(int lc,int th,int td,int cc,int fi,int gr,int py){return add(cyprps,&st.n_plant,&st.t_ht,N,lc,th,td,cc,fi,gr,py);}
int cypr_pruning(int lc,int th,int td,int cc,int fi,int gr,int py){return add(cyprds,&st.n_prune,&st.t_dia,N-2,lc,th,td,cc,fi,gr,py);}
int cypr_shaping(int lc,int th,int td,int cc,int fi,int gr,int py){return add(cyprsh,&st.n_shape,&st.t_cn,N-4,lc,th,td,cc,fi,gr,py);}
int cypr_harvest(int lc,int th,int td,int cc,int fi,int gr,int py){return add(cyprhv,&st.n_harv,&st.t_fol,N-6,lc,th,td,cc,fi,gr,py);}
int cypr_market(int lc,int th,int td,int cc,int fi,int gr,int py){return add(cyprmk,&st.n_mkt,&st.t_gr,N-6,lc,th,td,cc,fi,gr,py);}
void cypr_report(void){ps("[CYPR] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nPrune: ");pi(st.n_prune);ps(" Dia=");pi(st.t_dia);ps("\nShape: ");pi(st.n_shape);ps(" Cn=");pi(st.t_cn);ps("\nHarv: ");pi(st.n_harv);ps(" Fol=");pi(st.t_fol);ps("\nMkt: ");pi(st.n_mkt);ps(" Gr=");pi(st.t_gr);ps("\n");}
void cypr_state(void){ps("[CYPR] Plant=");pi(st.n_plant);ps(" Prune=");pi(st.n_prune);ps(" Shape=");pi(st.n_shape);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cypress Admin Demo ===\n\n");cypr_init();
/* 1=garden 2=hedge 3=avenue 4=forest 5=market */
ps("Cypress planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=3+(i*2),td=10+(i*3),cc=20+(i*10),fi=(i%6)+1,gr=1+(i%5),py=1+(i%4);cypr_planting(lc,th,td,cc,fi,gr,py);}
ps("\nCypress pruning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=4+(i*2),td=12+(i*2),cc=22+(i*8),fi=(i%5)+1,gr=1+(i%4),py=2+(i%3);cypr_pruning(lc,th,td,cc,fi,gr,py);}
ps("\nCypress shaping...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=5+(i*2),td=14+(i*2),cc=24+(i*7),fi=(i%4)+1,gr=2+(i%3),py=3+(i%3);cypr_shaping(lc,th,td,cc,fi,gr,py);}
ps("\nCypress harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=2+(i*3),td=8+(i*4),cc=18+(i*12),fi=(i%7)+1,gr=1+(i%6),py=1+(i%5);cypr_harvest(lc,th,td,cc,fi,gr,py);}
ps("\nCypress market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=6+(i*2),td=16+(i*2),cc=26+(i*6),fi=(i%3)+4,gr=2+(i%2),py=4+(i%2);cypr_market(lc,th,td,cc,fi,gr,py);}
ps("\n");cypr_report();cypr_state();ps("\n=== Demo Complete ===\n");return 0;}
