/* horse_admin: Horse (Equus caballus) domesticated equine mammal (v1.0)
 * Horse stable, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, leg_cm, run_speed, mane_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_cm,rn_sp,mn_idx,age_yr,active;} horse_t;
typedef struct{int n_stable,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_leg,t_rn,t_mn;} horse_state_t;
static horse_t stb[N],stf[N-2],stb2[N-4],sth[N-6],stm[N-6]; static horse_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(horse_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lc2,int rs,int mi,int ay){if(*cnt>=mx)return -1;horse_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_cm=lc2;x->rn_sp=rs;x->mn_idx=mi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HORSE] Horse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lc2=");pi(lc2);ps(" rs=");pi(rs);ps(" mi=");pi(mi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int horse_init(void){if(init)return -1;st.n_stable=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_rn=0;st.t_mn=0;for(int i=0;i<N;i++)stb[i].active=0;for(int i=0;i<N-2;i++)stf[i].active=0;for(int i=0;i<N-4;i++)stb2[i].active=0;for(int i=0;i<N-6;i++)sth[i].active=0;for(int i=0;i<N-6;i++)stm[i].active=0;init=1;ps("[HORSE] Horse initialized\n");return 0;}
int horse_stable(int lc,int bl,int bw,int lc2,int rs,int mi,int ay){return add(stb,&st.n_stable,&st.t_ln,N,lc,bl,bw,lc2,rs,mi,ay);}
int horse_feeding(int lc,int bl,int bw,int lc2,int rs,int mi,int ay){return add(stf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lc2,rs,mi,ay);}
int horse_breeding(int lc,int bl,int bw,int lc2,int rs,int mi,int ay){return add(stb2,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,lc2,rs,mi,ay);}
int horse_health(int lc,int bl,int bw,int lc2,int rs,int mi,int ay){return add(sth,&st.n_health,&st.t_rn,N-6,lc,bl,bw,lc2,rs,mi,ay);}
int horse_market(int lc,int bl,int bw,int lc2,int rs,int mi,int ay){return add(stm,&st.n_mkt,&st.t_mn,N-6,lc,bl,bw,lc2,rs,mi,ay);}
void horse_report(void){ps("[HORSE] Stable: ");pi(st.n_stable);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Mn=");pi(st.t_mn);ps("\n");}
void horse_state(void){ps("[HORSE] Stable=");pi(st.n_stable);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Horse Admin Demo ===\n\n");horse_init();
ps("Horse stable...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=220+(i*8),bw=450+(i*30),lgc=140+(i*6),rs=15+(i*4),mi=(i%10)+1,ay=(i%25)+1;horse_stable(lc,bl,bw,lgc,rs,mi,ay);}
ps("\nHorse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=225+(i*6),bw=460+(i*25),lgc=142+(i*5),rs=16+(i*3),mi=(i%9)+1,ay=(i%22)+1;horse_feeding(lc,bl,bw,lgc,rs,mi,ay);}
ps("\nHorse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=230+(i*5),bw=470+(i*20),lgc=144+(i*4),rs=14+(i*5),mi=(i%8)+1,ay=(i%18)+1;horse_breeding(lc,bl,bw,lgc,rs,mi,ay);}
ps("\nHorse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=218+(i*9),bw=440+(i*35),lgc=138+(i*7),rs=13+(i*6),mi=(i%11)+1,ay=(i%15)+1;horse_health(lc,bl,bw,lgc,rs,mi,ay);}
ps("\nHorse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=235+(i*4),bw=480+(i*18),lgc=146+(i*4),rs=17+(i*3),mi=(i%6)+1,ay=(i%12)+1;horse_market(lc,bl,bw,lgc,rs,mi,ay);}
ps("\n");horse_report();horse_state();ps("\n=== Demo Complete ===\n");return 0;}
