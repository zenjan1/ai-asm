/* stork_admin: Stork (Ciconia ciconia) large wading bird (v1.0)
 * Stork wetland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_cm, fly_speed, bill_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,bl_idx,age_yr,active;} stork_t;
typedef struct{int n_wet,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_bl;} stork_state_t;
static stork_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static stork_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(stork_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int bi,int ay){if(*cnt>=mx)return -1;stork_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->bl_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[STORK] Stork ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int stork_init(void){if(init)return -1;st.n_wet=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_bl=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[STORK] Stork initialized\n");return 0;}
int stork_wetland(int lc,int bl,int bw,int wc,int fs,int bi,int ay){return add(spl,&st.n_wet,&st.t_ln,N,lc,bl,bw,wc,fs,bi,ay);}
int stork_feeding(int lc,int bl,int bw,int wc,int fs,int bi,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,bi,ay);}
int stork_breeding(int lc,int bl,int bw,int wc,int fs,int bi,int ay){return add(sbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,bi,ay);}
int stork_health(int lc,int bl,int bw,int wc,int fs,int bi,int ay){return add(shl,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,bi,ay);}
int stork_market(int lc,int bl,int bw,int wc,int fs,int bi,int ay){return add(smk,&st.n_mkt,&st.t_bl,N-6,lc,bl,bw,wc,fs,bi,ay);}
void stork_report(void){ps("[STORK] Wetland: ");pi(st.n_wet);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Bl=");pi(st.t_bl);ps("\n");}
void stork_state(void){ps("[STORK] Wet=");pi(st.n_wet);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Stork Admin Demo ===\n\n");stork_init();
ps("Stork wetland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*8),bw=3+(i*2),wc=150+(i*10),fs=30+(i*5),bi=(i%6)+1,ay=(i%5)+1;stork_wetland(lc,bl,bw,wc,fs,bi,ay);}
ps("\nStork feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=110+(i*6),bw=4+(i),wc=160+(i*8),fs=35+(i*4),bi=(i%5)+1,ay=(i%4)+1;stork_feeding(lc,bl,bw,wc,fs,bi,ay);}
ps("\nStork breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=90+(i*10),bw=2+(i*3),wc=140+(i*12),fs=25+(i*6),bi=(i%4)+1,ay=(i%3)+1;stork_breeding(lc,bl,bw,wc,fs,bi,ay);}
ps("\nStork health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=120+(i*5),bw=5+(i),wc=170+(i*6),fs=40+(i*3),bi=(i%7)+1,ay=(i%6)+2;stork_health(lc,bl,bw,wc,fs,bi,ay);}
ps("\nStork market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=130+(i*4),bw=6+(i%3),wc=180+(i*5),fs=20+(i*5),bi=(i%3)+3,ay=(i%4)+3;stork_market(lc,bl,bw,wc,fs,bi,ay);}
ps("\n");stork_report();stork_state();ps("\n=== Demo Complete ===\n");return 0;}
