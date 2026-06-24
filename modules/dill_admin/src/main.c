/* dill_admin: Dill (Anethum graveolens) aromatic herb management (v1.0)
 * Dill planting, feeding, flowering, harvest, market
 * Features: plant_ht_cm, stem_dia_mm, leaf_idx, flower_dia_cm, seed_wt_g, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,stm_dia,lf_idx,fl_dia,sd_wt,hv_wk,active;} dill_t;
typedef struct{int n_plant,n_feed,n_flow,n_harv,n_mkt,t_ht,t_dia,t_lf,t_fl,t_sd;} dill_state_t;
static dill_t dpl[N],dfd[N-2],dfl[N-4],dhv[N-6],dmk[N-6]; static dill_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dill_t*a,int*cnt,int*sum,int mx,int lc,int ph,int sd,int li,int fd,int sw,int hw){if(*cnt>=mx)return -1;dill_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->stm_dia=sd;x->lf_idx=li;x->fl_dia=fd;x->sd_wt=sw;x->hv_wk=hw;x->active=1;*sum+=ph;(*cnt)++;ps("[DILL] Dill ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" sd=");pi(sd);ps(" li=");pi(li);ps(" fd=");pi(fd);ps(" sw=");pi(sw);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int dill_init(void){if(init)return -1;st.n_plant=0;st.n_feed=0;st.n_flow=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_lf=0;st.t_fl=0;st.t_sd=0;for(int i=0;i<N;i++)dpl[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dfl[i].active=0;for(int i=0;i<N-6;i++)dhv[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DILL] Dill initialized\n");return 0;}
int dill_planting(int lc,int ph,int sd,int li,int fd,int sw,int hw){return add(dpl,&st.n_plant,&st.t_ht,N,lc,ph,sd,li,fd,sw,hw);}
int dill_feeding(int lc,int ph,int sd,int li,int fd,int sw,int hw){return add(dfd,&st.n_feed,&st.t_dia,N-2,lc,ph,sd,li,fd,sw,hw);}
int dill_flowering(int lc,int ph,int sd,int li,int fd,int sw,int hw){return add(dfl,&st.n_flow,&st.t_lf,N-4,lc,ph,sd,li,fd,sw,hw);}
int dill_harvest(int lc,int ph,int sd,int li,int fd,int sw,int hw){return add(dhv,&st.n_harv,&st.t_fl,N-6,lc,ph,sd,li,fd,sw,hw);}
int dill_market(int lc,int ph,int sd,int li,int fd,int sw,int hw){return add(dmk,&st.n_mkt,&st.t_sd,N-6,lc,ph,sd,li,fd,sw,hw);}
void dill_report(void){ps("[DILL] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nFeed: ");pi(st.n_feed);ps(" Dia=");pi(st.t_dia);ps("\nFlow: ");pi(st.n_flow);ps(" Lf=");pi(st.t_lf);ps("\nHarv: ");pi(st.n_harv);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sd=");pi(st.t_sd);ps("\n");}
void dill_state(void){ps("[DILL] Plant=");pi(st.n_plant);ps(" Feed=");pi(st.n_feed);ps(" Flow=");pi(st.n_flow);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dill Admin Demo ===\n\n");dill_init();
/* 1=garden 2=field 3=greenhouse 4=border 5=market */
ps("Dill planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=40+(i*4),sd=2+(i%2),li=(i%6)+1,fd=1+(i%2),sw=3+(i*2),hw=(i%12)+1;dill_planting(lc,ph,sd,li,fd,sw,hw);}
ps("\nDill feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=45+(i*3),sd=3+(i%2),li=(i%5)+1,fd=2+(i%2),sw=4+(i*2),hw=(i%10)+1;dill_feeding(lc,ph,sd,li,fd,sw,hw);}
ps("\nDill flowering...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=50+(i*3),sd=3+(i%3),li=(i%4)+1,fd=2+(i%3),sw=5+(i*2),hw=(i%8)+1;dill_flowering(lc,ph,sd,li,fd,sw,hw);}
ps("\nDill harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=35+(i*5),sd=2+(i%3),li=(i%7)+1,fd=1+(i%4),sw=2+(i*3),hw=(i%11)+1;dill_harvest(lc,ph,sd,li,fd,sw,hw);}
ps("\nDill market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=55+(i*3),sd=4+(i%2),li=(i%3)+4,fd=3+(i%2),sw=7+(i*2),hw=(i%6)+4;dill_market(lc,ph,sd,li,fd,sw,hw);}
ps("\n");dill_report();dill_state();ps("\n=== Demo Complete ===\n");return 0;}
