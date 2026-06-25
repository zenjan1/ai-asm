/* newt_admin: Newt (Triturus cristatus) crested newt salamander amphibian (v1.0)
 * Newt pond, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, skin_idx, egg_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,sk_idx,eg_ct,age_yr,active;} newt_t;
typedef struct{int n_pond,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_sk,t_eg;} newt_state_t;
static newt_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static newt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(newt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int si,int ec,int ay){if(*cnt>=mx)return -1;newt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->sk_idx=si;x->eg_ct=ec;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[NEWT] Newt ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" si=");pi(si);ps(" ec=");pi(ec);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int newt_init(void){if(init)return -1;st.n_pond=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_sk=0;st.t_eg=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[NEWT] Newt initialized\n");return 0;}
int newt_pond(int lc,int bl,int bw,int tc,int si,int ec,int ay){return add(apl,&st.n_pond,&st.t_ln,N,lc,bl,bw,tc,si,ec,ay);}
int newt_feeding(int lc,int bl,int bw,int tc,int si,int ec,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,si,ec,ay);}
int newt_breeding(int lc,int bl,int bw,int tc,int si,int ec,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,si,ec,ay);}
int newt_health(int lc,int bl,int bw,int tc,int si,int ec,int ay){return add(ahd,&st.n_health,&st.t_sk,N-6,lc,bl,bw,tc,si,ec,ay);}
int newt_market(int lc,int bl,int bw,int tc,int si,int ec,int ay){return add(amd,&st.n_mkt,&st.t_eg,N-6,lc,bl,bw,tc,si,ec,ay);}
void newt_report(void){ps("[NEWT] Pond: ");pi(st.n_pond);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Sk=");pi(st.t_sk);ps("\nMkt: ");pi(st.n_mkt);ps(" Eg=");pi(st.t_eg);ps("\n");}
void newt_state(void){ps("[NEWT] Pond=");pi(st.n_pond);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Newt Admin Demo ===\n\n");newt_init();
ps("Newt pond...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=6+(i*2),bw=3+(i*2),tc=3+(i*2),si=(i%6)+1,ec=20+(i*10),ay=1+(i%5);newt_pond(lc,bl,bw,tc,si,ec,ay);}
ps("\nNewt feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=7+(i*2),bw=4+(i*2),tc=4+(i*2),si=(i%5)+2,ec=25+(i*10),ay=1+(i%4);newt_feeding(lc,bl,bw,tc,si,ec,ay);}
ps("\nNewt breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=8+(i*2),bw=4+(i*2),tc=4+(i*2),si=(i%4)+1,ec=30+(i*10),ay=2+(i%3);newt_breeding(lc,bl,bw,tc,si,ec,ay);}
ps("\nNewt health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=5+(i*3),bw=3+(i*3),tc=3+(i*3),si=(i%3)+3,ec=15+(i*12),ay=2+(i%3);newt_health(lc,bl,bw,tc,si,ec,ay);}
ps("\nNewt market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=10+(i*2),bw=5+(i*2),tc=5+(i*2),si=(i%6)+1,ec=35+(i*10),ay=3+(i%2);newt_market(lc,bl,bw,tc,si,ec,ay);}
ps("\n");newt_report();newt_state();ps("\n=== Demo Complete ===\n");return 0;}
