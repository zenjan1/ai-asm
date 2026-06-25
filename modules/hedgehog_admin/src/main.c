/* hedgehog_admin: Hedgehog (Erinaceus europaeus) European hedgehog spiny mammal (v1.0)
 * Hedgehog housing, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, spine_ct, curl_idx, snout_cm, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,sp_ct,crl_idx,sn_cm,age_yr,active;} hedgehog_t;
typedef struct{int n_hous,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_sp,t_crl,t_sn;} hedgehog_state_t;
static hedgehog_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static hedgehog_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hedgehog_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int ci,int nc,int ay){if(*cnt>=mx)return -1;hedgehog_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->sp_ct=sc;x->crl_idx=ci;x->sn_cm=nc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HHDG] Hedgehog ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" ci=");pi(ci);ps(" nc=");pi(nc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hedgehog_init(void){if(init)return -1;st.n_hous=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_sp=0;st.t_crl=0;st.t_sn=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HHDG] Hedgehog initialized\n");return 0;}
int hedgehog_housing(int lc,int bl,int bw,int sc,int ci,int nc,int ay){return add(gpl,&st.n_hous,&st.t_ln,N,lc,bl,bw,sc,ci,nc,ay);}
int hedgehog_feeding(int lc,int bl,int bw,int sc,int ci,int nc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,ci,nc,ay);}
int hedgehog_breeding(int lc,int bl,int bw,int sc,int ci,int nc,int ay){return add(gbr,&st.n_breed,&st.t_sp,N-4,lc,bl,bw,sc,ci,nc,ay);}
int hedgehog_health(int lc,int bl,int bw,int sc,int ci,int nc,int ay){return add(ghl,&st.n_health,&st.t_crl,N-6,lc,bl,bw,sc,ci,nc,ay);}
int hedgehog_market(int lc,int bl,int bw,int sc,int ci,int nc,int ay){return add(gmk,&st.n_mkt,&st.t_sn,N-6,lc,bl,bw,sc,ci,nc,ay);}
void hedgehog_report(void){ps("[HHDG] Hous: ");pi(st.n_hous);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Sp=");pi(st.t_sp);ps("\nHlth: ");pi(st.n_health);ps(" Crl=");pi(st.t_crl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sn=");pi(st.t_sn);ps("\n");}
void hedgehog_state(void){ps("[HHDG] Hous=");pi(st.n_hous);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hedgehog Admin Demo ===\n\n");hedgehog_init();
/* 1=garden 2=hedge 3=compost 4=nest 5=home */
ps("Hedgehog housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*2),bw=400+(i*50),sc=2000+(i*300),ci=(i%5)+1,nc=3+(i%3),ay=(i%5)+1;hedgehog_housing(lc,bl,bw,sc,ci,nc,ay);}
ps("\nHedgehog feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=22+(i*2),bw=420+(i*40),sc=2100+(i*250),ci=(i%4)+1,nc=4+(i%2),ay=(i%4)+1;hedgehog_feeding(lc,bl,bw,sc,ci,nc,ay);}
ps("\nHedgehog breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=18+(i*3),bw=380+(i*60),sc=1900+(i*350),ci=(i%3)+2,nc=3+(i%3),ay=(i%3)+1;hedgehog_breeding(lc,bl,bw,sc,ci,nc,ay);}
ps("\nHedgehog health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=25+(i*2),bw=450+(i*35),sc=2200+(i*200),ci=(i%5)+1,nc=5+(i%2),ay=(i%4)+1;hedgehog_health(lc,bl,bw,sc,ci,nc,ay);}
ps("\nHedgehog market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=28+(i*2),bw=500+(i*30),sc=2300+(i*180),ci=(i%4)+1,nc=6+(i%2),ay=(i%3)+1;hedgehog_market(lc,bl,bw,sc,ci,nc,ay);}
ps("\n");hedgehog_report();hedgehog_state();ps("\n=== Demo Complete ===\n");return 0;}
