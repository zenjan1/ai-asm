/* woylies_admin: Woylies (Bettongia penicillata) medium potoroid marsupials (v1.0)
 * Woylies forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, ear_cm, tail_cm, wo_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,tail_cm,wo_idx,age_yr,active;} woyl_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_tail,t_wo;} woyl_state_t;
static woyl_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static woyl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(woyl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int tc,int wo,int ay){if(*cnt>=mx)return -1;woyl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->tail_cm=tc;x->wo_idx=wo;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WOYL] Woylie ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" tc=");pi(tc);ps(" wo=");pi(wo);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int woylies_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_tail=0;st.t_wo=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[WOYL] Woylies initialized\n");return 0;}
int woylies_forest(int lc,int bl,int bw,int ec,int tc,int wo,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ec,tc,wo,ay);}
int woylies_feeding(int lc,int bl,int bw,int ec,int tc,int wo,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,tc,wo,ay);}
int woylies_breeding(int lc,int bl,int bw,int ec,int tc,int wo,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,tc,wo,ay);}
int woylies_health(int lc,int bl,int bw,int ec,int tc,int wo,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,ec,tc,wo,ay);}
int woylies_market(int lc,int bl,int bw,int ec,int tc,int wo,int ay){return add(svm,&st.n_mkt,&st.t_wo,N-6,lc,bl,bw,ec,tc,wo,ay);}
void woylies_report(void){ps("[WOYL] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Wo=");pi(st.t_wo);ps("\n");}
void woylies_state(void){ps("[WOYL] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Woylies Admin Demo ===\n\n");woylies_init();
ps("Woylies forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*2),bw=800+(i*50),ec=4+(i%3),tc=25+(i*2),wo=(i%8)+1,ay=(i%5)+1;woylies_forest(lc,bl,bw,ec,tc,wo,ay);}
ps("\nWoylies feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*2),bw=850+(i*40),ec=4+(i%3),tc=26+(i*2),wo=(i%6)+1,ay=(i%4)+1;woylies_feeding(lc,bl,bw,ec,tc,wo,ay);}
ps("\nWoylies breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*2),bw=900+(i*30),ec=5+(i%3),tc=27+(i*2),wo=(i%5)+1,ay=(i%3)+1;woylies_breeding(lc,bl,bw,ec,tc,wo,ay);}
ps("\nWoylies health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*3),bw=750+(i*60),ec=3+(i%3),tc=24+(i*2),wo=(i%10)+1,ay=(i%5)+1;woylies_health(lc,bl,bw,ec,tc,wo,ay);}
ps("\nWoylies market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*2),bw=950+(i*30),ec=5+(i%3),tc=28+(i*2),wo=(i%4)+1,ay=(i%3)+1;woylies_market(lc,bl,bw,ec,tc,wo,ay);}
ps("\n");woylies_report();woylies_state();ps("\n=== Demo Complete ===\n");return 0;}
