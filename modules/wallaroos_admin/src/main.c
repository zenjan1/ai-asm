/* wallaroos_admin: Wallaroos (Macropus) large rock-wallabies (v1.0)
 * Wallaroos forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, tail_cm, wl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,tail_cm,wl_idx,age_yr,active;} wllr_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_tail,t_wl;} wllr_state_t;
static wllr_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static wllr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wllr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int tc,int wl,int ay){if(*cnt>=mx)return -1;wllr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->tail_cm=tc;x->wl_idx=wl;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WLLR] Wallaroo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" tc=");pi(tc);ps(" wl=");pi(wl);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int wallaroos_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_tail=0;st.t_wl=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[WLLR] Wallaroos initialized\n");return 0;}
int wallaroos_forest(int lc,int bl,int bw,int ec,int tc,int wl,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ec,tc,wl,ay);}
int wallaroos_feeding(int lc,int bl,int bw,int ec,int tc,int wl,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,tc,wl,ay);}
int wallaroos_breeding(int lc,int bl,int bw,int ec,int tc,int wl,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,tc,wl,ay);}
int wallaroos_health(int lc,int bl,int bw,int ec,int tc,int wl,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,ec,tc,wl,ay);}
int wallaroos_market(int lc,int bl,int bw,int ec,int tc,int wl,int ay){return add(svm,&st.n_mkt,&st.t_wl,N-6,lc,bl,bw,ec,tc,wl,ay);}
void wallaroos_report(void){ps("[WLLR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Wl=");pi(st.t_wl);ps("\n");}
void wallaroos_state(void){ps("[WLLR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Wallaroos Admin Demo ===\n\n");wallaroos_init();
ps("Wallaroos forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*3),bw=15+(i*2),ec=12+(i%4),tc=60+(i*3),wl=(i%8)+1,ay=(i%5)+1;wallaroos_forest(lc,bl,bw,ec,tc,wl,ay);}
ps("\nWallaroos feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=82+(i*2),bw=16+(i*2),ec=13+(i%3),tc=62+(i*2),wl=(i%6)+1,ay=(i%4)+1;wallaroos_feeding(lc,bl,bw,ec,tc,wl,ay);}
ps("\nWallaroos breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=85+(i*2),bw=18+(i*1),ec=14+(i%3),tc=65+(i*2),wl=(i%5)+1,ay=(i%3)+1;wallaroos_breeding(lc,bl,bw,ec,tc,wl,ay);}
ps("\nWallaroos health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=78+(i*4),bw=14+(i*2),ec=11+(i%4),tc=58+(i*3),wl=(i%10)+1,ay=(i%5)+1;wallaroos_health(lc,bl,bw,ec,tc,wl,ay);}
ps("\nWallaroos market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=88+(i*2),bw=20+(i*1),ec=15+(i%2),tc=68+(i*2),wl=(i%4)+1,ay=(i%3)+1;wallaroos_market(lc,bl,bw,ec,tc,wl,ay);}
ps("\n");wallaroos_report();wallaroos_state();ps("\n=== Demo Complete ===\n");return 0;}
