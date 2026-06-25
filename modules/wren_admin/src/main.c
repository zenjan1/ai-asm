/* wren_admin: Wren (Troglodytes troglodytes) small songbird (v1.0)
 * Wren hedgerow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, song_vol, plumage_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,sg_vl,pl_idx,age_yr,active;} wren_t;
typedef struct{int n_hedge,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_sg,t_pl;} wren_state_t;
static wren_t wpl[N],wfd[N-2],wbr[N-4],whl[N-6],wmk[N-6]; static wren_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wren_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int sv,int pi2,int ay){if(*cnt>=mx)return -1;wren_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->sg_vl=sv;x->pl_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WREN] Wren ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" sv=");pi(sv);ps(" pl=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int wren_init(void){if(init)return -1;st.n_hedge=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_sg=0;st.t_pl=0;for(int i=0;i<N;i++)wpl[i].active=0;for(int i=0;i<N-2;i++)wfd[i].active=0;for(int i=0;i<N-4;i++)wbr[i].active=0;for(int i=0;i<N-6;i++)whl[i].active=0;for(int i=0;i<N-6;i++)wmk[i].active=0;init=1;ps("[WREN] Wren initialized\n");return 0;}
int wren_hedgerow(int lc,int bl,int bw,int wc,int sv,int pi2,int ay){return add(wpl,&st.n_hedge,&st.t_ln,N,lc,bl,bw,wc,sv,pi2,ay);}
int wren_feeding(int lc,int bl,int bw,int wc,int sv,int pi2,int ay){return add(wfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,sv,pi2,ay);}
int wren_breeding(int lc,int bl,int bw,int wc,int sv,int pi2,int ay){return add(wbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,sv,pi2,ay);}
int wren_health(int lc,int bl,int bw,int wc,int sv,int pi2,int ay){return add(whl,&st.n_health,&st.t_sg,N-6,lc,bl,bw,wc,sv,pi2,ay);}
int wren_market(int lc,int bl,int bw,int wc,int sv,int pi2,int ay){return add(wmk,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,wc,sv,pi2,ay);}
void wren_report(void){ps("[WREN] Hedge: ");pi(st.n_hedge);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Sg=");pi(st.t_sg);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void wren_state(void){ps("[WREN] Hedge=");pi(st.n_hedge);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Wren Admin Demo ===\n\n");wren_init();
ps("Wren hedgerow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i%3),bw=8+(i*2),wc=5+(i%2),sv=50+(i*8),pl=(i%6)+1,ay=(i%3)+1;wren_hedgerow(lc,bl,bw,wc,sv,pl,ay);}
ps("\nWren feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=11+(i%2),bw=10+(i*2),wc=5+(i%3),sv=55+(i*6),pl=(i%5)+1,ay=(i%3)+1;wren_feeding(lc,bl,bw,wc,sv,pl,ay);}
ps("\nWren breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=9+(i*2),bw=7+(i*3),wc=4+(i%4),sv=45+(i*10),pl=(i%4)+1,ay=(i%2)+1;wren_breeding(lc,bl,bw,wc,sv,pl,ay);}
ps("\nWren health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=12+(i),bw=12+(i),wc=6+(i%2),sv=60+(i*5),pl=(i%7)+1,ay=(i%4)+2;wren_health(lc,bl,bw,wc,sv,pl,ay);}
ps("\nWren market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i%2),bw=14+(i%2),wc=7+(i%2),sv=40+(i*7),pl=(i%3)+3,ay=(i%2)+2;wren_market(lc,bl,bw,wc,sv,pl,ay);}
ps("\n");wren_report();wren_state();ps("\n=== Demo Complete ===\n");return 0;}
