/* bigoll_mouse_admin: Bigoll-mouse vegetable rodents (v1.0)
 * Bigoll-mouse forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, bigoll_sz, tail_cm, bgll_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,bigoll_sz,tail_cm,bgll_idx,age_yr,active;} bgll_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_bz,t_tail,t_si;} bgll_state_t;
static bgll_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static bgll_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bgll_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bz,int tc,int si,int ay){if(*cnt>=mx)return -1;bgll_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->bigoll_sz=bz;x->tail_cm=tc;x->bgll_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BGLL] Bigoll-mouse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bz=");pi(bz);ps(" tc=");pi(tc);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bigoll_mouse_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_bz=0;st.t_tail=0;st.t_si=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BGLL] Bigoll-mouse initialized\n");return 0;}
int bigoll_mouse_forest(int lc,int bl,int bw,int bz,int tc,int si,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,bz,tc,si,ay);}
int bigoll_mouse_feeding(int lc,int bl,int bw,int bz,int tc,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bz,tc,si,ay);}
int bigoll_mouse_breeding(int lc,int bl,int bw,int bz,int tc,int si,int ay){return add(svb,&st.n_breed,&st.t_bz,N-4,lc,bl,bw,bz,tc,si,ay);}
int bigoll_mouse_health(int lc,int bl,int bw,int bz,int tc,int si,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,bz,tc,si,ay);}
int bigoll_mouse_market(int lc,int bl,int bw,int bz,int tc,int si,int ay){return add(svm,&st.n_mkt,&st.t_si,N-6,lc,bl,bw,bz,tc,si,ay);}
void bigoll_mouse_report(void){ps("[BGLL] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Bz=");pi(st.t_bz);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Si=");pi(st.t_si);ps("\n");}
void bigoll_mouse_state(void){ps("[BGLL] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bigoll-Mouse Admin Demo ===\n\n");bigoll_mouse_init();
ps("Bigoll-mouse forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*1),bw=18+(i*3),bz=8+(i*4),tc=8+(i*1),si=(i%7)+1,ay=(i%5)+1;bigoll_mouse_forest(lc,bl,bw,bz,tc,si,ay);}
ps("\nBigoll-mouse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*1),bw=20+(i*3),bz=8+(i*4),tc=8+(i*1),si=(i%6)+1,ay=(i%4)+1;bigoll_mouse_feeding(lc,bl,bw,bz,tc,si,ay);}
ps("\nBigoll-mouse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=22+(i*3),bz=9+(i*4),tc=9+(i*1),si=(i%5)+1,ay=(i%3)+1;bigoll_mouse_breeding(lc,bl,bw,bz,tc,si,ay);}
ps("\nBigoll-mouse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*1),bw=16+(i*3),bz=7+(i*4),tc=7+(i*1),si=(i%9)+1,ay=(i%5)+1;bigoll_mouse_health(lc,bl,bw,bz,tc,si,ay);}
ps("\nBigoll-mouse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=24+(i*3),bz=10+(i*4),tc=9+(i*1),si=(i%4)+1,ay=(i%3)+1;bigoll_mouse_market(lc,bl,bw,bz,tc,si,ay);}
ps("\n");bigoll_mouse_report();bigoll_mouse_state();ps("\n=== Demo Complete ===\n");return 0;}
