/* starfish_admin: Starfish (Asterias rubens) echinoderm (v1.0)
 * Starfish tidepool, feeding, breeding, health, market
 * Features: arm_span_cm, body_wt_g, arm_ct, move_speed, color_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,arm_sp,bdy_wt,arm_ct,mv_sp,cl_idx,age_yr,active;} star_t;
typedef struct{int n_tide,n_feed,n_breed,n_health,n_mkt,t_as,t_wt,t_ac,t_mv,t_cl;} star_state_t;
static star_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static star_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(star_t*a,int*cnt,int*sum,int mx,int lc,int as2,int bw,int ac,int ms,int ci,int ay){if(*cnt>=mx)return -1;star_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->arm_sp=as2;x->bdy_wt=bw;x->arm_ct=ac;x->mv_sp=ms;x->cl_idx=ci;x->age_yr=ay;x->active=1;*sum+=as2;(*cnt)++;ps("[STAR] Starfish ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" as=");pi(as2);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" ms=");pi(ms);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int star_init(void){if(init)return -1;st.n_tide=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_as=0;st.t_wt=0;st.t_ac=0;st.t_mv=0;st.t_cl=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[STAR] Starfish initialized\n");return 0;}
int star_tidepool(int lc,int as2,int bw,int ac,int ms,int ci,int ay){return add(spl,&st.n_tide,&st.t_as,N,lc,as2,bw,ac,ms,ci,ay);}
int star_feeding(int lc,int as2,int bw,int ac,int ms,int ci,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,as2,bw,ac,ms,ci,ay);}
int star_breeding(int lc,int as2,int bw,int ac,int ms,int ci,int ay){return add(sbr,&st.n_breed,&st.t_ac,N-4,lc,as2,bw,ac,ms,ci,ay);}
int star_health(int lc,int as2,int bw,int ac,int ms,int ci,int ay){return add(shl,&st.n_health,&st.t_mv,N-6,lc,as2,bw,ac,ms,ci,ay);}
int star_market(int lc,int as2,int bw,int ac,int ms,int ci,int ay){return add(smk,&st.n_mkt,&st.t_cl,N-6,lc,as2,bw,ac,ms,ci,ay);}
void star_report(void){ps("[STAR] Tide: ");pi(st.n_tide);ps(" As=");pi(st.t_as);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ac=");pi(st.t_ac);ps("\nHealth: ");pi(st.n_health);ps(" Mv=");pi(st.t_mv);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void star_state(void){ps("[STAR] Tide=");pi(st.n_tide);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Starfish Admin Demo ===\n\n");star_init();
ps("Starfish tidepool...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,as=15+(i*3),bw=30+(i*10),ac=5+(i%3),ms=0+(i%2),ci=(i%5)+1,ay=(i%3)+1;star_tidepool(lc,as,bw,ac,ms,ci,ay);}
ps("\nStarfish feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,as=18+(i*2),bw=40+(i*8),ac=5+(i%2),ms=1+(i%2),ci=(i%4)+1,ay=(i%3)+1;star_feeding(lc,as,bw,ac,ms,ci,ay);}
ps("\nStarfish breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,as=12+(i*4),bw=25+(i*12),ac=5+(i%3),ms=0+(i%3),ci=(i%6)+1,ay=(i%2)+1;star_breeding(lc,as,bw,ac,ms,ci,ay);}
ps("\nStarfish health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,as=20+(i*2),bw=50+(i*6),ac=5+(i%2),ms=0+(i%2),ci=(i%7)+1,ay=(i%4)+2;star_health(lc,as,bw,ac,ms,ci,ay);}
ps("\nStarfish market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,as=25+(i),bw=60+(i*5),ac=5+(i%3),ms=0+(i%3),ci=(i%3)+3,ay=(i%2)+2;star_market(lc,as,bw,ac,ms,ci,ay);}
ps("\n");star_report();star_state();ps("\n=== Demo Complete ===\n");return 0;}
