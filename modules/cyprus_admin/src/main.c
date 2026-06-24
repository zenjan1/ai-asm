/* cyprus_admin: Cyclamen (Cyclamen persicum) tuberous flowering plant (v1.0)
 * Cyclamen planting, feeding, dormancy, flowering, market
 * Features: plant_ht_cm, flower_dia_cm, petal_count, tuber_wt_g, color_idx, bloom_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,fl_dia,pt_ct,tubr_wt,clr_idx,blm_wk,active;} cycl_t;
typedef struct{int n_plant,n_feed,n_dorm,n_flow,n_mkt,t_ht,t_dia,t_pt,t_tubr,t_clr;} cycl_state_t;
static cycl_t cyclps[N],cyclfd[N-2],cycldm[N-4],cyclfl[N-6],cyclmk[N-6]; static cycl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cycl_t*a,int*cnt,int*sum,int mx,int lc,int ph,int fd,int pc,int tw,int ci,int bw){if(*cnt>=mx)return -1;cycl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->fl_dia=fd;x->pt_ct=pc;x->tubr_wt=tw;x->clr_idx=ci;x->blm_wk=bw;x->active=1;*sum+=ph;(*cnt)++;ps("[CYCL] Cyclamen ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" tw=");pi(tw);ps(" ci=");pi(ci);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int cycl_init(void){if(init)return -1;st.n_plant=0;st.n_feed=0;st.n_dorm=0;st.n_flow=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_pt=0;st.t_tubr=0;st.t_clr=0;for(int i=0;i<N;i++)cyclps[i].active=0;for(int i=0;i<N-2;i++)cyclfd[i].active=0;for(int i=0;i<N-4;i++)cycldm[i].active=0;for(int i=0;i<N-6;i++)cyclfl[i].active=0;for(int i=0;i<N-6;i++)cyclmk[i].active=0;init=1;ps("[CYCL] Cyclamen initialized\n");return 0;}
int cycl_planting(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(cyclps,&st.n_plant,&st.t_ht,N,lc,ph,fd,pc,tw,ci,bw);}
int cycl_feeding(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(cyclfd,&st.n_feed,&st.t_dia,N-2,lc,ph,fd,pc,tw,ci,bw);}
int cycl_dormancy(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(cycldm,&st.n_dorm,&st.t_pt,N-4,lc,ph,fd,pc,tw,ci,bw);}
int cycl_flowering(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(cyclfl,&st.n_flow,&st.t_tubr,N-6,lc,ph,fd,pc,tw,ci,bw);}
int cycl_market(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(cyclmk,&st.n_mkt,&st.t_clr,N-6,lc,ph,fd,pc,tw,ci,bw);}
void cycl_report(void){ps("[CYCL] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nFeed: ");pi(st.n_feed);ps(" Dia=");pi(st.t_dia);ps("\nDorm: ");pi(st.n_dorm);ps(" Pt=");pi(st.t_pt);ps("\nFlow: ");pi(st.n_flow);ps(" Tubr=");pi(st.t_tubr);ps("\nMkt: ");pi(st.n_mkt);ps(" Clr=");pi(st.t_clr);ps("\n");}
void cycl_state(void){ps("[CYCL] Plant=");pi(st.n_plant);ps(" Feed=");pi(st.n_feed);ps(" Dorm=");pi(st.n_dorm);ps(" Flow=");pi(st.n_flow);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cyclamen Admin Demo ===\n\n");cycl_init();
/* 1=garden 2=greenhouse 3=container 4=indoor 5=market */
ps("Cyclamen planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=10+(i*2),fd=2+(i*2),pc=5+(i*2),tw=20+(i*5),ci=(i%7)+1,bw=10+(i%6);cycl_planting(lc,ph,fd,pc,tw,ci,bw);}
ps("\nCyclamen feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=12+(i*2),fd=3+(i*2),pc=6+(i*2),tw=22+(i*4),ci=(i%6)+1,bw=11+(i%5);cycl_feeding(lc,ph,fd,pc,tw,ci,bw);}
ps("\nCyclamen dormancy...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=14+(i*2),fd=4+(i*2),pc=7+(i*2),tw=24+(i*3),ci=(i%5)+1,bw=12+(i%4);cycl_dormancy(lc,ph,fd,pc,tw,ci,bw);}
ps("\nCyclamen flowering...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=8+(i*3),fd=2+(i*3),pc=4+(i*3),tw=18+(i*6),ci=(i%8)+1,bw=9+(i%7);cycl_flowering(lc,ph,fd,pc,tw,ci,bw);}
ps("\nCyclamen market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=16+(i*2),fd=5+(i*2),pc=8+(i*2),tw=26+(i*3),ci=(i%4)+4,bw=13+(i%3);cycl_market(lc,ph,fd,pc,tw,ci,bw);}
ps("\n");cycl_report();cycl_state();ps("\n=== Demo Complete ===\n");return 0;}
