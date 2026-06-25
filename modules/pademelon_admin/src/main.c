/* pademelon_admin: Pademelon (Thylogale) small wallaby marsupial (v1.0)
 * Pademelon forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, hop_cm, pm_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,hop_cm,pm_idx,age_yr,active;} pdmn_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_hop,t_pm;} pdmn_state_t;
static pdmn_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static pdmn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pdmn_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int hc,int pm,int ay){if(*cnt>=mx)return -1;pdmn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->hop_cm=hc;x->pm_idx=pm;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PDMN] Pademelon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" hc=");pi(hc);ps(" pm=");pi(pm);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int pademelon_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_hop=0;st.t_pm=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PDMN] Pademelon initialized\n");return 0;}
int pademelon_forest(int lc,int bl,int bw,int ec,int hc,int pm,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ec,hc,pm,ay);}
int pademelon_feeding(int lc,int bl,int bw,int ec,int hc,int pm,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,hc,pm,ay);}
int pademelon_breeding(int lc,int bl,int bw,int ec,int hc,int pm,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,hc,pm,ay);}
int pademelon_health(int lc,int bl,int bw,int ec,int hc,int pm,int ay){return add(svh,&st.n_health,&st.t_hop,N-6,lc,bl,bw,ec,hc,pm,ay);}
int pademelon_market(int lc,int bl,int bw,int ec,int hc,int pm,int ay){return add(svm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,ec,hc,pm,ay);}
void pademelon_report(void){ps("[PDMN] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Hop=");pi(st.t_hop);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void pademelon_state(void){ps("[PDMN] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Pademelon Admin Demo ===\n\n");pademelon_init();
ps("Pademelon forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=5+(i*1),ec=6+(i%3),hc=50+(i*3),pm=(i%8)+1,ay=(i%6)+1;pademelon_forest(lc,bl,bw,ec,hc,pm,ay);}
ps("\nPademelon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=5+(i*1),ec=7+(i%2),hc=52+(i*2),pm=(i%6)+1,ay=(i%5)+1;pademelon_feeding(lc,bl,bw,ec,hc,pm,ay);}
ps("\nPademelon breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=44+(i*2),bw=6+(i*1),ec=7+(i%2),hc=54+(i*2),pm=(i%5)+1,ay=(i%4)+1;pademelon_breeding(lc,bl,bw,ec,hc,pm,ay);}
ps("\nPademelon health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=5+(i*1),ec=5+(i%3),hc=48+(i*3),pm=(i%10)+1,ay=(i%5)+1;pademelon_health(lc,bl,bw,ec,hc,pm,ay);}
ps("\nPademelon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=46+(i*1),bw=6+(i*1),ec=8+(i%2),hc=56+(i*2),pm=(i%4)+1,ay=(i%4)+1;pademelon_market(lc,bl,bw,ec,hc,pm,ay);}
ps("\n");pademelon_report();pademelon_state();ps("\n=== Demo Complete ===\n");return 0;}
