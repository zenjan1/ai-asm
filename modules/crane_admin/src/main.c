/* crane_admin: Crane (Grus grus) common tall wading bird (v1.0)
 * Crane marsh, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, leg_cm, fly_speed, neck_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_cm,fl_sp,nk_idx,age_yr,active;} crne_t;
typedef struct{int n_marsh,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_leg,t_fl,t_nk;} crne_state_t;
static crne_t msl[N],msf[N-2],msb[N-4],msh[N-6],msm[N-6]; static crne_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crne_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lc2,int fs,int ni,int ay){if(*cnt>=mx)return -1;crne_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_cm=lc2;x->fl_sp=fs;x->nk_idx=ni;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CRNE] Crane ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lc2=");pi(lc2);ps(" fs=");pi(fs);ps(" ni=");pi(ni);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int crane_init(void){if(init)return -1;st.n_marsh=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_fl=0;st.t_nk=0;for(int i=0;i<N;i++)msl[i].active=0;for(int i=0;i<N-2;i++)msf[i].active=0;for(int i=0;i<N-4;i++)msb[i].active=0;for(int i=0;i<N-6;i++)msh[i].active=0;for(int i=0;i<N-6;i++)msm[i].active=0;init=1;ps("[CRNE] Crane initialized\n");return 0;}
int crane_marsh(int lc,int bl,int bw,int lc2,int fs,int ni,int ay){return add(msl,&st.n_marsh,&st.t_ln,N,lc,bl,bw,lc2,fs,ni,ay);}
int crane_feeding(int lc,int bl,int bw,int lc2,int fs,int ni,int ay){return add(msf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lc2,fs,ni,ay);}
int crane_breeding(int lc,int bl,int bw,int lc2,int fs,int ni,int ay){return add(msb,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,lc2,fs,ni,ay);}
int crane_health(int lc,int bl,int bw,int lc2,int fs,int ni,int ay){return add(msh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,lc2,fs,ni,ay);}
int crane_market(int lc,int bl,int bw,int lc2,int fs,int ni,int ay){return add(msm,&st.n_mkt,&st.t_nk,N-6,lc,bl,bw,lc2,fs,ni,ay);}
void crane_report(void){ps("[CRNE] Marsh: ");pi(st.n_marsh);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Nk=");pi(st.t_nk);ps("\n");}
void crane_state(void){ps("[CRNE] Marsh=");pi(st.n_marsh);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Crane Admin Demo ===\n\n");crane_init();
ps("Crane marsh...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*8),bw=300+(i*40),leg=80+(i*6),fs=20+(i*3),ni=(i%8)+1,ay=(i%15)+1;crane_marsh(lc,bl,bw,leg,fs,ni,ay);}
ps("\nCrane feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=105+(i*6),bw=320+(i*30),leg=84+(i*5),fs=22+(i*2),ni=(i%6)+1,ay=(i%12)+1;crane_feeding(lc,bl,bw,leg,fs,ni,ay);}
ps("\nCrane breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=110+(i*5),bw=340+(i*25),leg=88+(i*4),fs=18+(i*4),ni=(i%5)+1,ay=(i%10)+1;crane_breeding(lc,bl,bw,leg,fs,ni,ay);}
ps("\nCrane health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*9),bw=290+(i*50),leg=78+(i*7),fs=15+(i*5),ni=(i%10)+1,ay=(i%8)+1;crane_health(lc,bl,bw,leg,fs,ni,ay);}
ps("\nCrane market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=115+(i*4),bw=360+(i*20),leg=90+(i*3),fs=25+(i*3),ni=(i%4)+1,ay=(i%7)+1;crane_market(lc,bl,bw,leg,fs,ni,ay);}
ps("\n");crane_report();crane_state();ps("\n=== Demo Complete ===\n");return 0;}
