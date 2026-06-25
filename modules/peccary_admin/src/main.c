/* peccary_admin: Peccary (Pecari tajacu) collared peccary wild pig (v1.0)
 * Peccary scrub, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tusk_cm, run_speed, pc_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tsk_cm,rn_sp,pc_idx,age_yr,active;} pecr_t;
typedef struct{int n_scrub,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tsk,t_rn,t_pc;} pecr_state_t;
static pecr_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static pecr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pecr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int pi_idx,int ay){if(*cnt>=mx)return -1;pecr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tsk_cm=tc;x->rn_sp=rs;x->pc_idx=pi_idx;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PECR] Peccary ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" pi=");pi(pi_idx);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int peccary_init(void){if(init)return -1;st.n_scrub=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tsk=0;st.t_rn=0;st.t_pc=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PECR] Peccary initialized\n");return 0;}
int peccary_scrub(int lc,int bl,int bw,int tc,int rs,int pi_idx,int ay){return add(svl,&st.n_scrub,&st.t_ln,N,lc,bl,bw,tc,rs,pi_idx,ay);}
int peccary_feeding(int lc,int bl,int bw,int tc,int rs,int pi_idx,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,pi_idx,ay);}
int peccary_breeding(int lc,int bl,int bw,int tc,int rs,int pi_idx,int ay){return add(svb,&st.n_breed,&st.t_tsk,N-4,lc,bl,bw,tc,rs,pi_idx,ay);}
int peccary_health(int lc,int bl,int bw,int tc,int rs,int pi_idx,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,tc,rs,pi_idx,ay);}
int peccary_market(int lc,int bl,int bw,int tc,int rs,int pi_idx,int ay){return add(svm,&st.n_mkt,&st.t_pc,N-6,lc,bl,bw,tc,rs,pi_idx,ay);}
void peccary_report(void){ps("[PECR] Scrub: ");pi(st.n_scrub);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tsk=");pi(st.t_tsk);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Pc=");pi(st.t_pc);ps("\n");}
void peccary_state(void){ps("[PECR] Scrub=");pi(st.n_scrub);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Peccary Admin Demo ===\n\n");peccary_init();
ps("Peccary scrub...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*5),bw=20+(i*3),tc=8+(i*1),rs=18+(i*2),pi_idx=(i%8)+1,ay=(i%12)+1;peccary_scrub(lc,bl,bw,tc,rs,pi_idx,ay);}
ps("\nPeccary feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=83+(i*4),bw=22+(i*2),tc=9+(i*1),rs=19+(i*1),pi_idx=(i%6)+1,ay=(i%10)+1;peccary_feeding(lc,bl,bw,tc,rs,pi_idx,ay);}
ps("\nPeccary breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=86+(i*3),bw=24+(i*2),tc=10+(i*1),rs=17+(i*2),pi_idx=(i%5)+1,ay=(i%8)+1;peccary_breeding(lc,bl,bw,tc,rs,pi_idx,ay);}
ps("\nPeccary health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=78+(i*6),bw=18+(i*4),tc=7+(i*1),rs=15+(i*3),pi_idx=(i%10)+1,ay=(i%7)+1;peccary_health(lc,bl,bw,tc,rs,pi_idx,ay);}
ps("\nPeccary market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=90+(i*2),bw=26+(i*1),tc=11+(i*1),rs=21+(i*1),pi_idx=(i%4)+1,ay=(i%6)+1;peccary_market(lc,bl,bw,tc,rs,pi_idx,ay);}
ps("\n");peccary_report();peccary_state();ps("\n=== Demo Complete ===\n");return 0;}
