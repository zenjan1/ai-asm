/* bandicoots_admin: Bandicoots (Perameles) omnivorous marsupials (v1.0)
 * Bandicoots forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, snout_cm, claw_mm, bc_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,snout_cm,claw_mm,bc_idx,age_yr,active;} bdcs_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_snout,t_claw,t_bc;} bdcs_state_t;
static bdcs_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static bdcs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bdcs_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int cm,int bc,int ay){if(*cnt>=mx)return -1;bdcs_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->snout_cm=sc;x->claw_mm=cm;x->bc_idx=bc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BDCS] Bandicoot ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" cm=");pi(cm);ps(" bc=");pi(bc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bandicoots_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_snout=0;st.t_claw=0;st.t_bc=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BDCS] Bandicoots initialized\n");return 0;}
int bandicoots_forest(int lc,int bl,int bw,int sc,int cm,int bc,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,sc,cm,bc,ay);}
int bandicoots_feeding(int lc,int bl,int bw,int sc,int cm,int bc,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,cm,bc,ay);}
int bandicoots_breeding(int lc,int bl,int bw,int sc,int cm,int bc,int ay){return add(svb,&st.n_breed,&st.t_snout,N-4,lc,bl,bw,sc,cm,bc,ay);}
int bandicoots_health(int lc,int bl,int bw,int sc,int cm,int bc,int ay){return add(svh,&st.n_health,&st.t_claw,N-6,lc,bl,bw,sc,cm,bc,ay);}
int bandicoots_market(int lc,int bl,int bw,int sc,int cm,int bc,int ay){return add(svm,&st.n_mkt,&st.t_bc,N-6,lc,bl,bw,sc,cm,bc,ay);}
void bandicoots_report(void){ps("[BDCS] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Snout=");pi(st.t_snout);ps("\nHealth: ");pi(st.n_health);ps(" Claw=");pi(st.t_claw);ps("\nMkt: ");pi(st.n_mkt);ps(" Bc=");pi(st.t_bc);ps("\n");}
void bandicoots_state(void){ps("[BDCS] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bandicoots Admin Demo ===\n\n");bandicoots_init();
ps("Bandicoots forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*2),bw=1+(i*1),sc=5+(i%3),cm=4+(i%3),bc=(i%8)+1,ay=(i%5)+1;bandicoots_forest(lc,bl,bw,sc,cm,bc,ay);}
ps("\nBandicoots feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*2),bw=1+(i*1),sc=6+(i%2),cm=5+(i%2),bc=(i%6)+1,ay=(i%4)+1;bandicoots_feeding(lc,bl,bw,sc,cm,bc,ay);}
ps("\nBandicoots breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*1),bw=2+(i*1),sc=6+(i%2),cm=5+(i%2),bc=(i%5)+1,ay=(i%4)+1;bandicoots_breeding(lc,bl,bw,sc,cm,bc,ay);}
ps("\nBandicoots health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*3),bw=1+(i*1),sc=4+(i%3),cm=3+(i%3),bc=(i%10)+1,ay=(i%5)+1;bandicoots_health(lc,bl,bw,sc,cm,bc,ay);}
ps("\nBandicoots market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*1),bw=2+(i*1),sc=7+(i%2),cm=6+(i%2),bc=(i%4)+1,ay=(i%3)+1;bandicoots_market(lc,bl,bw,sc,cm,bc,ay);}
ps("\n");bandicoots_report();bandicoots_state();ps("\n=== Demo Complete ===\n");return 0;}
