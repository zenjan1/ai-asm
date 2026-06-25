/* walrus_admin: Walrus (Odobenus rosmarus) arctic marine mammal (v1.0)
 * Walrus arctic, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tusk_cm, swim_speed, whisker_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tusk_cm,sw_sp,wh_idx,age_yr,active;} wlr_t;
typedef struct{int n_arctic,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tusk,t_sw,t_wh;} wlr_state_t;
static wlr_t wpl[N],wfd[N-2],wbr[N-4],whl[N-6],wmk[N-6]; static wlr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wlr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int wi,int ay){if(*cnt>=mx)return -1;wlr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tusk_cm=tc;x->sw_sp=ss;x->wh_idx=wi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WLR] Walrus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" wi=");pi(wi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int wlr_init(void){if(init)return -1;st.n_arctic=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tusk=0;st.t_sw=0;st.t_wh=0;for(int i=0;i<N;i++)wpl[i].active=0;for(int i=0;i<N-2;i++)wfd[i].active=0;for(int i=0;i<N-4;i++)wbr[i].active=0;for(int i=0;i<N-6;i++)whl[i].active=0;for(int i=0;i<N-6;i++)wmk[i].active=0;init=1;ps("[WLR] Walrus initialized\n");return 0;}
int wlr_arctic(int lc,int bl,int bw,int tc,int ss,int wi,int ay){return add(wpl,&st.n_arctic,&st.t_ln,N,lc,bl,bw,tc,ss,wi,ay);}
int wlr_feeding(int lc,int bl,int bw,int tc,int ss,int wi,int ay){return add(wfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,wi,ay);}
int wlr_breeding(int lc,int bl,int bw,int tc,int ss,int wi,int ay){return add(wbr,&st.n_breed,&st.t_tusk,N-4,lc,bl,bw,tc,ss,wi,ay);}
int wlr_health(int lc,int bl,int bw,int tc,int ss,int wi,int ay){return add(whl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,tc,ss,wi,ay);}
int wlr_market(int lc,int bl,int bw,int tc,int ss,int wi,int ay){return add(wmk,&st.n_mkt,&st.t_wh,N-6,lc,bl,bw,tc,ss,wi,ay);}
void wlr_report(void){ps("[WLR] Arctic: ");pi(st.n_arctic);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tusk=");pi(st.t_tusk);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Wh=");pi(st.t_wh);ps("\n");}
void wlr_state(void){ps("[WLR] Arctic=");pi(st.n_arctic);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Walrus Admin Demo ===\n\n");wlr_init();
ps("Walrus arctic...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=250+(i*20),bw=800+(i*100),tc=50+(i*5),ss=5+(i*2),wi=(i%6)+1,ay=(i%5)+5;wlr_arctic(lc,bl,bw,tc,ss,wi,ay);}
ps("\nWalrus feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=270+(i*15),bw=900+(i*80),tc=55+(i*4),ss=6+(i*2),wi=(i%5)+1,ay=(i%4)+4;wlr_feeding(lc,bl,bw,tc,ss,wi,ay);}
ps("\nWalrus breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=230+(i*25),bw=700+(i*120),tc=45+(i*6),ss=4+(i*3),wi=(i%4)+1,ay=(i%3)+3;wlr_breeding(lc,bl,bw,tc,ss,wi,ay);}
ps("\nWalrus health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=290+(i*10),bw=1000+(i*60),tc=60+(i*3),ss=7+(i),wi=(i%7)+1,ay=(i%6)+6;wlr_health(lc,bl,bw,tc,ss,wi,ay);}
ps("\nWalrus market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=310+(i*8),bw=1100+(i*50),tc=65+(i*2),ss=3+(i*2),wi=(i%3)+3,ay=(i%4)+8;wlr_market(lc,bl,bw,tc,ss,wi,ay);}
ps("\n");wlr_report();wlr_state();ps("\n=== Demo Complete ===\n");return 0;}
