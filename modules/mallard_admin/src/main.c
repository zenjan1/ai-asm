/* mallard_admin: Mallard (Anas platyrhynchos) mallard dabbling duck (v1.0)
 * Mallard pond, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, bill_cm, quack_vol, plumage_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,bill_cm,qu_vl,pl_idx,age_yr,active;} mall_t;
typedef struct{int n_pond,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_bill,t_qu,t_pl;} mall_state_t;
static mall_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static mall_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mall_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bm,int qv,int pi2,int ay){if(*cnt>=mx)return -1;mall_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->bill_cm=bm;x->qu_vl=qv;x->pl_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MLLD] Mallard ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bm=");pi(bm);ps(" qv=");pi(qv);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mall_init(void){if(init)return -1;st.n_pond=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_bill=0;st.t_qu=0;st.t_pl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MLLD] Mallard initialized\n");return 0;}
int mall_pond(int lc,int bl,int bw,int bm,int qv,int pi2,int ay){return add(apl,&st.n_pond,&st.t_ln,N,lc,bl,bw,bm,qv,pi2,ay);}
int mall_feeding(int lc,int bl,int bw,int bm,int qv,int pi2,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bm,qv,pi2,ay);}
int mall_breeding(int lc,int bl,int bw,int bm,int qv,int pi2,int ay){return add(abd,&st.n_breed,&st.t_bill,N-4,lc,bl,bw,bm,qv,pi2,ay);}
int mall_health(int lc,int bl,int bw,int bm,int qv,int pi2,int ay){return add(ahd,&st.n_health,&st.t_qu,N-6,lc,bl,bw,bm,qv,pi2,ay);}
int mall_market(int lc,int bl,int bw,int bm,int qv,int pi2,int ay){return add(amd,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,bm,qv,pi2,ay);}
void mall_report(void){ps("[MLLD] Pond: ");pi(st.n_pond);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Bill=");pi(st.t_bill);ps("\nHealth: ");pi(st.n_health);ps(" Qu=");pi(st.t_qu);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void mall_state(void){ps("[MLLD] Pond=");pi(st.n_pond);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Mallard Admin Demo ===\n\n");mall_init();
ps("Mallard pond...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=900+(i*50),bm=4+(i*2),qv=50+(i*5),pi2=(i%6)+1,ay=1+(i%10);mall_pond(lc,bl,bw,bm,qv,pi2,ay);}
ps("\nMallard feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*3),bw=950+(i*50),bm=5+(i*2),qv=52+(i*5),pi2=(i%5)+2,ay=2+(i%8);mall_feeding(lc,bl,bw,bm,qv,pi2,ay);}
ps("\nMallard breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*3),bw=1000+(i*50),bm=6+(i*2),qv=55+(i*5),pi2=(i%4)+1,ay=3+(i%6);mall_breeding(lc,bl,bw,bm,qv,pi2,ay);}
ps("\nMallard health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=880+(i*60),bm=3+(i*3),qv=48+(i*6),pi2=(i%3)+3,ay=4+(i%5);mall_health(lc,bl,bw,bm,qv,pi2,ay);}
ps("\nMallard market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=50+(i*3),bw=1100+(i*50),bm=7+(i*2),qv=60+(i*5),pi2=(i%6)+1,ay=5+(i%4);mall_market(lc,bl,bw,bm,qv,pi2,ay);}
ps("\n");mall_report();mall_state();ps("\n=== Demo Complete ===\n");return 0;}
