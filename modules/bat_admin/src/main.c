/* bat_admin: Bat (Myotis myotis) greater mouse-eared echolocating mammal (v1.0)
 * Bat cave, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, fly_speed, ear_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,er_idx,age_yr,active;} bat_t;
typedef struct{int n_cave,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_er;} bat_state_t;
static bat_t cvl[N],cvf[N-2],cvb[N-4],cvh[N-6],cvm[N-6]; static bat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bat_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int ei,int ay){if(*cnt>=mx)return -1;bat_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->er_idx=ei;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BAT] Bat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" ei=");pi(ei);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bat_init(void){if(init)return -1;st.n_cave=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_er=0;for(int i=0;i<N;i++)cvl[i].active=0;for(int i=0;i<N-2;i++)cvf[i].active=0;for(int i=0;i<N-4;i++)cvb[i].active=0;for(int i=0;i<N-6;i++)cvh[i].active=0;for(int i=0;i<N-6;i++)cvm[i].active=0;init=1;ps("[BAT] Bat initialized\n");return 0;}
int bat_cave(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(cvl,&st.n_cave,&st.t_ln,N,lc,bl,bw,wc,fs,ei,ay);}
int bat_feeding(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(cvf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,ei,ay);}
int bat_breeding(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(cvb,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,ei,ay);}
int bat_health(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(cvh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,ei,ay);}
int bat_market(int lc,int bl,int bw,int wc,int fs,int ei,int ay){return add(cvm,&st.n_mkt,&st.t_er,N-6,lc,bl,bw,wc,fs,ei,ay);}
void bat_report(void){ps("[BAT] Cave: ");pi(st.n_cave);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Er=");pi(st.t_er);ps("\n");}
void bat_state(void){ps("[BAT] Cave=");pi(st.n_cave);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bat Admin Demo ===\n\n");bat_init();
ps("Bat cave...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=5+(i*1),bw=10+(i*3),wc=25+(i*2),fs=20+(i*3),ei=(i%6)+1,ay=(i%8)+1;bat_cave(lc,bl,bw,wc,fs,ei,ay);}
ps("\nBat feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=6+(i*1),bw=12+(i*2),wc=27+(i*2),fs=22+(i*2),ei=(i%5)+1,ay=(i%7)+1;bat_feeding(lc,bl,bw,wc,fs,ei,ay);}
ps("\nBat breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=7+(i*1),bw=14+(i*2),wc=29+(i*2),fs=18+(i*4),ei=(i%4)+1,ay=(i%6)+1;bat_breeding(lc,bl,bw,wc,fs,ei,ay);}
ps("\nBat health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=4+(i*2),bw=9+(i*4),wc=24+(i*3),fs=15+(i*5),ei=(i%8)+1,ay=(i%5)+1;bat_health(lc,bl,bw,wc,fs,ei,ay);}
ps("\nBat market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=8+(i*1),bw=16+(i*2),wc=30+(i*2),fs=25+(i*2),ei=(i%3)+1,ay=(i%4)+1;bat_market(lc,bl,bw,wc,fs,ei,ay);}
ps("\n");bat_report();bat_state();ps("\n=== Demo Complete ===\n");return 0;}
