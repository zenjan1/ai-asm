/* falcon_admin: Falcon (Falco peregrinus) Peregrine fast diving raptor (v1.0)
 * Falcon cliff, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, dive_speed, feather_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wng_cm,dv_sp,fth_idx,age_yr,active;} flcn_t;
typedef struct{int n_cliff,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wng,t_dv,t_fth;} flcn_state_t;
static flcn_t clfs[N],clf[N-2],clb[N-4],clh[N-6],clm[N-6]; static flcn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(flcn_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int ds,int fi,int ay){if(*cnt>=mx)return -1;flcn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wng_cm=wc;x->dv_sp=ds;x->fth_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FLCN] Falcon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" ds=");pi(ds);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int falcon_init(void){if(init)return -1;st.n_cliff=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wng=0;st.t_dv=0;st.t_fth=0;for(int i=0;i<N;i++)clfs[i].active=0;for(int i=0;i<N-2;i++)clf[i].active=0;for(int i=0;i<N-4;i++)clb[i].active=0;for(int i=0;i<N-6;i++)clh[i].active=0;for(int i=0;i<N-6;i++)clm[i].active=0;init=1;ps("[FLCN] Falcon initialized\n");return 0;}
int falcon_cliff(int lc,int bl,int bw,int wc,int ds,int fi,int ay){return add(clfs,&st.n_cliff,&st.t_ln,N,lc,bl,bw,wc,ds,fi,ay);}
int falcon_feeding(int lc,int bl,int bw,int wc,int ds,int fi,int ay){return add(clf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,ds,fi,ay);}
int falcon_breeding(int lc,int bl,int bw,int wc,int ds,int fi,int ay){return add(clb,&st.n_breed,&st.t_wng,N-4,lc,bl,bw,wc,ds,fi,ay);}
int falcon_health(int lc,int bl,int bw,int wc,int ds,int fi,int ay){return add(clh,&st.n_health,&st.t_dv,N-6,lc,bl,bw,wc,ds,fi,ay);}
int falcon_market(int lc,int bl,int bw,int wc,int ds,int fi,int ay){return add(clm,&st.n_mkt,&st.t_fth,N-6,lc,bl,bw,wc,ds,fi,ay);}
void falcon_report(void){ps("[FLCN] Cliff: ");pi(st.n_cliff);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wng=");pi(st.t_wng);ps("\nHealth: ");pi(st.n_health);ps(" Dv=");pi(st.t_dv);ps("\nMkt: ");pi(st.n_mkt);ps(" Fth=");pi(st.t_fth);ps("\n");}
void falcon_state(void){ps("[FLCN] Cliff=");pi(st.n_cliff);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Falcon Admin Demo ===\n\n");falcon_init();
ps("Falcon cliff...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=600+(i*50),wc=80+(i*6),ds=50+(i*15),fi=(i%10)+1,ay=(i%12)+1;falcon_cliff(lc,bl,bw,wc,ds,fi,ay);}
ps("\nFalcon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=620+(i*40),wc=82+(i*5),ds=52+(i*12),fi=(i%9)+1,ay=(i%10)+1;falcon_feeding(lc,bl,bw,wc,ds,fi,ay);}
ps("\nFalcon breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=44+(i*2),bw=640+(i*30),wc=84+(i*4),ds=48+(i*18),fi=(i%8)+1,ay=(i%8)+1;falcon_breeding(lc,bl,bw,wc,ds,fi,ay);}
ps("\nFalcon health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=39+(i*4),bw=580+(i*60),wc=78+(i*7),ds=45+(i*20),fi=(i%11)+1,ay=(i%7)+1;falcon_health(lc,bl,bw,wc,ds,fi,ay);}
ps("\nFalcon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=46+(i*2),bw=660+(i*25),wc=86+(i*3),ds=55+(i*10),fi=(i%6)+1,ay=(i%6)+1;falcon_market(lc,bl,bw,wc,ds,fi,ay);}
ps("\n");falcon_report();falcon_state();ps("\n=== Demo Complete ===\n");return 0;}
