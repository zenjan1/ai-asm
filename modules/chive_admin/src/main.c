/* chive_admin: Chive (Allium schoenoprasum) herb cluster management (v1.0)
 * Chive planting, dividing, flowering, harvest, market
 * Features: stem_ht_cm, stem_dia_mm, leaf_count, bulb_size_mm, flavor_idx, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,stm_ht,stm_dia,lf_ct,blb_sz,flv_idx,harv_wk,active;} chv_t;
typedef struct{int n_plant,n_div,n_flow,n_harv,n_mkt,t_ht,t_dia,t_lf,t_blb,t_flv;} chv_state_t;
static chv_t chvps[N],chvds[N-2],chvfs[N-4],chvhs[N-6],chvms[N-6]; static chv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chv_t*a,int*cnt,int*sum,int mx,int lc,int sh,int sd,int lc2,int bs,int fi,int hw){if(*cnt>=mx)return -1;chv_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->stm_ht=sh;x->stm_dia=sd;x->lf_ct=lc2;x->blb_sz=bs;x->flv_idx=fi;x->harv_wk=hw;x->active=1;*sum+=sh;(*cnt)++;ps("[CHV] Chive ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" sd=");pi(sd);ps(" lc2=");pi(lc2);ps(" bs=");pi(bs);ps(" fi=");pi(fi);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int chv_init(void){if(init)return -1;st.n_plant=0;st.n_div=0;st.n_flow=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_lf=0;st.t_blb=0;st.t_flv=0;for(int i=0;i<N;i++)chvps[i].active=0;for(int i=0;i<N-2;i++)chvds[i].active=0;for(int i=0;i<N-4;i++)chvfs[i].active=0;for(int i=0;i<N-6;i++)chvhs[i].active=0;for(int i=0;i<N-6;i++)chvms[i].active=0;init=1;ps("[CHV] Chive initialized\n");return 0;}
int chv_planting(int lc,int sh,int sd,int lc2,int bs,int fi,int hw){return add(chvps,&st.n_plant,&st.t_ht,N,lc,sh,sd,lc2,bs,fi,hw);}
int chv_dividing(int lc,int sh,int sd,int lc2,int bs,int fi,int hw){return add(chvds,&st.n_div,&st.t_dia,N-2,lc,sh,sd,lc2,bs,fi,hw);}
int chv_flowering(int lc,int sh,int sd,int lc2,int bs,int fi,int hw){return add(chvfs,&st.n_flow,&st.t_lf,N-4,lc,sh,sd,lc2,bs,fi,hw);}
int chv_harvest(int lc,int sh,int sd,int lc2,int bs,int fi,int hw){return add(chvhs,&st.n_harv,&st.t_blb,N-6,lc,sh,sd,lc2,bs,fi,hw);}
int chv_market(int lc,int sh,int sd,int lc2,int bs,int fi,int hw){return add(chvms,&st.n_mkt,&st.t_flv,N-6,lc,sh,sd,lc2,bs,fi,hw);}
void chv_report(void){ps("[CHV] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nDiv: ");pi(st.n_div);ps(" Dia=");pi(st.t_dia);ps("\nFlow: ");pi(st.n_flow);ps(" Lf=");pi(st.t_lf);ps("\nHarv: ");pi(st.n_harv);ps(" Blb=");pi(st.t_blb);ps("\nMkt: ");pi(st.n_mkt);ps(" Flv=");pi(st.t_flv);ps("\n");}
void chv_state(void){ps("[CHV] Plant=");pi(st.n_plant);ps(" Div=");pi(st.n_div);ps(" Flow=");pi(st.n_flow);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chive Admin Demo ===\n\n");chv_init();
/* 1=garden 2=raised_bed 3=container 4=herb spiral 5=market */
ps("Chive planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sh=20+(i*4),sd=2+(i%3),lc2=5+(i*2),bs=8+(i%5),fi=(i%8)+1,hw=8+(i%6);chv_planting(lc,sh,sd,lc2,bs,fi,hw);}
ps("\nChive dividing...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sh=25+(i*3),sd=3+(i%3),lc2=6+(i*2),bs=10+(i%4),fi=(i%7)+1,hw=9+(i%5);chv_dividing(lc,sh,sd,lc2,bs,fi,hw);}
ps("\nChive flowering...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sh=30+(i*3),sd=4+(i%2),lc2=7+(i*2),bs=12+(i%3),fi=(i%6)+1,hw=10+(i%4);chv_flowering(lc,sh,sd,lc2,bs,fi,hw);}
ps("\nChive harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sh=18+(i*5),sd=2+(i%4),lc2=4+(i*3),bs=6+(i%6),fi=(i%9)+1,hw=7+(i%7);chv_harvest(lc,sh,sd,lc2,bs,fi,hw);}
ps("\nChive market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sh=35+(i*3),sd=5+(i%2),lc2=8+(i*2),bs=14+(i%3),fi=(i%5)+4,hw=12+(i%3);chv_market(lc,sh,sd,lc2,bs,fi,hw);}
ps("\n");chv_report();chv_state();ps("\n=== Demo Complete ===\n");return 0;}
