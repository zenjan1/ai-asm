/* drone_admin: Drone (Apis mellifera) male bee apiculture management (v1.0)
 * Drone housing, feeding, mating, health, market
 * Features: body_len_mm, body_wt_mg, wing_mm, eye_idx, flight_spd, age_day
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wng_mm,eye_idx,fly_sp,age_dy,active;} dron_t;
typedef struct{int n_house,n_feed,n_mate,n_hlth,n_mkt,t_ln,t_wt,t_wg,t_ey,t_fl;} dron_state_t;
static dron_t dhs[N],dfd[N-2],dmt[N-4],dhl[N-6],dmk[N-6]; static dron_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dron_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wm,int ei,int fs,int ad){if(*cnt>=mx)return -1;dron_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wng_mm=wm;x->eye_idx=ei;x->fly_sp=fs;x->age_dy=ad;x->active=1;*sum+=bl;(*cnt)++;ps("[DRON] Drone ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wm=");pi(wm);ps(" ei=");pi(ei);ps(" fs=");pi(fs);ps(" ad=");pi(ad);ps("\n");return *cnt-1;}
int dron_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_mate=0;st.n_hlth=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wg=0;st.t_ey=0;st.t_fl=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dmt[i].active=0;for(int i=0;i<N-6;i++)dhl[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DRON] Drone initialized\n");return 0;}
int dron_housing(int lc,int bl,int bw,int wm,int ei,int fs,int ad){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,wm,ei,fs,ad);}
int dron_feeding(int lc,int bl,int bw,int wm,int ei,int fs,int ad){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wm,ei,fs,ad);}
int dron_mating(int lc,int bl,int bw,int wm,int ei,int fs,int ad){return add(dmt,&st.n_mate,&st.t_wg,N-4,lc,bl,bw,wm,ei,fs,ad);}
int dron_health(int lc,int bl,int bw,int wm,int ei,int fs,int ad){return add(dhl,&st.n_hlth,&st.t_ey,N-6,lc,bl,bw,wm,ei,fs,ad);}
int dron_market(int lc,int bl,int bw,int wm,int ei,int fs,int ad){return add(dmk,&st.n_mkt,&st.t_fl,N-6,lc,bl,bw,wm,ei,fs,ad);}
void dron_report(void){ps("[DRON] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nMate: ");pi(st.n_mate);ps(" Wg=");pi(st.t_wg);ps("\nHlth: ");pi(st.n_hlth);ps(" Ey=");pi(st.t_ey);ps("\nMkt: ");pi(st.n_mkt);ps(" Fl=");pi(st.t_fl);ps("\n");}
void dron_state(void){ps("[DRON] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Mate=");pi(st.n_mate);ps(" Hlth=");pi(st.n_hlth);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Drone Admin Demo ===\n\n");dron_init();
/* 1=hive 2=apiary 3=field 4=meadow 5=market */
ps("Drone housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=15+(i%3),bw=250+(i*20),wm=10+(i%3),ei=(i%6)+1,fs=20+(i*3),ad=5+(i*3);dron_housing(lc,bl,bw,wm,ei,fs,ad);}
ps("\nDrone feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=16+(i%2),bw=270+(i*15),wm=11+(i%2),ei=(i%5)+1,fs=22+(i*2),ad=8+(i*2);dron_feeding(lc,bl,bw,wm,ei,fs,ad);}
ps("\nDrone mating...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=17+(i%2),bw=290+(i*12),wm=12+(i%2),ei=(i%4)+1,fs=25+(i*2),ad=10+(i*2);dron_mating(lc,bl,bw,wm,ei,fs,ad);}
ps("\nDrone health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i%3),bw=230+(i*25),wm=9+(i%4),ei=(i%7)+1,fs=18+(i*4),ad=3+(i*4);dron_health(lc,bl,bw,wm,ei,fs,ad);}
ps("\nDrone market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=18+(i%2),bw=310+(i*10),wm=13+(i%2),ei=(i%3)+4,fs=28+(i*2),ad=12+(i*2);dron_market(lc,bl,bw,wm,ei,fs,ad);}
ps("\n");dron_report();dron_state();ps("\n=== Demo Complete ===\n");return 0;}
