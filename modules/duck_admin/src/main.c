/* duck_admin: Duck (Anas platyrhynchos) mallard waterfowl (v1.0)
 * Duck pond, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, bill_cm, swim_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,bill_cm,sw_sp,pm_idx,age_yr,active;} duck_t;
typedef struct{int n_pond,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_bill,t_sw,t_pm;} duck_state_t;
static duck_t pnl[N],pnf[N-2],pnb[N-4],pnh[N-6],pnm[N-6]; static duck_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(duck_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int ss,int pi2,int ay){if(*cnt>=mx)return -1;duck_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->bill_cm=bc;x->sw_sp=ss;x->pm_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DUCK] Duck ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" ss=");pi(ss);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int duck_init(void){if(init)return -1;st.n_pond=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_bill=0;st.t_sw=0;st.t_pm=0;for(int i=0;i<N;i++)pnl[i].active=0;for(int i=0;i<N-2;i++)pnf[i].active=0;for(int i=0;i<N-4;i++)pnb[i].active=0;for(int i=0;i<N-6;i++)pnh[i].active=0;for(int i=0;i<N-6;i++)pnm[i].active=0;init=1;ps("[DUCK] Duck initialized\n");return 0;}
int duck_pond(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(pnl,&st.n_pond,&st.t_ln,N,lc,bl,bw,bc,ss,pi2,ay);}
int duck_feeding(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(pnf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,ss,pi2,ay);}
int duck_breeding(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(pnb,&st.n_breed,&st.t_bill,N-4,lc,bl,bw,bc,ss,pi2,ay);}
int duck_health(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(pnh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,bc,ss,pi2,ay);}
int duck_market(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(pnm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,bc,ss,pi2,ay);}
void duck_report(void){ps("[DUCK] Pond: ");pi(st.n_pond);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Bill=");pi(st.t_bill);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void duck_state(void){ps("[DUCK] Pond=");pi(st.n_pond);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Duck Admin Demo ===\n\n");duck_init();
ps("Duck pond...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=1+(i%2),bc=5+(i%2),ss=3+(i%2),pi2=(i%8)+1,ay=(i%8)+1;duck_pond(lc,bl,bw,bc,ss,pi2,ay);}
ps("\nDuck feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=1+(i%3),bc=5+(i%3),ss=3+(i%1),pi2=(i%6)+1,ay=(i%7)+1;duck_feeding(lc,bl,bw,bc,ss,pi2,ay);}
ps("\nDuck breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=44+(i*2),bw=2+(i%2),bc=6+(i%2),ss=2+(i*3),pi2=(i%5)+1,ay=(i%6)+1;duck_breeding(lc,bl,bw,bc,ss,pi2,ay);}
ps("\nDuck health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=1+(i%4),bc=4+(i%3),ss=2+(i*4),pi2=(i%10)+1,ay=(i%5)+1;duck_health(lc,bl,bw,bc,ss,pi2,ay);}
ps("\nDuck market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=45+(i*2),bw=2+(i%2),bc=6+(i%2),ss=4+(i*2),pi2=(i%4)+1,ay=(i%4)+1;duck_market(lc,bl,bw,bc,ss,pi2,ay);}
ps("\n");duck_report();duck_state();ps("\n=== Demo Complete ===\n");return 0;}
