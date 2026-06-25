/* robin_admin: Robin (Erithacus rubecula) European robin songbird (v1.0)
 * Robin garden, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, song_vol, breast_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,sg_vl,br_idx,age_yr,active;} rob_t;
typedef struct{int n_garden,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_sg,t_br;} rob_state_t;
static rob_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static rob_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rob_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int sv,int bi,int ay){if(*cnt>=mx)return -1;rob_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->sg_vl=sv;x->br_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ROBN] Robin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" sv=");pi(sv);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rob_init(void){if(init)return -1;st.n_garden=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_sg=0;st.t_br=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[ROBN] Robin initialized\n");return 0;}
int rob_garden(int lc,int bl,int bw,int wc,int sv,int bi,int ay){return add(apl,&st.n_garden,&st.t_ln,N,lc,bl,bw,wc,sv,bi,ay);}
int rob_feeding(int lc,int bl,int bw,int wc,int sv,int bi,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,sv,bi,ay);}
int rob_breeding(int lc,int bl,int bw,int wc,int sv,int bi,int ay){return add(abd,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,sv,bi,ay);}
int rob_health(int lc,int bl,int bw,int wc,int sv,int bi,int ay){return add(ahd,&st.n_health,&st.t_sg,N-6,lc,bl,bw,wc,sv,bi,ay);}
int rob_market(int lc,int bl,int bw,int wc,int sv,int bi,int ay){return add(amd,&st.n_mkt,&st.t_br,N-6,lc,bl,bw,wc,sv,bi,ay);}
void rob_report(void){ps("[ROBN] Garden: ");pi(st.n_garden);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Sg=");pi(st.t_sg);ps("\nMkt: ");pi(st.n_mkt);ps(" Br=");pi(st.t_br);ps("\n");}
void rob_state(void){ps("[ROBN] Garden=");pi(st.n_garden);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Robin Admin Demo ===\n\n");rob_init();
ps("Robin garden...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=13+(i*2),bw=16+(i*3),wc=7+(i*2),sv=35+(i*5),bi=(i%6)+1,ay=1+(i%8);rob_garden(lc,bl,bw,wc,sv,bi,ay);}
ps("\nRobin feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=14+(i*2),bw=17+(i*3),wc=7+(i*2),sv=37+(i*5),bi=(i%5)+2,ay=1+(i%7);rob_feeding(lc,bl,bw,wc,sv,bi,ay);}
ps("\nRobin breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=15+(i*2),bw=18+(i*3),wc=8+(i*2),sv=40+(i*5),bi=(i%4)+1,ay=2+(i%6);rob_breeding(lc,bl,bw,wc,sv,bi,ay);}
ps("\nRobin health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=12+(i*3),bw=15+(i*4),wc=6+(i*3),sv=33+(i*6),bi=(i%3)+3,ay=2+(i%5);rob_health(lc,bl,bw,wc,sv,bi,ay);}
ps("\nRobin market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=17+(i*2),bw=20+(i*3),wc=9+(i*2),sv=45+(i*5),bi=(i%6)+1,ay=3+(i%4);rob_market(lc,bl,bw,wc,sv,bi,ay);}
ps("\n");rob_report();rob_state();ps("\n=== Demo Complete ===\n");return 0;}
