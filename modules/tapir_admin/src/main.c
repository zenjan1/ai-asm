/* tapir_admin: Tapir (Tapirus terrestris) large browsing mammal (v1.0)
 * Tapir forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, snout_cm, swim_speed, tp_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,sn_cm,sw_sp,tp_idx,age_yr,active;} tapr_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_sn,t_sw,t_tp;} tapr_state_t;
static tapr_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static tapr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tapr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int ss,int ti,int ay){if(*cnt>=mx)return -1;tapr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->sn_cm=sc;x->sw_sp=ss;x->tp_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TAPR] Tapir ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" ss=");pi(ss);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int tapir_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_sn=0;st.t_sw=0;st.t_tp=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[TAPR] Tapir initialized\n");return 0;}
int tapir_forest(int lc,int bl,int bw,int sc,int ss,int ti,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,sc,ss,ti,ay);}
int tapir_feeding(int lc,int bl,int bw,int sc,int ss,int ti,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,ss,ti,ay);}
int tapir_breeding(int lc,int bl,int bw,int sc,int ss,int ti,int ay){return add(svb,&st.n_breed,&st.t_sn,N-4,lc,bl,bw,sc,ss,ti,ay);}
int tapir_health(int lc,int bl,int bw,int sc,int ss,int ti,int ay){return add(svh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,sc,ss,ti,ay);}
int tapir_market(int lc,int bl,int bw,int sc,int ss,int ti,int ay){return add(svm,&st.n_mkt,&st.t_tp,N-6,lc,bl,bw,sc,ss,ti,ay);}
void tapir_report(void){ps("[TAPR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Sn=");pi(st.t_sn);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Tp=");pi(st.t_tp);ps("\n");}
void tapir_state(void){ps("[TAPR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tapir Admin Demo ===\n\n");tapir_init();
ps("Tapir forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=180+(i*10),bw=150+(i*15),sc=20+(i*2),ss=8+(i*1),ti=(i%8)+1,ay=(i%18)+1;tapir_forest(lc,bl,bw,sc,ss,ti,ay);}
ps("\nTapir feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=185+(i*8),bw=155+(i*12),sc=21+(i*1),ss=9+(i*1),ti=(i%6)+1,ay=(i%15)+1;tapir_feeding(lc,bl,bw,sc,ss,ti,ay);}
ps("\nTapir breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=190+(i*6),bw=160+(i*10),sc=22+(i*2),ss=7+(i*1),ti=(i%5)+1,ay=(i%12)+1;tapir_breeding(lc,bl,bw,sc,ss,ti,ay);}
ps("\nTapir health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=175+(i*12),bw=145+(i*18),sc=19+(i*3),ss=6+(i*2),ti=(i%10)+1,ay=(i%10)+1;tapir_health(lc,bl,bw,sc,ss,ti,ay);}
ps("\nTapir market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=195+(i*5),bw=165+(i*8),sc=23+(i*1),ss=10+(i*1),ti=(i%4)+1,ay=(i%8)+1;tapir_market(lc,bl,bw,sc,ss,ti,ay);}
ps("\n");tapir_report();tapir_state();ps("\n=== Demo Complete ===\n");return 0;}
