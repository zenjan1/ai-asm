/* cineraria_admin: Cineraria (Pericallis x hybrida) flowering ornamental (v1.0)
 * Cineraria planting, feeding, pinching, flowering, market
 * Features: plant_ht_cm, flower_dia_cm, petal_count, leaf_span_cm, color_idx, bloom_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,fl_dia,pt_ct,lf_sp,clr_idx,blm_wk,active;} cnr_t;
typedef struct{int n_plant,n_feed,n_pinch,n_flow,n_mkt,t_ht,t_dia,t_pt,t_lf,t_clr;} cnr_state_t;
static cnr_t cnrps[N],cnrfs[N-2],cnrds[N-4],cnrfl[N-6],cnrms[N-6]; static cnr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cnr_t*a,int*cnt,int*sum,int mx,int lc,int ph,int fd,int pc,int ls,int ci,int bw){if(*cnt>=mx)return -1;cnr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->fl_dia=fd;x->pt_ct=pc;x->lf_sp=ls;x->clr_idx=ci;x->blm_wk=bw;x->active=1;*sum+=ph;(*cnt)++;ps("[CNR] Cineraria ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" ls=");pi(ls);ps(" ci=");pi(ci);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int cnr_init(void){if(init)return -1;st.n_plant=0;st.n_feed=0;st.n_pinch=0;st.n_flow=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_pt=0;st.t_lf=0;st.t_clr=0;for(int i=0;i<N;i++)cnrps[i].active=0;for(int i=0;i<N-2;i++)cnrfs[i].active=0;for(int i=0;i<N-4;i++)cnrds[i].active=0;for(int i=0;i<N-6;i++)cnrfl[i].active=0;for(int i=0;i<N-6;i++)cnrms[i].active=0;init=1;ps("[CNR] Cineraria initialized\n");return 0;}
int cnr_planting(int lc,int ph,int fd,int pc,int ls,int ci,int bw){return add(cnrps,&st.n_plant,&st.t_ht,N,lc,ph,fd,pc,ls,ci,bw);}
int cnr_feeding(int lc,int ph,int fd,int pc,int ls,int ci,int bw){return add(cnrfs,&st.n_feed,&st.t_dia,N-2,lc,ph,fd,pc,ls,ci,bw);}
int cnr_pinching(int lc,int ph,int fd,int pc,int ls,int ci,int bw){return add(cnrds,&st.n_pinch,&st.t_pt,N-4,lc,ph,fd,pc,ls,ci,bw);}
int cnr_flowering(int lc,int ph,int fd,int pc,int ls,int ci,int bw){return add(cnrfl,&st.n_flow,&st.t_lf,N-6,lc,ph,fd,pc,ls,ci,bw);}
int cnr_market(int lc,int ph,int fd,int pc,int ls,int ci,int bw){return add(cnrms,&st.n_mkt,&st.t_clr,N-6,lc,ph,fd,pc,ls,ci,bw);}
void cnr_report(void){ps("[CNR] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nFeed: ");pi(st.n_feed);ps(" Dia=");pi(st.t_dia);ps("\nPinch: ");pi(st.n_pinch);ps(" Pt=");pi(st.t_pt);ps("\nFlow: ");pi(st.n_flow);ps(" Lf=");pi(st.t_lf);ps("\nMkt: ");pi(st.n_mkt);ps(" Clr=");pi(st.t_clr);ps("\n");}
void cnr_state(void){ps("[CNR] Plant=");pi(st.n_plant);ps(" Feed=");pi(st.n_feed);ps(" Pinch=");pi(st.n_pinch);ps(" Flow=");pi(st.n_flow);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cineraria Admin Demo ===\n\n");cnr_init();
/* 1=garden 2=greenhouse 3=container 4=shade_house 5=market */
ps("Cineraria planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=15+(i*3),fd=3+(i*2),pc=8+(i*3),ls=10+(i*4),ci=(i%7)+1,bw=12+(i%6);cnr_planting(lc,ph,fd,pc,ls,ci,bw);}
ps("\nCineraria feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=18+(i*3),fd=4+(i*2),pc=10+(i*3),ls=12+(i*3),ci=(i%6)+1,bw=13+(i%5);cnr_feeding(lc,ph,fd,pc,ls,ci,bw);}
ps("\nCineraria pinching...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=20+(i*3),fd=5+(i*2),pc=12+(i*2),ls=14+(i*3),ci=(i%5)+1,bw=14+(i%4);cnr_pinching(lc,ph,fd,pc,ls,ci,bw);}
ps("\nCineraria flowering...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=12+(i*4),fd=2+(i*3),pc=6+(i*4),ls=8+(i*5),ci=(i%8)+1,bw=10+(i%7);cnr_flowering(lc,ph,fd,pc,ls,ci,bw);}
ps("\nCineraria market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=22+(i*2),fd=6+(i*2),pc=14+(i*2),ls=16+(i*2),ci=(i%4)+4,bw=15+(i%3);cnr_market(lc,ph,fd,pc,ls,ci,bw);}
ps("\n");cnr_report();cnr_state();ps("\n=== Demo Complete ===\n");return 0;}
