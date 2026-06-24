/* daisy_admin: Daisy (Bellis perennis) perennial flowering plant (v1.0)
 * Daisy planting, feeding, flowering, pruning, market
 * Features: plant_ht_cm, flower_dia_cm, petal_count, leaf_idx, bloom_week, seed_ct
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,fl_dia,pt_ct,lf_idx,blm_wk,sd_ct,active;} dais_t;
typedef struct{int n_plant,n_feed,n_flow,n_prun,n_mkt,t_ht,t_dia,t_pt,t_lf,t_sd;} dais_state_t;
static dais_t dpl[N],dfd[N-2],dfl[N-4],dpr[N-6],dmk[N-6]; static dais_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dais_t*a,int*cnt,int*sum,int mx,int lc,int ph,int fd,int pc,int li,int bw,int sc){if(*cnt>=mx)return -1;dais_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->fl_dia=fd;x->pt_ct=pc;x->lf_idx=li;x->blm_wk=bw;x->sd_ct=sc;x->active=1;*sum+=ph;(*cnt)++;ps("[DAIS] Daisy ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" li=");pi(li);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps("\n");return *cnt-1;}
int dais_init(void){if(init)return -1;st.n_plant=0;st.n_feed=0;st.n_flow=0;st.n_prun=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_pt=0;st.t_lf=0;st.t_sd=0;for(int i=0;i<N;i++)dpl[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dfl[i].active=0;for(int i=0;i<N-6;i++)dpr[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DAIS] Daisy initialized\n");return 0;}
int dais_planting(int lc,int ph,int fd,int pc,int li,int bw,int sc){return add(dpl,&st.n_plant,&st.t_ht,N,lc,ph,fd,pc,li,bw,sc);}
int dais_feeding(int lc,int ph,int fd,int pc,int li,int bw,int sc){return add(dfd,&st.n_feed,&st.t_dia,N-2,lc,ph,fd,pc,li,bw,sc);}
int dais_flowering(int lc,int ph,int fd,int pc,int li,int bw,int sc){return add(dfl,&st.n_flow,&st.t_pt,N-4,lc,ph,fd,pc,li,bw,sc);}
int dais_pruning(int lc,int ph,int fd,int pc,int li,int bw,int sc){return add(dpr,&st.n_prun,&st.t_lf,N-6,lc,ph,fd,pc,li,bw,sc);}
int dais_market(int lc,int ph,int fd,int pc,int li,int bw,int sc){return add(dmk,&st.n_mkt,&st.t_sd,N-6,lc,ph,fd,pc,li,bw,sc);}
void dais_report(void){ps("[DAIS] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nFeed: ");pi(st.n_feed);ps(" Dia=");pi(st.t_dia);ps("\nFlow: ");pi(st.n_flow);ps(" Pt=");pi(st.t_pt);ps("\nPrun: ");pi(st.n_prun);ps(" Lf=");pi(st.t_lf);ps("\nMkt: ");pi(st.n_mkt);ps(" Sd=");pi(st.t_sd);ps("\n");}
void dais_state(void){ps("[DAIS] Plant=");pi(st.n_plant);ps(" Feed=");pi(st.n_feed);ps(" Flow=");pi(st.n_flow);ps(" Prun=");pi(st.n_prun);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Daisy Admin Demo ===\n\n");dais_init();
/* 1=garden 2=field 3=park 4=border 5=market */
ps("Daisy planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=15+(i*2),fd=4+(i%3),pc=12+(i%8),li=(i%5)+1,bw=(i%12)+1,sc=20+(i*5);dais_planting(lc,ph,fd,pc,li,bw,sc);}
ps("\nDaisy feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=16+(i*2),fd=5+(i%3),pc=14+(i%6),li=(i%4)+1,bw=(i%10)+1,sc=25+(i*4);dais_feeding(lc,ph,fd,pc,li,bw,sc);}
ps("\nDaisy flowering...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=18+(i*2),fd=6+(i%2),pc=16+(i%5),li=(i%6)+1,bw=(i%8)+1,sc=30+(i*3);dais_flowering(lc,ph,fd,pc,li,bw,sc);}
ps("\nDaisy pruning...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=14+(i*3),fd=3+(i%4),pc=10+(i%7),li=(i%5)+1,bw=(i%11)+1,sc=15+(i*6);dais_pruning(lc,ph,fd,pc,li,bw,sc);}
ps("\nDaisy market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=20+(i*2),fd=7+(i%2),pc=18+(i%4),li=(i%3)+4,bw=(i%6)+1,sc=35+(i*3);dais_market(lc,ph,fd,pc,li,bw,sc);}
ps("\n");dais_report();dais_state();ps("\n=== Demo Complete ===\n");return 0;}
