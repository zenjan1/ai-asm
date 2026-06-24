/* camel_admin: Camel (Camelus) herd and desert livestock management (v1.0)
 * Camel herding, milking, shearing, transport, market
 * Features: herd count, avg weight, milk yield, wool, distance, load capacity
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,herd_ct,avg_wt,milk_yd,wool_kg,dist_km,load_cap,active;} cam_t;
typedef struct{int n_herd,n_milk,n_shear,n_trans,n_mkt,t_herd,t_wt,t_milk,t_wool,t_dist;} cam_state_t;
static cam_t camps[N],camms[N-2],camsrs[N-4],camts[N-6],camks[N-6]; static cam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cam_t*a,int*cnt,int*sum,int mx,int lc,int hc,int aw,int my,int wk,int dk,int lcp){if(*cnt>=mx)return -1;cam_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->herd_ct=hc;x->avg_wt=aw;x->milk_yd=my;x->wool_kg=wk;x->dist_km=dk;x->load_cap=lcp;x->active=1;*sum+=hc;(*cnt)++;ps("[CAM] Camel ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" hc=");pi(hc);ps(" aw=");pi(aw);ps(" my=");pi(my);ps(" wk=");pi(wk);ps(" dk=");pi(dk);ps(" lcp=");pi(lcp);ps("\n");return *cnt-1;}
int cam_init(void){if(init)return -1;st.n_herd=0;st.n_milk=0;st.n_shear=0;st.n_trans=0;st.n_mkt=0;st.t_herd=0;st.t_wt=0;st.t_milk=0;st.t_wool=0;st.t_dist=0;for(int i=0;i<N;i++)camps[i].active=0;for(int i=0;i<N-2;i++)camms[i].active=0;for(int i=0;i<N-4;i++)camsrs[i].active=0;for(int i=0;i<N-6;i++)camts[i].active=0;for(int i=0;i<N-6;i++)camks[i].active=0;init=1;ps("[CAM] Camel initialized\n");return 0;}
int cam_herding(int lc,int hc,int aw,int my,int wk,int dk,int lcp){return add(camps,&st.n_herd,&st.t_herd,N,lc,hc,aw,my,wk,dk,lcp);}
int cam_milking(int lc,int hc,int aw,int my,int wk,int dk,int lcp){return add(camms,&st.n_milk,&st.t_wt,N-2,lc,hc,aw,my,wk,dk,lcp);}
int cam_shearing(int lc,int hc,int aw,int my,int wk,int dk,int lcp){return add(camsrs,&st.n_shear,&st.t_milk,N-4,lc,hc,aw,my,wk,dk,lcp);}
int cam_transport(int lc,int hc,int aw,int my,int wk,int dk,int lcp){return add(camts,&st.n_trans,&st.t_wool,N-6,lc,hc,aw,my,wk,dk,lcp);}
int cam_market(int lc,int hc,int aw,int my,int wk,int dk,int lcp){return add(camks,&st.n_mkt,&st.t_dist,N-6,lc,hc,aw,my,wk,dk,lcp);}
void cam_report(void){ps("[CAM] Herd: ");pi(st.n_herd);ps(" Herd=");pi(st.t_herd);ps("\nMilk: ");pi(st.n_milk);ps(" Wt=");pi(st.t_wt);ps("\nShear: ");pi(st.n_shear);ps(" Milk=");pi(st.t_milk);ps("\nTrans: ");pi(st.n_trans);ps(" Wool=");pi(st.t_wool);ps("\nMkt: ");pi(st.n_mkt);ps(" Dist=");pi(st.t_dist);ps("\n");}
void cam_state(void){ps("[CAM] Herd=");pi(st.n_herd);ps(" Milk=");pi(st.n_milk);ps(" Shear=");pi(st.n_shear);ps(" Trans=");pi(st.n_trans);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Camel Admin Demo ===\n\n");cam_init();
/* 1=desert 2=oasis 3=caravan_route 4=ranch 5=market */
ps("Camel herding...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,hc=5+(i*3),aw=400+(i*30),my=3+(i*2),wk=2+(i%5),dk=20+(i*10),lcp=150+(i*20);cam_herding(lc,hc,aw,my,wk,dk,lcp);}
ps("\nCamel milking...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,hc=6+(i*2),aw=450+(i*25),my=4+(i*2),wk=3+(i%4),dk=25+(i*8),lcp=160+(i*15);cam_milking(lc,hc,aw,my,wk,dk,lcp);}
ps("\nCamel shearing...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,hc=7+(i*2),aw=500+(i*20),my=5+(i*2),wk=4+(i%3),dk=30+(i*6),lcp=170+(i*12);cam_shearing(lc,hc,aw,my,wk,dk,lcp);}
ps("\nCamel transport...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,hc=4+(i*4),aw=350+(i*35),my=2+(i*3),wk=1+(i%5),dk=15+(i*12),lcp=140+(i*25);cam_transport(lc,hc,aw,my,wk,dk,lcp);}
ps("\nCamel market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,hc=8+(i*2),aw=550+(i*15),my=6+(i*2),wk=5+(i%2),dk=35+(i*5),lcp=180+(i*10);cam_market(lc,hc,aw,my,wk,dk,lcp);}
ps("\n");cam_report();cam_state();ps("\n=== Demo Complete ===\n");return 0;}
