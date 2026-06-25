/* nightingale_admin: Nightingale (Luscinia megarhynchos) common songbird (v1.0)
 * Nightingale thicket, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, fly_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,fl_sp,pm_idx,age_yr,active;} ntgl_t;
typedef struct{int n_thicket,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_fl,t_pm;} ntgl_state_t;
static ntgl_t thk[N],tkf[N-2],tkb[N-4],tkh[N-6],tkm[N-6]; static ntgl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ntgl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int fs,int pm,int ay){if(*cnt>=mx)return -1;ntgl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->fl_sp=fs;x->pm_idx=pm;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[NTGL] Nightingale ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" fs=");pi(fs);ps(" pm=");pi(pm);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int nightingale_init(void){if(init)return -1;st.n_thicket=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_fl=0;st.t_pm=0;for(int i=0;i<N;i++)thk[i].active=0;for(int i=0;i<N-2;i++)tkf[i].active=0;for(int i=0;i<N-4;i++)tkb[i].active=0;for(int i=0;i<N-6;i++)tkh[i].active=0;for(int i=0;i<N-6;i++)tkm[i].active=0;init=1;ps("[NTGL] Nightingale initialized\n");return 0;}
int nightingale_thicket(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(thk,&st.n_thicket,&st.t_ln,N,lc,bl,bw,bc,fs,pm,ay);}
int nightingale_feeding(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(tkf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,fs,pm,ay);}
int nightingale_breeding(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(tkb,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,fs,pm,ay);}
int nightingale_health(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(tkh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,bc,fs,pm,ay);}
int nightingale_market(int lc,int bl,int bw,int bc,int fs,int pm,int ay){return add(tkm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,bc,fs,pm,ay);}
void nightingale_report(void){ps("[NTGL] Thicket: ");pi(st.n_thicket);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void nightingale_state(void){ps("[NTGL] Thicket=");pi(st.n_thicket);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Nightingale Admin Demo ===\n\n");nightingale_init();
ps("Nightingale thicket...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=16+(i*1),bw=25+(i*3),bc=2+(i*1),fs=10+(i*3),pm=(i%8)+1,ay=(i%8)+1;nightingale_thicket(lc,bl,bw,bc,fs,pm,ay);}
ps("\nNightingale feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=17+(i*1),bw=26+(i*2),bc=2+(i*1),fs=11+(i*2),pm=(i%7)+1,ay=(i%7)+1;nightingale_feeding(lc,bl,bw,bc,fs,pm,ay);}
ps("\nNightingale breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=18+(i*1),bw=27+(i*2),bc=2+(i*1),fs=9+(i*4),pm=(i%6)+1,ay=(i%6)+1;nightingale_breeding(lc,bl,bw,bc,fs,pm,ay);}
ps("\nNightingale health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=15+(i*2),bw=24+(i*3),bc=1+(i*1),fs=8+(i*5),pm=(i%9)+1,ay=(i%5)+1;nightingale_health(lc,bl,bw,bc,fs,pm,ay);}
ps("\nNightingale market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=19+(i*1),bw=28+(i*2),bc=2+(i*1),fs=12+(i*2),pm=(i%5)+1,ay=(i%4)+1;nightingale_market(lc,bl,bw,bc,fs,pm,ay);}
ps("\n");nightingale_report();nightingale_state();ps("\n=== Demo Complete ===\n");return 0;}
