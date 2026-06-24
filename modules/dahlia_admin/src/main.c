/* dahlia_admin: Dahlia (Dahlia pinnata) tuberous flowering plant (v1.0)
 * Dahlia planting, feeding, dormancy, flowering, market
 * Features: plant_ht_cm, flower_dia_cm, petal_count, tuber_wt_g, color_idx, bloom_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,fl_dia,pt_ct,tubr_wt,clr_idx,blm_wk,active;} dahl_t;
typedef struct{int n_plant,n_feed,n_dorm,n_flow,n_mkt,t_ht,t_dia,t_pt,t_tb,t_cl;} dahl_state_t;
static dahl_t dpl[N],dfd[N-2],ddr[N-4],dfl[N-6],dmk[N-6]; static dahl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dahl_t*a,int*cnt,int*sum,int mx,int lc,int ph,int fd,int pc,int tw,int ci,int bw){if(*cnt>=mx)return -1;dahl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->fl_dia=fd;x->pt_ct=pc;x->tubr_wt=tw;x->clr_idx=ci;x->blm_wk=bw;x->active=1;*sum+=ph;(*cnt)++;ps("[DAHL] Dahlia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" tw=");pi(tw);ps(" ci=");pi(ci);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int dahl_init(void){if(init)return -1;st.n_plant=0;st.n_feed=0;st.n_dorm=0;st.n_flow=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_pt=0;st.t_tb=0;st.t_cl=0;for(int i=0;i<N;i++)dpl[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)ddr[i].active=0;for(int i=0;i<N-6;i++)dfl[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DAHL] Dahlia initialized\n");return 0;}
int dahl_planting(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(dpl,&st.n_plant,&st.t_ht,N,lc,ph,fd,pc,tw,ci,bw);}
int dahl_feeding(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(dfd,&st.n_feed,&st.t_dia,N-2,lc,ph,fd,pc,tw,ci,bw);}
int dahl_dormancy(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(ddr,&st.n_dorm,&st.t_pt,N-4,lc,ph,fd,pc,tw,ci,bw);}
int dahl_flowering(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(dfl,&st.n_flow,&st.t_tb,N-6,lc,ph,fd,pc,tw,ci,bw);}
int dahl_market(int lc,int ph,int fd,int pc,int tw,int ci,int bw){return add(dmk,&st.n_mkt,&st.t_cl,N-6,lc,ph,fd,pc,tw,ci,bw);}
void dahl_report(void){ps("[DAHL] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nFeed: ");pi(st.n_feed);ps(" Dia=");pi(st.t_dia);ps("\nDorm: ");pi(st.n_dorm);ps(" Pt=");pi(st.t_pt);ps("\nFlow: ");pi(st.n_flow);ps(" Tb=");pi(st.t_tb);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void dahl_state(void){ps("[DAHL] Plant=");pi(st.n_plant);ps(" Feed=");pi(st.n_feed);ps(" Dorm=");pi(st.n_dorm);ps(" Flow=");pi(st.n_flow);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dahlia Admin Demo ===\n\n");dahl_init();
/* 1=garden 2=greenhouse 3=park 4=border 5=market */
ps("Dahlia planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=60+(i*5),fd=10+(i*3),pc=20+(i%10),tw=50+(i*8),ci=(i%8)+1,bw=(i%12)+1;dahl_planting(lc,ph,fd,pc,tw,ci,bw);}
ps("\nDahlia feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=65+(i*4),fd=12+(i*2),pc=22+(i%8),tw=55+(i*7),ci=(i%7)+1,bw=(i%10)+1;dahl_feeding(lc,ph,fd,pc,tw,ci,bw);}
ps("\nDahlia dormancy...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=55+(i*6),fd=8+(i*3),pc=18+(i%9),tw=45+(i*9),ci=(i%6)+1,bw=(i%8)+1;dahl_dormancy(lc,ph,fd,pc,tw,ci,bw);}
ps("\nDahlia flowering...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=70+(i*4),fd=14+(i*2),pc=25+(i%7),tw=60+(i*6),ci=(i%5)+1,bw=(i%11)+1;dahl_flowering(lc,ph,fd,pc,tw,ci,bw);}
ps("\nDahlia market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=75+(i*3),fd=15+(i*2),pc=28+(i%6),tw=65+(i*5),ci=(i%4)+4,bw=(i%6)+4;dahl_market(lc,ph,fd,pc,tw,ci,bw);}
ps("\n");dahl_report();dahl_state();ps("\n=== Demo Complete ===\n");return 0;}
