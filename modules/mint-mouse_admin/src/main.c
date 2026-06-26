/* mint_mouse_admin: Mint-mouse herb rodents (v1.0)
 * Mint-mouse forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, node_ct, tail_cm, mntmx_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,node_ct,tail_cm,mntmx_idx,age_yr,active;} mntmx_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_nc,t_tail,t_mi;} mntmx_state_t;
static mntmx_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static mntmx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mntmx_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nc,int tc,int mi,int ay){if(*cnt>=mx)return -1;mntmx_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->node_ct=nc;x->tail_cm=tc;x->mntmx_idx=mi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MNTMX] Mint-mouse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nc=");pi(nc);ps(" tc=");pi(tc);ps(" mi=");pi(mi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mint_mouse_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_nc=0;st.t_tail=0;st.t_mi=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[MNTMX] Mint-mouse initialized\n");return 0;}
int mint_mouse_forest(int lc,int bl,int bw,int nc,int tc,int mi,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,nc,tc,mi,ay);}
int mint_mouse_feeding(int lc,int bl,int bw,int nc,int tc,int mi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nc,tc,mi,ay);}
int mint_mouse_breeding(int lc,int bl,int bw,int nc,int tc,int mi,int ay){return add(svb,&st.n_breed,&st.t_nc,N-4,lc,bl,bw,nc,tc,mi,ay);}
int mint_mouse_health(int lc,int bl,int bw,int nc,int tc,int mi,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,nc,tc,mi,ay);}
int mint_mouse_market(int lc,int bl,int bw,int nc,int tc,int mi,int ay){return add(svm,&st.n_mkt,&st.t_mi,N-6,lc,bl,bw,nc,tc,mi,ay);}
void mint_mouse_report(void){ps("[MNTMX] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Nc=");pi(st.t_nc);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Mi=");pi(st.t_mi);ps("\n");}
void mint_mouse_state(void){ps("[MNTMX] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Mint-Mouse Admin Demo ===\n\n");mint_mouse_init();
ps("Mint-mouse forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*1),bw=18+(i*3),nc=4+(i%8),tc=8+(i*1),mi=(i%7)+1,ay=(i%5)+1;mint_mouse_forest(lc,bl,bw,nc,tc,mi,ay);}
ps("\nMint-mouse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*1),bw=20+(i*3),nc=4+(i%8),tc=8+(i*1),mi=(i%6)+1,ay=(i%4)+1;mint_mouse_feeding(lc,bl,bw,nc,tc,mi,ay);}
ps("\nMint-mouse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=22+(i*3),nc=5+(i%8),tc=9+(i*1),mi=(i%5)+1,ay=(i%3)+1;mint_mouse_breeding(lc,bl,bw,nc,tc,mi,ay);}
ps("\nMint-mouse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*1),bw=16+(i*3),nc=3+(i%8),tc=7+(i*1),mi=(i%9)+1,ay=(i%5)+1;mint_mouse_health(lc,bl,bw,nc,tc,mi,ay);}
ps("\nMint-mouse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=24+(i*3),nc=6+(i%8),tc=9+(i*1),mi=(i%4)+1,ay=(i%3)+1;mint_mouse_market(lc,bl,bw,nc,tc,mi,ay);}
ps("\n");mint_mouse_report();mint_mouse_state();ps("\n=== Demo Complete ===\n");return 0;}
