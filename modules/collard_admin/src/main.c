/* collard_admin: Collard (Brassica oleracea) leafy green vegetable cultivation (v1.0)
 * Collard planting, cutting, blanching, harvest, market
 * Features: leaf_span_cm, stem_height_cm, leaf_count, rib_width_mm, bolt_week, cutting_count
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,lf_sp,stm_ht,lf_ct,rib_wd,bolt_wk,cut_ct,active;} cold_t;
typedef struct{int n_plant,n_cut,n_blanch,n_harv,n_mkt,t_sp,t_ht,t_lf,t_rib,t_bolt;} cold_state_t;
static cold_t coldps[N],colds[N-2],coldr[N-4],coldh[N-6],coldm[N-6]; static cold_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cold_t*a,int*cnt,int*sum,int mx,int lc,int ls,int sh,int lfc,int rw,int bw,int cc){if(*cnt>=mx)return -1;cold_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->lf_sp=ls;x->stm_ht=sh;x->lf_ct=lfc;x->rib_wd=rw;x->bolt_wk=bw;x->cut_ct=cc;x->active=1;*sum+=ls;(*cnt)++;ps("[COLD] Collard ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ls=");pi(ls);ps(" sh=");pi(sh);ps(" lfc=");pi(lfc);ps(" rw=");pi(rw);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps("\n");return *cnt-1;}
int cold_init(void){if(init)return -1;st.n_plant=0;st.n_cut=0;st.n_blanch=0;st.n_harv=0;st.n_mkt=0;st.t_sp=0;st.t_ht=0;st.t_lf=0;st.t_rib=0;st.t_bolt=0;for(int i=0;i<N;i++)coldps[i].active=0;for(int i=0;i<N-2;i++)colds[i].active=0;for(int i=0;i<N-4;i++)coldr[i].active=0;for(int i=0;i<N-6;i++)coldh[i].active=0;for(int i=0;i<N-6;i++)coldm[i].active=0;init=1;ps("[COLD] Collard initialized\n");return 0;}
int cold_planting(int lc,int ls,int sh,int lfc,int rw,int bw,int cc){return add(coldps,&st.n_plant,&st.t_sp,N,lc,ls,sh,lfc,rw,bw,cc);}
int cold_cutting(int lc,int ls,int sh,int lfc,int rw,int bw,int cc){return add(colds,&st.n_cut,&st.t_ht,N-2,lc,ls,sh,lfc,rw,bw,cc);}
int cold_blanching(int lc,int ls,int sh,int lfc,int rw,int bw,int cc){return add(coldr,&st.n_blanch,&st.t_lf,N-4,lc,ls,sh,lfc,rw,bw,cc);}
int cold_harvest(int lc,int ls,int sh,int lfc,int rw,int bw,int cc){return add(coldh,&st.n_harv,&st.t_rib,N-6,lc,ls,sh,lfc,rw,bw,cc);}
int cold_market(int lc,int ls,int sh,int lfc,int rw,int bw,int cc){return add(coldm,&st.n_mkt,&st.t_bolt,N-6,lc,ls,sh,lfc,rw,bw,cc);}
void cold_report(void){ps("[COLD] Plant: ");pi(st.n_plant);ps(" Sp=");pi(st.t_sp);ps("\nCut: ");pi(st.n_cut);ps(" Ht=");pi(st.t_ht);ps("\nBlanch: ");pi(st.n_blanch);ps(" Lf=");pi(st.t_lf);ps("\nHarv: ");pi(st.n_harv);ps(" Rib=");pi(st.t_rib);ps("\nMkt: ");pi(st.n_mkt);ps(" Bolt=");pi(st.t_bolt);ps("\n");}
void cold_state(void){ps("[COLD] Plant=");pi(st.n_plant);ps(" Cut=");pi(st.n_cut);ps(" Blanch=");pi(st.n_blanch);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Collard Admin Demo ===\n\n");cold_init();
/* 1=garden 2=raised_bed 3=greenhouse 4=organic 5=market */
ps("Collard planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ls=25+(i*4),sh=35+(i*6),lfc=5+(i*2),rw=3+(i%4),bw=12+(i%4),cc=2+(i%5);cold_planting(lc,ls,sh,lfc,rw,bw,cc);}
ps("\nCollard cutting...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ls=28+(i*3),sh=38+(i*5),lfc=6+(i*2),rw=4+(i%3),bw=14+(i%3),cc=3+(i%4);cold_cutting(lc,ls,sh,lfc,rw,bw,cc);}
ps("\nCollard blanching...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ls=30+(i*3),sh=42+(i*4),lfc=7+(i*2),rw=5+(i%3),bw=16+(i%2),cc=4+(i%3);cold_blanching(lc,ls,sh,lfc,rw,bw,cc);}
ps("\nCollard harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ls=22+(i*5),sh=30+(i*7),lfc=4+(i*3),rw=2+(i%5),bw=10+(i%5),cc=1+(i%6);cold_harvest(lc,ls,sh,lfc,rw,bw,cc);}
ps("\nCollard market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ls=35+(i*3),sh=48+(i*4),lfc=8+(i*2),rw=6+(i%2),bw=18+(i%2),cc=5+(i%3);cold_market(lc,ls,sh,lfc,rw,bw,cc);}
ps("\n");cold_report();cold_state();ps("\n=== Demo Complete ===\n");return 0;}
