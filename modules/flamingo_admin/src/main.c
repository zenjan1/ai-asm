/* flamingo_admin: Flamingo (Phoenicopterus roseus) greater pink wading bird (v1.0)
 * Flamingo lagoon, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, neck_cm, fly_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,nck_cm,fl_sp,pm_idx,age_yr,active;} flmg_t;
typedef struct{int n_lagoon,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_nck,t_fl,t_pm;} flmg_state_t;
static flmg_t lgn[N],lgf[N-2],lgb[N-4],lgh[N-6],lgm[N-6]; static flmg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(flmg_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nc,int fs,int pm,int ay){if(*cnt>=mx)return -1;flmg_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->nck_cm=nc;x->fl_sp=fs;x->pm_idx=pm;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FLMG] Flamingo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nc=");pi(nc);ps(" fs=");pi(fs);ps(" pm=");pi(pm);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int flamingo_init(void){if(init)return -1;st.n_lagoon=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_nck=0;st.t_fl=0;st.t_pm=0;for(int i=0;i<N;i++)lgn[i].active=0;for(int i=0;i<N-2;i++)lgf[i].active=0;for(int i=0;i<N-4;i++)lgb[i].active=0;for(int i=0;i<N-6;i++)lgh[i].active=0;for(int i=0;i<N-6;i++)lgm[i].active=0;init=1;ps("[FLMG] Flamingo initialized\n");return 0;}
int flamingo_lagoon(int lc,int bl,int bw,int nc,int fs,int pm,int ay){return add(lgn,&st.n_lagoon,&st.t_ln,N,lc,bl,bw,nc,fs,pm,ay);}
int flamingo_feeding(int lc,int bl,int bw,int nc,int fs,int pm,int ay){return add(lgf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nc,fs,pm,ay);}
int flamingo_breeding(int lc,int bl,int bw,int nc,int fs,int pm,int ay){return add(lgb,&st.n_breed,&st.t_nck,N-4,lc,bl,bw,nc,fs,pm,ay);}
int flamingo_health(int lc,int bl,int bw,int nc,int fs,int pm,int ay){return add(lgh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,nc,fs,pm,ay);}
int flamingo_market(int lc,int bl,int bw,int nc,int fs,int pm,int ay){return add(lgm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,nc,fs,pm,ay);}
void flamingo_report(void){ps("[FLMG] Lagoon: ");pi(st.n_lagoon);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Nck=");pi(st.t_nck);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void flamingo_state(void){ps("[FLMG] Lagoon=");pi(st.n_lagoon);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Flamingo Admin Demo ===\n\n");flamingo_init();
ps("Flamingo lagoon...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*8),bw=2500+(i*200),nc=70+(i*6),fs=20+(i*5),pm=(i%10)+1,ay=(i%20)+1;flamingo_lagoon(lc,bl,bw,nc,fs,pm,ay);}
ps("\nFlamingo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=125+(i*6),bw=2600+(i*150),nc=72+(i*5),fs=22+(i*4),pm=(i%9)+1,ay=(i%18)+1;flamingo_feeding(lc,bl,bw,nc,fs,pm,ay);}
ps("\nFlamingo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=130+(i*5),bw=2700+(i*120),nc=74+(i*4),fs=18+(i*6),pm=(i%8)+1,ay=(i%15)+1;flamingo_breeding(lc,bl,bw,nc,fs,pm,ay);}
ps("\nFlamingo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=118+(i*9),bw=2400+(i*250),nc=68+(i*7),fs=16+(i*7),pm=(i%11)+1,ay=(i%12)+1;flamingo_health(lc,bl,bw,nc,fs,pm,ay);}
ps("\nFlamingo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=135+(i*4),bw=2800+(i*100),nc=76+(i*3),fs=24+(i*3),pm=(i%6)+1,ay=(i%10)+1;flamingo_market(lc,bl,bw,nc,fs,pm,ay);}
ps("\n");flamingo_report();flamingo_state();ps("\n=== Demo Complete ===\n");return 0;}
