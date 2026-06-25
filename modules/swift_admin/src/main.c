/* swift_admin: Swift (Apus apus) fast flying bird (v1.0)
 * Swift skyline, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, fly_speed, plg_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,pg_idx,age_yr,active;} swft_t;
typedef struct{int n_sky,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_pg;} swft_state_t;
static swft_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static swft_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(swft_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int pi2,int ay){if(*cnt>=mx)return -1;swft_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->pg_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SWFT] Swift ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" pg=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int swft_init(void){if(init)return -1;st.n_sky=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_pg=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SWFT] Swift initialized\n");return 0;}
int swft_skyline(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(spl,&st.n_sky,&st.t_ln,N,lc,bl,bw,wc,fs,pi2,ay);}
int swft_feeding(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,pi2,ay);}
int swft_breeding(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(sbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,pi2,ay);}
int swft_health(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(shl,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,pi2,ay);}
int swft_market(int lc,int bl,int bw,int wc,int fs,int pi2,int ay){return add(smk,&st.n_mkt,&st.t_pg,N-6,lc,bl,bw,wc,fs,pi2,ay);}
void swft_report(void){ps("[SWFT] Skyline: ");pi(st.n_sky);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pg=");pi(st.t_pg);ps("\n");}
void swft_state(void){ps("[SWFT] Sky=");pi(st.n_sky);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Swift Admin Demo ===\n\n");swft_init();
ps("Swift skyline...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=15+(i%3),bw=30+(i*5),wc=12+(i*2),fs=60+(i*10),pg=(i%6)+1,ay=(i%3)+1;swft_skyline(lc,bl,bw,wc,fs,pg,ay);}
ps("\nSwift feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=16+(i%2),bw=35+(i*4),wc=13+(i%2),fs=65+(i*8),pg=(i%5)+1,ay=(i%3)+1;swft_feeding(lc,bl,bw,wc,fs,pg,ay);}
ps("\nSwift breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=14+(i*2),bw=25+(i*6),wc=11+(i%3),fs=55+(i*12),pg=(i%4)+1,ay=(i%2)+1;swft_breeding(lc,bl,bw,wc,fs,pg,ay);}
ps("\nSwift health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=17+(i),bw=40+(i*3),wc=14+(i%2),fs=70+(i*6),pg=(i%7)+1,ay=(i%4)+2;swft_health(lc,bl,bw,wc,fs,pg,ay);}
ps("\nSwift market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=18+(i%2),bw=45+(i*2),wc=15+(i%2),fs=50+(i*8),pg=(i%3)+3,ay=(i%2)+2;swft_market(lc,bl,bw,wc,fs,pg,ay);}
ps("\n");swft_report();swft_state();ps("\n=== Demo Complete ===\n");return 0;}
