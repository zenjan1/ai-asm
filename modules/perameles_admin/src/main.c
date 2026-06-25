/* perameles_admin: Perameles (Perameles) bandicoots (v1.0)
 * Perameles forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, nose_cm, tail_cm, pm_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,nose_cm,tail_cm,pm_idx,age_yr,active;} prml_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_nose,t_tail,t_pm;} prml_state_t;
static prml_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static prml_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(prml_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nc,int tc,int pm,int ay){if(*cnt>=mx)return -1;prml_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->nose_cm=nc;x->tail_cm=tc;x->pm_idx=pm;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PRML] Perameles ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nc=");pi(nc);ps(" tc=");pi(tc);ps(" pm=");pi(pm);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int perameles_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_nose=0;st.t_tail=0;st.t_pm=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PRML] Perameles initialized\n");return 0;}
int perameles_forest(int lc,int bl,int bw,int nc,int tc,int pm,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,nc,tc,pm,ay);}
int perameles_feeding(int lc,int bl,int bw,int nc,int tc,int pm,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nc,tc,pm,ay);}
int perameles_breeding(int lc,int bl,int bw,int nc,int tc,int pm,int ay){return add(svb,&st.n_breed,&st.t_nose,N-4,lc,bl,bw,nc,tc,pm,ay);}
int perameles_health(int lc,int bl,int bw,int nc,int tc,int pm,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,nc,tc,pm,ay);}
int perameles_market(int lc,int bl,int bw,int nc,int tc,int pm,int ay){return add(svm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,nc,tc,pm,ay);}
void perameles_report(void){ps("[PRML] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Nose=");pi(st.t_nose);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void perameles_state(void){ps("[PRML] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Perameles Admin Demo ===\n\n");perameles_init();
ps("Perameles forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=32+(i*2),bw=1+(i*1),nc=4+(i%3),tc=10+(i*2),pm=(i%8)+1,ay=(i%5)+1;perameles_forest(lc,bl,bw,nc,tc,pm,ay);}
ps("\nPerameles feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=34+(i*1),bw=2+(i*1),nc=5+(i%2),tc=11+(i%1),pm=(i%6)+1,ay=(i%4)+1;perameles_feeding(lc,bl,bw,nc,tc,pm,ay);}
ps("\nPerameles breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=36+(i*1),bw=2+(i*1),nc=6+(i%2),tc=13+(i%1),pm=(i%5)+1,ay=(i%3)+1;perameles_breeding(lc,bl,bw,nc,tc,pm,ay);}
ps("\nPerameles health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=30+(i*3),bw=1+(i*1),nc=3+(i%3),tc=9+(i*2),pm=(i%10)+1,ay=(i%5)+1;perameles_health(lc,bl,bw,nc,tc,pm,ay);}
ps("\nPerameles market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=38+(i*1),bw=3+(i*1),nc=7+(i%2),tc=14+(i%1),pm=(i%4)+1,ay=(i%3)+1;perameles_market(lc,bl,bw,nc,tc,pm,ay);}
ps("\n");perameles_report();perameles_state();ps("\n=== Demo Complete ===\n");return 0;}
