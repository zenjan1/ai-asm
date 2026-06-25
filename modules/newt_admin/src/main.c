/* newt_admin: Newt (Triturus vulgaris) smooth small amphibian (v1.0)
 * Newt pond, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, swim_speed, skin_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,sw_sp,sk_idx,age_yr,active;} newt_t;
typedef struct{int n_pond,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_sw,t_sk;} newt_state_t;
static newt_t pnd[N],pnf[N-2],pnb[N-4],pnh[N-6],pnm[N-6]; static newt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(newt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int si,int ay){if(*cnt>=mx)return -1;newt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->sw_sp=ss;x->sk_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[NEWT] Newt ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int newt_init(void){if(init)return -1;st.n_pond=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_sw=0;st.t_sk=0;for(int i=0;i<N;i++)pnd[i].active=0;for(int i=0;i<N-2;i++)pnf[i].active=0;for(int i=0;i<N-4;i++)pnb[i].active=0;for(int i=0;i<N-6;i++)pnh[i].active=0;for(int i=0;i<N-6;i++)pnm[i].active=0;init=1;ps("[NEWT] Newt initialized\n");return 0;}
int newt_pond(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(pnd,&st.n_pond,&st.t_ln,N,lc,bl,bw,tc,ss,si,ay);}
int newt_feeding(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(pnf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,si,ay);}
int newt_breeding(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(pnb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,ss,si,ay);}
int newt_health(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(pnh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,tc,ss,si,ay);}
int newt_market(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(pnm,&st.n_mkt,&st.t_sk,N-6,lc,bl,bw,tc,ss,si,ay);}
void newt_report(void){ps("[NEWT] Pond: ");pi(st.n_pond);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sk=");pi(st.t_sk);ps("\n");}
void newt_state(void){ps("[NEWT] Pond=");pi(st.n_pond);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Newt Admin Demo ===\n\n");newt_init();
ps("Newt pond...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=8+(i*1),bw=3+(i*1),tc=5+(i*1),ss=1+(i*1),si=(i%6)+1,ay=(i%5)+1;newt_pond(lc,bl,bw,tc,ss,si,ay);}
ps("\nNewt feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=9+(i*1),bw=3+(i*1),tc=5+(i*1),ss=1+(i*1),si=(i%5)+1,ay=(i%4)+1;newt_feeding(lc,bl,bw,tc,ss,si,ay);}
ps("\nNewt breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=10+(i*1),bw=4+(i*1),tc=6+(i*1),ss=1+(i*1),si=(i%4)+1,ay=(i%3)+1;newt_breeding(lc,bl,bw,tc,ss,si,ay);}
ps("\nNewt health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=7+(i*1),bw=2+(i*1),tc=4+(i*1),ss=0+(i*1),si=(i%7)+1,ay=(i%3)+1;newt_health(lc,bl,bw,tc,ss,si,ay);}
ps("\nNewt market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=11+(i*1),bw=4+(i*1),tc=6+(i*1),ss=2+(i*1),si=(i%3)+1,ay=(i%2)+1;newt_market(lc,bl,bw,tc,ss,si,ay);}
ps("\n");newt_report();newt_state();ps("\n=== Demo Complete ===\n");return 0;}
