/* bee_admin: Bee (Apis mellifera) western honeybee pollinating insect (v1.0)
 * Bee hive, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, fly_speed, stripe_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,st_idx,age_yr,active;} bee_t;
typedef struct{int n_hive,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_st;} bee_state_t;
static bee_t hvl[N],hvf[N-2],hvb[N-4],hvh[N-6],hvm[N-6]; static bee_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bee_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int si,int ay){if(*cnt>=mx)return -1;bee_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->st_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BEE] Bee ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bee_init(void){if(init)return -1;st.n_hive=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_st=0;for(int i=0;i<N;i++)hvl[i].active=0;for(int i=0;i<N-2;i++)hvf[i].active=0;for(int i=0;i<N-4;i++)hvb[i].active=0;for(int i=0;i<N-6;i++)hvh[i].active=0;for(int i=0;i<N-6;i++)hvm[i].active=0;init=1;ps("[BEE] Bee initialized\n");return 0;}
int bee_hive(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(hvl,&st.n_hive,&st.t_ln,N,lc,bl,bw,wc,fs,si,ay);}
int bee_feeding(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(hvf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,si,ay);}
int bee_breeding(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(hvb,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,si,ay);}
int bee_health(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(hvh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,si,ay);}
int bee_market(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(hvm,&st.n_mkt,&st.t_st,N-6,lc,bl,bw,wc,fs,si,ay);}
void bee_report(void){ps("[BEE] Hive: ");pi(st.n_hive);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void bee_state(void){ps("[BEE] Hive=");pi(st.n_hive);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bee Admin Demo ===\n\n");bee_init();
ps("Bee hive...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=1+(i%3),bw=50+(i*10),wc=1+(i%2),fs=10+(i*2),si=(i%5)+1,ay=(i%4)+1;bee_hive(lc,bl,bw,wc,fs,si,ay);}
ps("\nBee feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=1+(i%2),bw=55+(i*8),wc=1+(i%3),fs=12+(i*2),si=(i%4)+1,ay=(i%3)+1;bee_feeding(lc,bl,bw,wc,fs,si,ay);}
ps("\nBee breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=2+(i%2),bw=60+(i*6),wc=1+(i%2),fs=8+(i*3),si=(i%3)+1,ay=(i%3)+1;bee_breeding(lc,bl,bw,wc,fs,si,ay);}
ps("\nBee health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=1+(i%4),bw=48+(i*12),wc=1+(i%3),fs=6+(i*4),si=(i%6)+1,ay=(i%2)+1;bee_health(lc,bl,bw,wc,fs,si,ay);}
ps("\nBee market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=2+(i%2),bw=65+(i*5),wc=2+(i%2),fs=15+(i*2),si=(i%3)+1,ay=(i%2)+1;bee_market(lc,bl,bw,wc,fs,si,ay);}
ps("\n");bee_report();bee_state();ps("\n=== Demo Complete ===\n");return 0;}
