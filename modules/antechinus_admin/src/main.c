/* antechinus_admin: Antechinus (Antechinus) small marsupial mouse (v1.0)
 * Antechinus forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fur_mm, claw_mm, ac_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fur_mm,claw_mm,ac_idx,age_yr,active;} atcn_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fur,t_claw,t_ac;} atcn_state_t;
static atcn_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static atcn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(atcn_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fm,int cm,int ac,int ay){if(*cnt>=mx)return -1;atcn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fur_mm=fm;x->claw_mm=cm;x->ac_idx=ac;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ATCN] Antechinus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fm=");pi(fm);ps(" cm=");pi(cm);ps(" ac=");pi(ac);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int antechinus_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fur=0;st.t_claw=0;st.t_ac=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ATCN] Antechinus initialized\n");return 0;}
int antechinus_forest(int lc,int bl,int bw,int fm,int cm,int ac,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,fm,cm,ac,ay);}
int antechinus_feeding(int lc,int bl,int bw,int fm,int cm,int ac,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fm,cm,ac,ay);}
int antechinus_breeding(int lc,int bl,int bw,int fm,int cm,int ac,int ay){return add(svb,&st.n_breed,&st.t_fur,N-4,lc,bl,bw,fm,cm,ac,ay);}
int antechinus_health(int lc,int bl,int bw,int fm,int cm,int ac,int ay){return add(svh,&st.n_health,&st.t_claw,N-6,lc,bl,bw,fm,cm,ac,ay);}
int antechinus_market(int lc,int bl,int bw,int fm,int cm,int ac,int ay){return add(svm,&st.n_mkt,&st.t_ac,N-6,lc,bl,bw,fm,cm,ac,ay);}
void antechinus_report(void){ps("[ATCN] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fur=");pi(st.t_fur);ps("\nHealth: ");pi(st.n_health);ps(" Claw=");pi(st.t_claw);ps("\nMkt: ");pi(st.n_mkt);ps(" Ac=");pi(st.t_ac);ps("\n");}
void antechinus_state(void){ps("[ATCN] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Antechinus Admin Demo ===\n\n");antechinus_init();
ps("Antechinus forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=8+(i*1),bw=15+(i*2),fm=5+(i%3),cm=2+(i%3),ac=(i%8)+1,ay=(i%2)+1;antechinus_forest(lc,bl,bw,fm,cm,ac,ay);}
ps("\nAntechinus feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=9+(i*1),bw=17+(i*2),fm=6+(i%2),cm=2+(i%2),ac=(i%6)+1,ay=(i%2)+1;antechinus_feeding(lc,bl,bw,fm,cm,ac,ay);}
ps("\nAntechinus breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=10+(i*1),bw=19+(i*2),fm=6+(i%2),cm=3+(i%2),ac=(i%5)+1,ay=(i%1)+1;antechinus_breeding(lc,bl,bw,fm,cm,ac,ay);}
ps("\nAntechinus health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=7+(i*1),bw=13+(i*3),fm=4+(i%3),cm=1+(i%3),ac=(i%10)+1,ay=(i%2)+1;antechinus_health(lc,bl,bw,fm,cm,ac,ay);}
ps("\nAntechinus market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=11+(i*1),bw=21+(i*2),fm=7+(i%2),cm=3+(i%2),ac=(i%4)+1,ay=(i%1)+1;antechinus_market(lc,bl,bw,fm,cm,ac,ay);}
ps("\n");antechinus_report();antechinus_state();ps("\n=== Demo Complete ===\n");return 0;}
