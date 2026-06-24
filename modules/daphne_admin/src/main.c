/* daphne_admin: Daphne (Daphne odora) evergreen fragrant shrub (v1.0)
 * Daphne planting, feeding, pruning, flowering, market
 * Features: plant_ht_cm, spread_cm, flower_dia_cm, leaf_idx, scent_idx, bloom_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,sprd_cm,fl_dia,lf_idx,sct_idx,blm_wk,active;} daph_t;
typedef struct{int n_plant,n_feed,n_prun,n_flow,n_mkt,t_ht,t_sp,t_dia,t_lf,t_sc;} daph_state_t;
static daph_t dpl[N],dfd[N-2],dpr[N-4],dfl[N-6],dmk[N-6]; static daph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(daph_t*a,int*cnt,int*sum,int mx,int lc,int ph,int sp,int fd,int li,int si,int bw){if(*cnt>=mx)return -1;daph_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->sprd_cm=sp;x->fl_dia=fd;x->lf_idx=li;x->sct_idx=si;x->blm_wk=bw;x->active=1;*sum+=ph;(*cnt)++;ps("[DAPH] Daphne ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" sp=");pi(sp);ps(" fd=");pi(fd);ps(" li=");pi(li);ps(" si=");pi(si);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int daph_init(void){if(init)return -1;st.n_plant=0;st.n_feed=0;st.n_prun=0;st.n_flow=0;st.n_mkt=0;st.t_ht=0;st.t_sp=0;st.t_dia=0;st.t_lf=0;st.t_sc=0;for(int i=0;i<N;i++)dpl[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dpr[i].active=0;for(int i=0;i<N-6;i++)dfl[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DAPH] Daphne initialized\n");return 0;}
int daph_planting(int lc,int ph,int sp,int fd,int li,int si,int bw){return add(dpl,&st.n_plant,&st.t_ht,N,lc,ph,sp,fd,li,si,bw);}
int daph_feeding(int lc,int ph,int sp,int fd,int li,int si,int bw){return add(dfd,&st.n_feed,&st.t_sp,N-2,lc,ph,sp,fd,li,si,bw);}
int daph_pruning(int lc,int ph,int sp,int fd,int li,int si,int bw){return add(dpr,&st.n_prun,&st.t_dia,N-4,lc,ph,sp,fd,li,si,bw);}
int daph_flowering(int lc,int ph,int sp,int fd,int li,int si,int bw){return add(dfl,&st.n_flow,&st.t_lf,N-6,lc,ph,sp,fd,li,si,bw);}
int daph_market(int lc,int ph,int sp,int fd,int li,int si,int bw){return add(dmk,&st.n_mkt,&st.t_sc,N-6,lc,ph,sp,fd,li,si,bw);}
void daph_report(void){ps("[DAPH] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nFeed: ");pi(st.n_feed);ps(" Sp=");pi(st.t_sp);ps("\nPrun: ");pi(st.n_prun);ps(" Dia=");pi(st.t_dia);ps("\nFlow: ");pi(st.n_flow);ps(" Lf=");pi(st.t_lf);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void daph_state(void){ps("[DAPH] Plant=");pi(st.n_plant);ps(" Feed=");pi(st.n_feed);ps(" Prun=");pi(st.n_prun);ps(" Flow=");pi(st.n_flow);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Daphne Admin Demo ===\n\n");daph_init();
/* 1=garden 2=courtyard 3=park 4=border 5=market */
ps("Daphne planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=50+(i*5),sp=40+(i*4),fd=1+(i%2),li=(i%6)+1,si=(i%8)+1,bw=(i%12)+1;daph_planting(lc,ph,sp,fd,li,si,bw);}
ps("\nDaphne feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=55+(i*4),sp=45+(i*3),fd=1+(i%3),li=(i%5)+1,si=(i%7)+1,bw=(i%10)+1;daph_feeding(lc,ph,sp,fd,li,si,bw);}
ps("\nDaphne pruning...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=45+(i*6),sp=35+(i*5),fd=2+(i%2),li=(i%4)+1,si=(i%6)+1,bw=(i%8)+1;daph_pruning(lc,ph,sp,fd,li,si,bw);}
ps("\nDaphne flowering...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=60+(i*4),sp=50+(i*3),fd=2+(i%2),li=(i%7)+1,si=(i%5)+1,bw=(i%11)+1;daph_flowering(lc,ph,sp,fd,li,si,bw);}
ps("\nDaphne market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=65+(i*3),sp=55+(i*3),fd=3+(i%2),li=(i%3)+4,si=(i%4)+4,bw=(i%6)+4;daph_market(lc,ph,sp,fd,li,si,bw);}
ps("\n");daph_report();daph_state();ps("\n=== Demo Complete ===\n");return 0;}
