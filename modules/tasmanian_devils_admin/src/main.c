/* tasmanian_devils_admin: Tasmanian Devils (Sarcophilus) carnivorous marsupials (v1.0)
 * Tasmanian Devils forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, skull_cm, tail_cm, td_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,skull_cm,tail_cm,td_idx,age_yr,active;} tdvl_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_skull,t_tail,t_td;} tdvl_state_t;
static tdvl_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static tdvl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tdvl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int tc,int td,int ay){if(*cnt>=mx)return -1;tdvl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->skull_cm=sc;x->tail_cm=tc;x->td_idx=td;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TDVL] Tas Devil ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" tc=");pi(tc);ps(" td=");pi(td);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int tasmanian_devils_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_skull=0;st.t_tail=0;st.t_td=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[TDVL] Tasmanian Devils initialized\n");return 0;}
int tasmanian_devils_forest(int lc,int bl,int bw,int sc,int tc,int td,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,sc,tc,td,ay);}
int tasmanian_devils_feeding(int lc,int bl,int bw,int sc,int tc,int td,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,tc,td,ay);}
int tasmanian_devils_breeding(int lc,int bl,int bw,int sc,int tc,int td,int ay){return add(svb,&st.n_breed,&st.t_skull,N-4,lc,bl,bw,sc,tc,td,ay);}
int tasmanian_devils_health(int lc,int bl,int bw,int sc,int tc,int td,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,sc,tc,td,ay);}
int tasmanian_devils_market(int lc,int bl,int bw,int sc,int tc,int td,int ay){return add(svm,&st.n_mkt,&st.t_td,N-6,lc,bl,bw,sc,tc,td,ay);}
void tasmanian_devils_report(void){ps("[TDVL] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Skull=");pi(st.t_skull);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Td=");pi(st.t_td);ps("\n");}
void tasmanian_devils_state(void){ps("[TDVL] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tasmanian Devils Admin Demo ===\n\n");tasmanian_devils_init();
ps("Tas Devils forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=65+(i*2),bw=8+(i*1),sc=15+(i%3),tc=28+(i*2),td=(i%8)+1,ay=(i%5)+1;tasmanian_devils_forest(lc,bl,bw,sc,tc,td,ay);}
ps("\nTas Devils feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=67+(i*2),bw=9+(i*1),sc=16+(i%2),tc=29+(i*1),td=(i%6)+1,ay=(i%4)+1;tasmanian_devils_feeding(lc,bl,bw,sc,tc,td,ay);}
ps("\nTas Devils breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=70+(i*1),bw=10+(i*1),sc=17+(i%2),tc=31+(i*1),td=(i%5)+1,ay=(i%3)+1;tasmanian_devils_breeding(lc,bl,bw,sc,tc,td,ay);}
ps("\nTas Devils health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=62+(i*3),bw=7+(i*1),sc=14+(i%3),tc=26+(i*2),td=(i%10)+1,ay=(i%5)+1;tasmanian_devils_health(lc,bl,bw,sc,tc,td,ay);}
ps("\nTas Devils market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=72+(i*1),bw=11+(i*1),sc=18+(i%2),tc=33+(i*1),td=(i%4)+1,ay=(i%3)+1;tasmanian_devils_market(lc,bl,bw,sc,tc,td,ay);}
ps("\n");tasmanian_devils_report();tasmanian_devils_state();ps("\n=== Demo Complete ===\n");return 0;}
