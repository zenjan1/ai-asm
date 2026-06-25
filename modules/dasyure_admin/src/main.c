/* dasyure_admin: Dasyure (Dasyurus) carnivorous marsupial (v1.0)
 * Dasyure forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_mm, snout_cm, ds_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_mm,snout_cm,ds_idx,age_yr,active;} dsyr_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_snout,t_ds;} dsyr_state_t;
static dsyr_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static dsyr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dsyr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cm,int sc,int ds,int ay){if(*cnt>=mx)return -1;dsyr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_mm=cm;x->snout_cm=sc;x->ds_idx=ds;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DSYR] Dasyure ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cm=");pi(cm);ps(" sc=");pi(sc);ps(" ds=");pi(ds);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dasyure_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_snout=0;st.t_ds=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[DSYR] Dasyure initialized\n");return 0;}
int dasyure_forest(int lc,int bl,int bw,int cm,int sc,int ds,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,cm,sc,ds,ay);}
int dasyure_feeding(int lc,int bl,int bw,int cm,int sc,int ds,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cm,sc,ds,ay);}
int dasyure_breeding(int lc,int bl,int bw,int cm,int sc,int ds,int ay){return add(svb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cm,sc,ds,ay);}
int dasyure_health(int lc,int bl,int bw,int cm,int sc,int ds,int ay){return add(svh,&st.n_health,&st.t_snout,N-6,lc,bl,bw,cm,sc,ds,ay);}
int dasyure_market(int lc,int bl,int bw,int cm,int sc,int ds,int ay){return add(svm,&st.n_mkt,&st.t_ds,N-6,lc,bl,bw,cm,sc,ds,ay);}
void dasyure_report(void){ps("[DSYR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Snout=");pi(st.t_snout);ps("\nMkt: ");pi(st.n_mkt);ps(" Ds=");pi(st.t_ds);ps("\n");}
void dasyure_state(void){ps("[DSYR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dasyure Admin Demo ===\n\n");dasyure_init();
ps("Dasyure forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*2),bw=2+(i*1),cm=5+(i%3),sc=6+(i%2),ds=(i%8)+1,ay=(i%4)+1;dasyure_forest(lc,bl,bw,cm,sc,ds,ay);}
ps("\nDasyure feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*2),bw=2+(i*1),cm=6+(i%2),sc=7+(i%2),ds=(i%6)+1,ay=(i%3)+1;dasyure_feeding(lc,bl,bw,cm,sc,ds,ay);}
ps("\nDasyure breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*1),bw=3+(i*1),cm=6+(i%2),sc=7+(i%2),ds=(i%5)+1,ay=(i%3)+1;dasyure_breeding(lc,bl,bw,cm,sc,ds,ay);}
ps("\nDasyure health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*3),bw=2+(i*1),cm=4+(i%3),sc=5+(i%3),ds=(i%10)+1,ay=(i%4)+1;dasyure_health(lc,bl,bw,cm,sc,ds,ay);}
ps("\nDasyure market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*1),bw=3+(i*1),cm=7+(i%2),sc=8+(i%2),ds=(i%4)+1,ay=(i%3)+1;dasyure_market(lc,bl,bw,cm,sc,ds,ay);}
ps("\n");dasyure_report();dasyure_state();ps("\n=== Demo Complete ===\n");return 0;}
