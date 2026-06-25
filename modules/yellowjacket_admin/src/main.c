/* yellowjacket_admin: Yellowjacket (Vespula vulgaris) social wasp (v1.0)
 * Yellowjacket nest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, fly_speed, stripe_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,st_idx,age_yr,active;} yljk_t;
typedef struct{int n_nest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_st;} yljk_state_t;
static yljk_t ypl[N],yfd[N-2],ybr[N-4],yhl[N-6],ymk[N-6]; static yljk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(yljk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int si,int ay){if(*cnt>=mx)return -1;yljk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->st_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[YLJK] Yellowjacket ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int yljk_init(void){if(init)return -1;st.n_nest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_st=0;for(int i=0;i<N;i++)ypl[i].active=0;for(int i=0;i<N-2;i++)yfd[i].active=0;for(int i=0;i<N-4;i++)ybr[i].active=0;for(int i=0;i<N-6;i++)yhl[i].active=0;for(int i=0;i<N-6;i++)ymk[i].active=0;init=1;ps("[YLJK] Yellowjacket initialized\n");return 0;}
int yljk_nest(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(ypl,&st.n_nest,&st.t_ln,N,lc,bl,bw,wc,fs,si,ay);}
int yljk_feeding(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(yfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,si,ay);}
int yljk_breeding(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(ybr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,si,ay);}
int yljk_health(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(yhl,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,si,ay);}
int yljk_market(int lc,int bl,int bw,int wc,int fs,int si,int ay){return add(ymk,&st.n_mkt,&st.t_st,N-6,lc,bl,bw,wc,fs,si,ay);}
void yljk_report(void){ps("[YLJK] Nest: ");pi(st.n_nest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void yljk_state(void){ps("[YLJK] Nest=");pi(st.n_nest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Yellowjacket Admin Demo ===\n\n");yljk_init();
ps("Yellowjacket nest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=12+(i%3),bw=50+(i*10),wc=4+(i%2),fs=20+(i*5),st=(i%6)+1,ay=(i%2)+1;yljk_nest(lc,bl,bw,wc,fs,st,ay);}
ps("\nYellowjacket feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=13+(i%2),bw=60+(i*8),wc=4+(i%3),fs=22+(i*4),st=(i%5)+1,ay=(i%2)+1;yljk_feeding(lc,bl,bw,wc,fs,st,ay);}
ps("\nYellowjacket breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=11+(i*2),bw=40+(i*12),wc=3+(i%4),fs=18+(i*6),st=(i%4)+1,ay=(i%2)+1;yljk_breeding(lc,bl,bw,wc,fs,st,ay);}
ps("\nYellowjacket health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i),bw=70+(i*6),wc=5+(i%2),fs=25+(i*3),st=(i%7)+1,ay=(i%3)+2;yljk_health(lc,bl,bw,wc,fs,st,ay);}
ps("\nYellowjacket market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=15+(i%2),bw=80+(i*5),wc=5+(i%2),fs=15+(i*4),st=(i%3)+3,ay=(i%2)+2;yljk_market(lc,bl,bw,wc,fs,st,ay);}
ps("\n");yljk_report();yljk_state();ps("\n=== Demo Complete ===\n");return 0;}
