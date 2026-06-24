/* clover_admin: Clover (Trifolium repens) legume cover crop management (v1.0)
 * Clover planting, inoculation, mowing, harvest, market
 * Features: plant_ht_cm, leaf_span_cm, stem_count, root_depth_cm, nitrogen_ppm, bloom_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,lf_sp,stm_ct,root_dp,nitro_ppm,blm_wk,active;} clvr_t;
typedef struct{int n_plant,n_inoc,n_mow,n_harv,n_mkt,t_ht,t_sp,t_stm,t_root,t_nitro;} clvr_state_t;
static clvr_t clvrps[N],clvrs[N-2],clvrd[N-4],clvrh[N-6],clvrm[N-6]; static clvr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(clvr_t*a,int*cnt,int*sum,int mx,int lc,int ph,int ls,int sc,int rd,int np,int bw){if(*cnt>=mx)return -1;clvr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->lf_sp=ls;x->stm_ct=sc;x->root_dp=rd;x->nitro_ppm=np;x->blm_wk=bw;x->active=1;*sum+=ph;(*cnt)++;ps("[CLVR] Clover ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" ls=");pi(ls);ps(" sc=");pi(sc);ps(" rd=");pi(rd);ps(" np=");pi(np);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int clvr_init(void){if(init)return -1;st.n_plant=0;st.n_inoc=0;st.n_mow=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_sp=0;st.t_stm=0;st.t_root=0;st.t_nitro=0;for(int i=0;i<N;i++)clvrps[i].active=0;for(int i=0;i<N-2;i++)clvrs[i].active=0;for(int i=0;i<N-4;i++)clvrd[i].active=0;for(int i=0;i<N-6;i++)clvrh[i].active=0;for(int i=0;i<N-6;i++)clvrm[i].active=0;init=1;ps("[CLVR] Clover initialized\n");return 0;}
int clvr_planting(int lc,int ph,int ls,int sc,int rd,int np,int bw){return add(clvrps,&st.n_plant,&st.t_ht,N,lc,ph,ls,sc,rd,np,bw);}
int clvr_inoculation(int lc,int ph,int ls,int sc,int rd,int np,int bw){return add(clvrs,&st.n_inoc,&st.t_sp,N-2,lc,ph,ls,sc,rd,np,bw);}
int clvr_mowing(int lc,int ph,int ls,int sc,int rd,int np,int bw){return add(clvrd,&st.n_mow,&st.t_stm,N-4,lc,ph,ls,sc,rd,np,bw);}
int clvr_harvest(int lc,int ph,int ls,int sc,int rd,int np,int bw){return add(clvrh,&st.n_harv,&st.t_root,N-6,lc,ph,ls,sc,rd,np,bw);}
int clvr_market(int lc,int ph,int ls,int sc,int rd,int np,int bw){return add(clvrm,&st.n_mkt,&st.t_nitro,N-6,lc,ph,ls,sc,rd,np,bw);}
void clvr_report(void){ps("[CLVR] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nInoc: ");pi(st.n_inoc);ps(" Sp=");pi(st.t_sp);ps("\nMow: ");pi(st.n_mow);ps(" Stm=");pi(st.t_stm);ps("\nHarv: ");pi(st.n_harv);ps(" Root=");pi(st.t_root);ps("\nMkt: ");pi(st.n_mkt);ps(" Nitro=");pi(st.t_nitro);ps("\n");}
void clvr_state(void){ps("[CLVR] Plant=");pi(st.n_plant);ps(" Inoc=");pi(st.n_inoc);ps(" Mow=");pi(st.n_mow);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Clover Admin Demo ===\n\n");clvr_init();
/* 1=field 2=pasture 3=cover_crop 4=orchard 5=market */
ps("Clover planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=10+(i*3),ls=5+(i*2),sc=3+(i*2),rd=15+(i*4),np=20+(i*10),bw=8+(i%6);clvr_planting(lc,ph,ls,sc,rd,np,bw);}
ps("\nClover inoculation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=12+(i*3),ls=6+(i*2),sc=4+(i*2),rd=18+(i*3),np=25+(i*8),bw=9+(i%5);clvr_inoculation(lc,ph,ls,sc,rd,np,bw);}
ps("\nClover mowing...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=14+(i*2),ls=7+(i*2),sc=5+(i*2),rd=20+(i*3),np=30+(i*7),bw=10+(i%4);clvr_mowing(lc,ph,ls,sc,rd,np,bw);}
ps("\nClover harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=8+(i*4),ls=4+(i*3),sc=2+(i*3),rd=12+(i*5),np=15+(i*12),bw=7+(i%7);clvr_harvest(lc,ph,ls,sc,rd,np,bw);}
ps("\nClover market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=16+(i*2),ls=8+(i*2),sc=6+(i*2),rd=22+(i*2),np=35+(i*5),bw=11+(i%3);clvr_market(lc,ph,ls,sc,rd,np,bw);}
ps("\n");clvr_report();clvr_state();ps("\n=== Demo Complete ===\n");return 0;}
