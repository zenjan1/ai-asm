/* koalas_admin: Koalas (Phascolarctos) tree-dwelling marsupials (v1.0)
 * Koalas canopy, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, grip_kg, ko_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,grip_kg,ko_idx,age_yr,active;} kols_t;
typedef struct{int n_canopy,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_grip,t_ko;} kols_state_t;
static kols_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static kols_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kols_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int gk,int ko,int ay){if(*cnt>=mx)return -1;kols_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->grip_kg=gk;x->ko_idx=ko;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KOLS] Koala ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" gk=");pi(gk);ps(" ko=");pi(ko);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int koalas_init(void){if(init)return -1;st.n_canopy=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_grip=0;st.t_ko=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[KOLS] Koalas initialized\n");return 0;}
int koalas_canopy(int lc,int bl,int bw,int ec,int gk,int ko,int ay){return add(svl,&st.n_canopy,&st.t_ln,N,lc,bl,bw,ec,gk,ko,ay);}
int koalas_feeding(int lc,int bl,int bw,int ec,int gk,int ko,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,gk,ko,ay);}
int koalas_breeding(int lc,int bl,int bw,int ec,int gk,int ko,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,gk,ko,ay);}
int koalas_health(int lc,int bl,int bw,int ec,int gk,int ko,int ay){return add(svh,&st.n_health,&st.t_grip,N-6,lc,bl,bw,ec,gk,ko,ay);}
int koalas_market(int lc,int bl,int bw,int ec,int gk,int ko,int ay){return add(svm,&st.n_mkt,&st.t_ko,N-6,lc,bl,bw,ec,gk,ko,ay);}
void koalas_report(void){ps("[KOLS] Canopy: ");pi(st.n_canopy);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Grip=");pi(st.t_grip);ps("\nMkt: ");pi(st.n_mkt);ps(" Ko=");pi(st.t_ko);ps("\n");}
void koalas_state(void){ps("[KOLS] Canopy=");pi(st.n_canopy);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Koalas Admin Demo ===\n\n");koalas_init();
ps("Koalas canopy...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=65+(i*4),bw=8+(i*1),ec=5+(i%3),gk=3+(i%3),ko=(i%8)+1,ay=(i%10)+1;koalas_canopy(lc,bl,bw,ec,gk,ko,ay);}
ps("\nKoalas feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=68+(i*3),bw=9+(i*1),ec=6+(i%2),gk=3+(i%2),ko=(i%6)+1,ay=(i%8)+1;koalas_feeding(lc,bl,bw,ec,gk,ko,ay);}
ps("\nKoalas breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=70+(i*3),bw=9+(i*1),ec=6+(i%2),gk=4+(i%2),ko=(i%5)+1,ay=(i%6)+1;koalas_breeding(lc,bl,bw,ec,gk,ko,ay);}
ps("\nKoalas health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=62+(i*5),bw=7+(i*1),ec=4+(i%3),gk=2+(i%3),ko=(i%10)+1,ay=(i%8)+1;koalas_health(lc,bl,bw,ec,gk,ko,ay);}
ps("\nKoalas market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=73+(i*2),bw=10+(i*1),ec=7+(i%2),gk=4+(i%2),ko=(i%4)+1,ay=(i%6)+1;koalas_market(lc,bl,bw,ec,gk,ko,ay);}
ps("\n");koalas_report();koalas_state();ps("\n=== Demo Complete ===\n");return 0;}
