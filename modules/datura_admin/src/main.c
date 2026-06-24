/* datura_admin: Datura (Datura stramonium) trumpet flowering plant (v1.0)
 * Datura planting, feeding, pruning, flowering, market
 * Features: plant_ht_cm, flower_dia_cm, flower_len_cm, petal_idx, scent_lvl, bloom_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,fl_dia,fl_ln,pt_idx,sct_lv,blm_wk,active;} datu_t;
typedef struct{int n_plant,n_feed,n_prun,n_flow,n_mkt,t_ht,t_dia,t_ln,t_pt,t_sc;} datu_state_t;
static datu_t dpl[N],dfd[N-2],dpr[N-4],dfl[N-6],dmk[N-6]; static datu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(datu_t*a,int*cnt,int*sum,int mx,int lc,int ph,int fd,int fl,int px,int sl,int bw){if(*cnt>=mx)return -1;datu_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->fl_dia=fd;x->fl_ln=fl;x->pt_idx=px;x->sct_lv=sl;x->blm_wk=bw;x->active=1;*sum+=ph;(*cnt)++;ps("[DATU] Datura ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" fd=");pi(fd);ps(" fl=");pi(fl);ps(" px=");pi(px);ps(" sl=");pi(sl);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int datu_init(void){if(init)return -1;st.n_plant=0;st.n_feed=0;st.n_prun=0;st.n_flow=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_ln=0;st.t_pt=0;st.t_sc=0;for(int i=0;i<N;i++)dpl[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dpr[i].active=0;for(int i=0;i<N-6;i++)dfl[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DATU] Datura initialized\n");return 0;}
int datu_planting(int lc,int ph,int fd,int fl,int px,int sl,int bw){return add(dpl,&st.n_plant,&st.t_ht,N,lc,ph,fd,fl,px,sl,bw);}
int datu_feeding(int lc,int ph,int fd,int fl,int px,int sl,int bw){return add(dfd,&st.n_feed,&st.t_dia,N-2,lc,ph,fd,fl,px,sl,bw);}
int datu_pruning(int lc,int ph,int fd,int fl,int px,int sl,int bw){return add(dpr,&st.n_prun,&st.t_ln,N-4,lc,ph,fd,fl,px,sl,bw);}
int datu_flowering(int lc,int ph,int fd,int fl,int px,int sl,int bw){return add(dfl,&st.n_flow,&st.t_pt,N-6,lc,ph,fd,fl,px,sl,bw);}
int datu_market(int lc,int ph,int fd,int fl,int px,int sl,int bw){return add(dmk,&st.n_mkt,&st.t_sc,N-6,lc,ph,fd,fl,px,sl,bw);}
void datu_report(void){ps("[DATU] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nFeed: ");pi(st.n_feed);ps(" Dia=");pi(st.t_dia);ps("\nPrun: ");pi(st.n_prun);ps(" Ln=");pi(st.t_ln);ps("\nFlow: ");pi(st.n_flow);ps(" Pt=");pi(st.t_pt);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void datu_state(void){ps("[DATU] Plant=");pi(st.n_plant);ps(" Feed=");pi(st.n_feed);ps(" Prun=");pi(st.n_prun);ps(" Flow=");pi(st.n_flow);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Datura Admin Demo ===\n\n");datu_init();
/* 1=garden 2=greenhouse 3=park 4=border 5=market */
ps("Datura planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=80+(i*8),fd=8+(i*3),fl=12+(i*2),px=(i%5)+1,sl=(i%7)+1,bw=(i%12)+1;datu_planting(lc,ph,fd,fl,px,sl,bw);}
ps("\nDatura feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=85+(i*6),fd=9+(i*2),fl=13+(i*2),px=(i%4)+1,sl=(i%6)+1,bw=(i%10)+1;datu_feeding(lc,ph,fd,fl,px,sl,bw);}
ps("\nDatura pruning...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=75+(i*9),fd=7+(i*3),fl=11+(i*3),px=(i%6)+1,sl=(i%5)+1,bw=(i%8)+1;datu_pruning(lc,ph,fd,fl,px,sl,bw);}
ps("\nDatura flowering...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=90+(i*5),fd=10+(i*2),fl=14+(i*2),px=(i%3)+1,sl=(i%8)+1,bw=(i%11)+1;datu_flowering(lc,ph,fd,fl,px,sl,bw);}
ps("\nDatura market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=95+(i*4),fd=11+(i*2),fl=15+(i*2),px=(i%4)+3,sl=(i%5)+4,bw=(i%6)+4;datu_market(lc,ph,fd,fl,px,sl,bw);}
ps("\n");datu_report();datu_state();ps("\n=== Demo Complete ===\n");return 0;}
