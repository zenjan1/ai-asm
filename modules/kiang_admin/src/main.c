/* kiang_admin: Kiang (Equus kiang) Tibetan plateau wild ass (v1.0)
 * Kiang plateau, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, run_speed, ki_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,rn_sp,ki_idx,age_yr,active;} kiang_t;
typedef struct{int n_plateau,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_rn,t_ki;} kiang_state_t;
static kiang_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static kiang_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kiang_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int rs,int ki,int ay){if(*cnt>=mx)return -1;kiang_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->rn_sp=rs;x->ki_idx=ki;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KING] Kiang ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" rs=");pi(rs);ps(" ki=");pi(ki);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int kiang_init(void){if(init)return -1;st.n_plateau=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_rn=0;st.t_ki=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[KING] Kiang initialized\n");return 0;}
int kiang_plateau(int lc,int bl,int bw,int ec,int rs,int ki,int ay){return add(svl,&st.n_plateau,&st.t_ln,N,lc,bl,bw,ec,rs,ki,ay);}
int kiang_feeding(int lc,int bl,int bw,int ec,int rs,int ki,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,rs,ki,ay);}
int kiang_breeding(int lc,int bl,int bw,int ec,int rs,int ki,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,rs,ki,ay);}
int kiang_health(int lc,int bl,int bw,int ec,int rs,int ki,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,ec,rs,ki,ay);}
int kiang_market(int lc,int bl,int bw,int ec,int rs,int ki,int ay){return add(svm,&st.n_mkt,&st.t_ki,N-6,lc,bl,bw,ec,rs,ki,ay);}
void kiang_report(void){ps("[KING] Plateau: ");pi(st.n_plateau);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Ki=");pi(st.t_ki);ps("\n");}
void kiang_state(void){ps("[KING] Plateau=");pi(st.n_plateau);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Kiang Admin Demo ===\n\n");kiang_init();
ps("Kiang plateau...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=300+(i*25),ec=14+(i*1),rs=28+(i*3),ki=(i%8)+1,ay=(i%18)+1;kiang_plateau(lc,bl,bw,ec,rs,ki,ay);}
ps("\nKiang feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=310+(i*20),ec=15+(i*1),rs=30+(i*2),ki=(i%6)+1,ay=(i%15)+1;kiang_feeding(lc,bl,bw,ec,rs,ki,ay);}
ps("\nKiang breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=320+(i*16),ec=16+(i*1),rs=26+(i*3),ki=(i%5)+1,ay=(i%12)+1;kiang_breeding(lc,bl,bw,ec,rs,ki,ay);}
ps("\nKiang health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=290+(i*30),ec=13+(i*2),rs=24+(i*4),ki=(i%10)+1,ay=(i%10)+1;kiang_health(lc,bl,bw,ec,rs,ki,ay);}
ps("\nKiang market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*5),bw=330+(i*12),ec=17+(i*1),rs=32+(i*2),ki=(i%4)+1,ay=(i%8)+1;kiang_market(lc,bl,bw,ec,rs,ki,ay);}
ps("\n");kiang_report();kiang_state();ps("\n=== Demo Complete ===\n");return 0;}
