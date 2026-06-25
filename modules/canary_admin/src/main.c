/* canary_admin: Canary (Serinus canaria) domestic songbird (v1.0)
 * Canary cage, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, fly_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,pm_idx,age_yr,active;} cnry_t;
typedef struct{int n_cage,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_pm;} cnry_state_t;
static cnry_t cgl[N],cgf[N-2],cgb[N-4],cgh[N-6],cgm[N-6]; static cnry_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cnry_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int pi2,int ay){if(*cnt>=mx)return -1;cnry_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->pm_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CNRY] Canary ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int canary_init(void){if(init)return -1;st.n_cage=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_pm=0;for(int i=0;i<N;i++)cgl[i].active=0;for(int i=0;i<N-2;i++)cgf[i].active=0;for(int i=0;i<N-4;i++)cgb[i].active=0;for(int i=0;i<N-6;i++)cgh[i].active=0;for(int i=0;i<N-6;i++)cgm[i].active=0;init=1;ps("[CNRY] Canary initialized\n");return 0;}
int canary_cage(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(cgl,&st.n_cage,&st.t_ln,N,lc,bl,bw,wc,fs,pi2,ay);}
int canary_feeding(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(cgf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,pi2,ay);}
int canary_breeding(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(cgb,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,pi2,ay);}
int canary_health(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(cgh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,pi2,ay);}
int canary_market(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(cgm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,wc,fs,pi2,ay);}
void canary_report(void){ps("[CNRY] Cage: ");pi(st.n_cage);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void canary_state(void){ps("[CNRY] Cage=");pi(st.n_cage);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Canary Admin Demo ===\n\n");canary_init();
ps("Canary cage...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*1),bw=15+(i*3),wc=5+(i%3),fs=8+(i*2),pi2=(i%6)+1,ay=(i%8)+1;canary_cage(lc,bl,bw,wc,fs,pi2,ay);}
ps("\nCanary feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*1),bw=17+(i*2),wc=6+(i%2),fs=9+(i*1),pi2=(i%5)+1,ay=(i%7)+1;canary_feeding(lc,bl,bw,wc,fs,pi2,ay);}
ps("\nCanary breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=19+(i*2),wc=5+(i%3),fs=7+(i*3),pi2=(i%4)+1,ay=(i%6)+1;canary_breeding(lc,bl,bw,wc,fs,pi2,ay);}
ps("\nCanary health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*2),bw=14+(i*4),wc=4+(i%4),fs=6+(i*4),pi2=(i%8)+1,ay=(i%5)+1;canary_health(lc,bl,bw,wc,fs,pi2,ay);}
ps("\nCanary market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=20+(i*2),wc=7+(i%2),fs=10+(i*2),pi2=(i%3)+1,ay=(i%4)+1;canary_market(lc,bl,bw,wc,fs,pi2,ay);}
ps("\n");canary_report();canary_state();ps("\n=== Demo Complete ===\n");return 0;}
