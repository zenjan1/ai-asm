/* echymipera_admin: Echymipera (Echymipera) echidna bandicoots (v1.0)
 * Echymipera forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, snout_cm, tail_cm, ec_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,snout_cm,tail_cm,ec_idx,age_yr,active;} ecmp_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_snout,t_tail,t_ec;} ecmp_state_t;
static ecmp_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static ecmp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ecmp_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int tc,int ec,int ay){if(*cnt>=mx)return -1;ecmp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->snout_cm=sc;x->tail_cm=tc;x->ec_idx=ec;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ECMP] Echymipera ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" tc=");pi(tc);ps(" ec=");pi(ec);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int echymipera_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_snout=0;st.t_tail=0;st.t_ec=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ECMP] Echymipera initialized\n");return 0;}
int echymipera_forest(int lc,int bl,int bw,int sc,int tc,int ec,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,sc,tc,ec,ay);}
int echymipera_feeding(int lc,int bl,int bw,int sc,int tc,int ec,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,tc,ec,ay);}
int echymipera_breeding(int lc,int bl,int bw,int sc,int tc,int ec,int ay){return add(svb,&st.n_breed,&st.t_snout,N-4,lc,bl,bw,sc,tc,ec,ay);}
int echymipera_health(int lc,int bl,int bw,int sc,int tc,int ec,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,sc,tc,ec,ay);}
int echymipera_market(int lc,int bl,int bw,int sc,int tc,int ec,int ay){return add(svm,&st.n_mkt,&st.t_ec,N-6,lc,bl,bw,sc,tc,ec,ay);}
void echymipera_report(void){ps("[ECMP] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Snout=");pi(st.t_snout);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Ec=");pi(st.t_ec);ps("\n");}
void echymipera_state(void){ps("[ECMP] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Echymipera Admin Demo ===\n\n");echymipera_init();
ps("Echymipera forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*2),bw=2+(i*1),sc=8+(i%3),tc=12+(i*2),ec=(i%8)+1,ay=(i%5)+1;echymipera_forest(lc,bl,bw,sc,tc,ec,ay);}
ps("\nEchymipera feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*1),bw=3+(i*1),sc=9+(i%2),tc=13+(i%1),ec=(i%6)+1,ay=(i%4)+1;echymipera_feeding(lc,bl,bw,sc,tc,ec,ay);}
ps("\nEchymipera breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*1),bw=3+(i*1),sc=10+(i%2),tc=15+(i%1),ec=(i%5)+1,ay=(i%3)+1;echymipera_breeding(lc,bl,bw,sc,tc,ec,ay);}
ps("\nEchymipera health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*3),bw=2+(i*1),sc=7+(i%3),tc=11+(i*2),ec=(i%10)+1,ay=(i%5)+1;echymipera_health(lc,bl,bw,sc,tc,ec,ay);}
ps("\nEchymipera market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=48+(i*1),bw=4+(i*1),sc=11+(i%2),tc=16+(i%1),ec=(i%4)+1,ay=(i%3)+1;echymipera_market(lc,bl,bw,sc,tc,ec,ay);}
ps("\n");echymipera_report();echymipera_state();ps("\n=== Demo Complete ===\n");return 0;}
