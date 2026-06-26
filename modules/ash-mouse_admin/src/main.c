/* ash_mouse_admin: Ash-mouse deciduous rodents (v1.0)
 * Ash-mouse forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, seed_hrd, tail_cm, ash_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,seed_hrd,tail_cm,ash_idx,age_yr,active;} ashmx_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_sh,t_tail,t_ah;} ashmx_state_t;
static ashmx_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static ashmx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ashmx_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sh,int tc,int ah,int ay){if(*cnt>=mx)return -1;ashmx_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->seed_hrd=sh;x->tail_cm=tc;x->ash_idx=ah;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ASHMX] Ash-mouse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sh=");pi(sh);ps(" tc=");pi(tc);ps(" ah=");pi(ah);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ash_mouse_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_sh=0;st.t_tail=0;st.t_ah=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ASHMX] Ash-mouse initialized\n");return 0;}
int ash_mouse_forest(int lc,int bl,int bw,int sh,int tc,int ah,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,sh,tc,ah,ay);}
int ash_mouse_feeding(int lc,int bl,int bw,int sh,int tc,int ah,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sh,tc,ah,ay);}
int ash_mouse_breeding(int lc,int bl,int bw,int sh,int tc,int ah,int ay){return add(svb,&st.n_breed,&st.t_sh,N-4,lc,bl,bw,sh,tc,ah,ay);}
int ash_mouse_health(int lc,int bl,int bw,int sh,int tc,int ah,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,sh,tc,ah,ay);}
int ash_mouse_market(int lc,int bl,int bw,int sh,int tc,int ah,int ay){return add(svm,&st.n_mkt,&st.t_ah,N-6,lc,bl,bw,sh,tc,ah,ay);}
void ash_mouse_report(void){ps("[ASHMX] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Sh=");pi(st.t_sh);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Ah=");pi(st.t_ah);ps("\n");}
void ash_mouse_state(void){ps("[ASHMX] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ash-Mouse Admin Demo ===\n\n");ash_mouse_init();
ps("Ash-mouse forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*1),bw=18+(i*3),sh=15+(i%10),tc=8+(i*1),ah=(i%7)+1,ay=(i%5)+1;ash_mouse_forest(lc,bl,bw,sh,tc,ah,ay);}
ps("\nAsh-mouse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*1),bw=20+(i*3),sh=15+(i%10),tc=8+(i*1),ah=(i%6)+1,ay=(i%4)+1;ash_mouse_feeding(lc,bl,bw,sh,tc,ah,ay);}
ps("\nAsh-mouse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=22+(i*3),sh=18+(i%10),tc=9+(i*1),ah=(i%5)+1,ay=(i%3)+1;ash_mouse_breeding(lc,bl,bw,sh,tc,ah,ay);}
ps("\nAsh-mouse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*1),bw=16+(i*3),sh=12+(i%10),tc=7+(i*1),ah=(i%9)+1,ay=(i%5)+1;ash_mouse_health(lc,bl,bw,sh,tc,ah,ay);}
ps("\nAsh-mouse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=24+(i*3),sh=20+(i%10),tc=9+(i*1),ah=(i%4)+1,ay=(i%3)+1;ash_mouse_market(lc,bl,bw,sh,tc,ah,ay);}
ps("\n");ash_mouse_report();ash_mouse_state();ps("\n=== Demo Complete ===\n");return 0;}
