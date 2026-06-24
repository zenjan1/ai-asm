/* bulrush_admin: Bulrush (Schoenoplectus) wetland plant management (v1.0)
 * Bulrush propagation, habitat, weaving, conservation, market
 * Features: stem height, density, water depth, flower head, root spread, harvest week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,stm_ht,dens_pct,wtr_dp,flr_hd,root_sp,harv_wk,active;} bulr_t;
typedef struct{int n_prop,n_hab,n_weave,n_cons,n_mkt,t_stm,t_dens,t_wtr,t_flr,t_root;} bulr_state_t;
static bulr_t bulrps[N],bulrhs[N-2],bulrws[N-4],bulrcs[N-6],bulrms[N-6]; static bulr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bulr_t*a,int*cnt,int*sum,int mx,int lc,int sh,int dp,int wd,int fh,int rs,int hw){if(*cnt>=mx)return -1;bulr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->stm_ht=sh;x->dens_pct=dp;x->wtr_dp=wd;x->flr_hd=fh;x->root_sp=rs;x->harv_wk=hw;x->active=1;*sum+=sh;(*cnt)++;ps("[BULR] Bulrush ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" dp=");pi(dp);ps(" wd=");pi(wd);ps(" fh=");pi(fh);ps(" rs=");pi(rs);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int bulr_init(void){if(init)return -1;st.n_prop=0;st.n_hab=0;st.n_weave=0;st.n_cons=0;st.n_mkt=0;st.t_stm=0;st.t_dens=0;st.t_wtr=0;st.t_flr=0;st.t_root=0;for(int i=0;i<N;i++)bulrps[i].active=0;for(int i=0;i<N-2;i++)bulrhs[i].active=0;for(int i=0;i<N-4;i++)bulrws[i].active=0;for(int i=0;i<N-6;i++)bulrcs[i].active=0;for(int i=0;i<N-6;i++)bulrms[i].active=0;init=1;ps("[BULR] Bulrush initialized\n");return 0;}
int bulr_propagation(int lc,int sh,int dp,int wd,int fh,int rs,int hw){return add(bulrps,&st.n_prop,&st.t_stm,N,lc,sh,dp,wd,fh,rs,hw);}
int bulr_habitat(int lc,int sh,int dp,int wd,int fh,int rs,int hw){return add(bulrhs,&st.n_hab,&st.t_dens,N-2,lc,sh,dp,wd,fh,rs,hw);}
int bulr_weaving(int lc,int sh,int dp,int wd,int fh,int rs,int hw){return add(bulrws,&st.n_weave,&st.t_wtr,N-4,lc,sh,dp,wd,fh,rs,hw);}
int bulr_conservation(int lc,int sh,int dp,int wd,int fh,int rs,int hw){return add(bulrcs,&st.n_cons,&st.t_flr,N-6,lc,sh,dp,wd,fh,rs,hw);}
int bulr_market(int lc,int sh,int dp,int wd,int fh,int rs,int hw){return add(bulrms,&st.n_mkt,&st.t_root,N-6,lc,sh,dp,wd,fh,rs,hw);}
void bulr_report(void){ps("[BULR] Prop: ");pi(st.n_prop);ps(" Stm=");pi(st.t_stm);ps("\nHab: ");pi(st.n_hab);ps(" Dens=");pi(st.t_dens);ps("\nWeave: ");pi(st.n_weave);ps(" Wtr=");pi(st.t_wtr);ps("\nCons: ");pi(st.n_cons);ps(" Flr=");pi(st.t_flr);ps("\nMkt: ");pi(st.n_mkt);ps(" Root=");pi(st.t_root);ps("\n");}
void bulr_state(void){ps("[BULR] Prop=");pi(st.n_prop);ps(" Hab=");pi(st.n_hab);ps(" Weave=");pi(st.n_weave);ps(" Cons=");pi(st.n_cons);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bulrush Admin Demo ===\n\n");bulr_init();
/* 1=marsh 2=bog 3=pond_edge 4=stream_bank 5=lake_shore */
ps("Bulrush propagation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,stm_ht=80+(i*15),dens_pct=30+(i*4),wtr_dp=10+(i*5),flr_hd=3+(i%5),root_sp=15+(i*3),harv_wk=12+(i%8);bulr_propagation(lc,stm_ht,dens_pct,wtr_dp,flr_hd,root_sp,harv_wk);}
ps("\nBulrush habitat...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,stm_ht=100+(i*12),dens_pct=35+(i*3),wtr_dp=15+(i*4),flr_hd=4+(i%4),root_sp=20+(i*2),harv_wk=14+(i%6);bulr_habitat(lc,stm_ht,dens_pct,wtr_dp,flr_hd,root_sp,harv_wk);}
ps("\nBulrush weaving...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,stm_ht=120+(i*10),dens_pct=40+(i*3),wtr_dp=20+(i*3),flr_hd=5+(i%3),root_sp=25+(i*2),harv_wk=16+(i%4);bulr_weaving(lc,stm_ht,dens_pct,wtr_dp,flr_hd,root_sp,harv_wk);}
ps("\nBulrush conservation...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,stm_ht=70+(i*18),dens_pct=25+(i*5),wtr_dp=8+(i*6),flr_hd=2+(i%5),root_sp=12+(i*4),harv_wk=10+(i%7);bulr_conservation(lc,stm_ht,dens_pct,wtr_dp,flr_hd,root_sp,harv_wk);}
ps("\nBulrush market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,stm_ht=140+(i*8),dens_pct=45+(i*3),wtr_dp=25+(i*2),flr_hd=6+(i%2),root_sp=30+(i*2),harv_wk=18+(i%3);bulr_market(lc,stm_ht,dens_pct,wtr_dp,flr_hd,root_sp,harv_wk);}
ps("\n");bulr_report();bulr_state();ps("\n=== Demo Complete ===\n");return 0;}
