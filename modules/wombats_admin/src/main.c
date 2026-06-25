/* wombats_admin: Wombats (Vombatus) burrowing marsupials (v1.0)
 * Wombats burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_mm, dig_sp, wm_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_mm,dig_sp,wm_idx,age_yr,active;} wmbs_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_dig,t_wm;} wmbs_state_t;
static wmbs_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static wmbs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wmbs_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cm,int dg,int wm,int ay){if(*cnt>=mx)return -1;wmbs_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_mm=cm;x->dig_sp=dg;x->wm_idx=wm;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WMBS] Wombat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cm=");pi(cm);ps(" dg=");pi(dg);ps(" wm=");pi(wm);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int wombats_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_dig=0;st.t_wm=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[WMBS] Wombats initialized\n");return 0;}
int wombats_burrow(int lc,int bl,int bw,int cm,int dg,int wm,int ay){return add(svl,&st.n_burrow,&st.t_ln,N,lc,bl,bw,cm,dg,wm,ay);}
int wombats_feeding(int lc,int bl,int bw,int cm,int dg,int wm,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cm,dg,wm,ay);}
int wombats_breeding(int lc,int bl,int bw,int cm,int dg,int wm,int ay){return add(svb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cm,dg,wm,ay);}
int wombats_health(int lc,int bl,int bw,int cm,int dg,int wm,int ay){return add(svh,&st.n_health,&st.t_dig,N-6,lc,bl,bw,cm,dg,wm,ay);}
int wombats_market(int lc,int bl,int bw,int cm,int dg,int wm,int ay){return add(svm,&st.n_mkt,&st.t_wm,N-6,lc,bl,bw,cm,dg,wm,ay);}
void wombats_report(void){ps("[WMBS] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Dig=");pi(st.t_dig);ps("\nMkt: ");pi(st.n_mkt);ps(" Wm=");pi(st.t_wm);ps("\n");}
void wombats_state(void){ps("[WMBS] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Wombats Admin Demo ===\n\n");wombats_init();
ps("Wombats burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*5),bw=20+(i*3),cm=8+(i%3),dg=5+(i%3),wm=(i%8)+1,ay=(i%8)+1;wombats_burrow(lc,bl,bw,cm,dg,wm,ay);}
ps("\nWombats feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=85+(i*4),bw=22+(i*2),cm=9+(i%2),dg=6+(i%2),wm=(i%6)+1,ay=(i%7)+1;wombats_feeding(lc,bl,bw,cm,dg,wm,ay);}
ps("\nWombats breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=90+(i*3),bw=24+(i*2),cm=9+(i%2),dg=6+(i%2),wm=(i%5)+1,ay=(i%6)+1;wombats_breeding(lc,bl,bw,cm,dg,wm,ay);}
ps("\nWombats health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=75+(i*6),bw=18+(i*4),cm=7+(i%3),dg=4+(i%3),wm=(i%10)+1,ay=(i%7)+1;wombats_health(lc,bl,bw,cm,dg,wm,ay);}
ps("\nWombats market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=95+(i*2),bw=26+(i*2),cm=10+(i%2),dg=7+(i%2),wm=(i%4)+1,ay=(i%5)+1;wombats_market(lc,bl,bw,cm,dg,wm,ay);}
ps("\n");wombats_report();wombats_state();ps("\n=== Demo Complete ===\n");return 0;}
