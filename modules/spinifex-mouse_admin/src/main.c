/* spinifex_mouse_admin: Spinifex-mouse desert-dwelling rodents (v1.0)
 * Spinifex-mouse forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, claw_ct, tail_cm, spm_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_ct,tail_cm,spm_idx,age_yr,active;} spmx_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_tail,t_sp;} spmx_state_t;
static spmx_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static spmx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spmx_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int tc,int sp,int ay){if(*cnt>=mx)return -1;spmx_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_ct=cc;x->tail_cm=tc;x->spm_idx=sp;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SPMX] Spinifex-mouse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" tc=");pi(tc);ps(" sp=");pi(sp);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int spinifex_mouse_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_tail=0;st.t_sp=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[SPMX] Spinifex-mouse initialized\n");return 0;}
int spinifex_mouse_forest(int lc,int bl,int bw,int cc,int tc,int sp,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,cc,tc,sp,ay);}
int spinifex_mouse_feeding(int lc,int bl,int bw,int cc,int tc,int sp,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,tc,sp,ay);}
int spinifex_mouse_breeding(int lc,int bl,int bw,int cc,int tc,int sp,int ay){return add(svb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,tc,sp,ay);}
int spinifex_mouse_health(int lc,int bl,int bw,int cc,int tc,int sp,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,cc,tc,sp,ay);}
int spinifex_mouse_market(int lc,int bl,int bw,int cc,int tc,int sp,int ay){return add(svm,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,cc,tc,sp,ay);}
void spinifex_mouse_report(void){ps("[SPMX] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void spinifex_mouse_state(void){ps("[SPMX] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Spinifex-Mouse Admin Demo ===\n\n");spinifex_mouse_init();
ps("Spinifex-mouse forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=8+(i*1),bw=11+(i*2),cc=4+(i%3),tc=6+(i*1),sp=(i%8)+1,ay=(i%5)+1;spinifex_mouse_forest(lc,bl,bw,cc,tc,sp,ay);}
ps("\nSpinifex-mouse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=9+(i*1),bw=13+(i*2),cc=4+(i%3),tc=6+(i*1),sp=(i%6)+1,ay=(i%4)+1;spinifex_mouse_feeding(lc,bl,bw,cc,tc,sp,ay);}
ps("\nSpinifex-mouse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=10+(i*1),bw=15+(i*2),cc=5+(i%3),tc=7+(i*1),sp=(i%5)+1,ay=(i%3)+1;spinifex_mouse_breeding(lc,bl,bw,cc,tc,sp,ay);}
ps("\nSpinifex-mouse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=7+(i*1),bw=9+(i*2),cc=3+(i%3),tc=5+(i*1),sp=(i%10)+1,ay=(i%5)+1;spinifex_mouse_health(lc,bl,bw,cc,tc,sp,ay);}
ps("\nSpinifex-mouse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=11+(i*1),bw=17+(i*2),cc=5+(i%3),tc=7+(i*1),sp=(i%4)+1,ay=(i%3)+1;spinifex_mouse_market(lc,bl,bw,cc,tc,sp,ay);}
ps("\n");spinifex_mouse_report();spinifex_mouse_state();ps("\n=== Demo Complete ===\n");return 0;}
