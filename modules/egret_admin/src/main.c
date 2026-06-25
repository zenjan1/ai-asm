/* egret_admin: Egret (Ardea alba) great white wading bird (v1.0)
 * Egret marsh, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, fly_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,fl_sp,pm_idx,age_yr,active;} egrt_t;
typedef struct{int n_marsh,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_fl,t_pm;} egrt_state_t;
static egrt_t msl[N],msf[N-2],msb[N-4],msh[N-6],msm[N-6]; static egrt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(egrt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int fs,int pi2,int ay){if(*cnt>=mx)return -1;egrt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->fl_sp=fs;x->pm_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[EGRT] Egret ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" fs=");pi(fs);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int egret_init(void){if(init)return -1;st.n_marsh=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_fl=0;st.t_pm=0;for(int i=0;i<N;i++)msl[i].active=0;for(int i=0;i<N-2;i++)msf[i].active=0;for(int i=0;i<N-4;i++)msb[i].active=0;for(int i=0;i<N-6;i++)msh[i].active=0;for(int i=0;i<N-6;i++)msm[i].active=0;init=1;ps("[EGRT] Egret initialized\n");return 0;}
int egret_marsh(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(msl,&st.n_marsh,&st.t_ln,N,lc,bl,bw,bc,fs,pi2,ay);}
int egret_feeding(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(msf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,fs,pi2,ay);}
int egret_breeding(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(msb,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,fs,pi2,ay);}
int egret_health(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(msh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,bc,fs,pi2,ay);}
int egret_market(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(msm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,bc,fs,pi2,ay);}
void egret_report(void){ps("[EGRT] Marsh: ");pi(st.n_marsh);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void egret_state(void){ps("[EGRT] Marsh=");pi(st.n_marsh);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Egret Admin Demo ===\n\n");egret_init();
ps("Egret marsh...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=70+(i*5),bw=400+(i*30),bc=10+(i*2),fs=12+(i*2),pi2=(i%8)+1,ay=(i%12)+1;egret_marsh(lc,bl,bw,bc,fs,pi2,ay);}
ps("\nEgret feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=73+(i*4),bw=420+(i*25),bc=11+(i%2),fs=13+(i*1),pi2=(i%6)+1,ay=(i%10)+1;egret_feeding(lc,bl,bw,bc,fs,pi2,ay);}
ps("\nEgret breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=76+(i*3),bw=440+(i*20),bc=12+(i%2),fs=11+(i*3),pi2=(i%5)+1,ay=(i%8)+1;egret_breeding(lc,bl,bw,bc,fs,pi2,ay);}
ps("\nEgret health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=68+(i*6),bw=390+(i*40),bc=9+(i*3),fs=9+(i*4),pi2=(i%10)+1,ay=(i%7)+1;egret_health(lc,bl,bw,bc,fs,pi2,ay);}
ps("\nEgret market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=78+(i*3),bw=460+(i*15),bc=13+(i%2),fs=15+(i*2),pi2=(i%4)+1,ay=(i%6)+1;egret_market(lc,bl,bw,bc,fs,pi2,ay);}
ps("\n");egret_report();egret_state();ps("\n=== Demo Complete ===\n");return 0;}
