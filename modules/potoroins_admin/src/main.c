/* potoroins_admin: Potoroins small rat-kangaroo species (v1.0)
 * Potoroins forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, hop_dist, tail_cm, pt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hop_dist,tail_cm,pt_idx,age_yr,active;} ptin_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hop,t_tail,t_pt;} ptin_state_t;
static ptin_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static ptin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ptin_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hd,int tc,int pt,int ay){if(*cnt>=mx)return -1;ptin_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hop_dist=hd;x->tail_cm=tc;x->pt_idx=pt;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PTIN] Potoroin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hd=");pi(hd);ps(" tc=");pi(tc);ps(" pt=");pi(pt);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int potoroins_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hop=0;st.t_tail=0;st.t_pt=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PTIN] Potoroins initialized\n");return 0;}
int potoroins_forest(int lc,int bl,int bw,int hd,int tc,int pt,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,hd,tc,pt,ay);}
int potoroins_feeding(int lc,int bl,int bw,int hd,int tc,int pt,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hd,tc,pt,ay);}
int potoroins_breeding(int lc,int bl,int bw,int hd,int tc,int pt,int ay){return add(svb,&st.n_breed,&st.t_hop,N-4,lc,bl,bw,hd,tc,pt,ay);}
int potoroins_health(int lc,int bl,int bw,int hd,int tc,int pt,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,hd,tc,pt,ay);}
int potoroins_market(int lc,int bl,int bw,int hd,int tc,int pt,int ay){return add(svm,&st.n_mkt,&st.t_pt,N-6,lc,bl,bw,hd,tc,pt,ay);}
void potoroins_report(void){ps("[PTIN] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hop=");pi(st.t_hop);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Pt=");pi(st.t_pt);ps("\n");}
void potoroins_state(void){ps("[PTIN] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Potoroins Admin Demo ===\n\n");potoroins_init();
ps("Potoroins forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*1),bw=400+(i*30),hd=60+(i*10),tc=18+(i*1),pt=(i%8)+1,ay=(i%5)+1;potoroins_forest(lc,bl,bw,hd,tc,pt,ay);}
ps("\nPotoroins feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=26+(i*1),bw=430+(i*25),hd=65+(i*10),tc=19+(i*1),pt=(i%6)+1,ay=(i%4)+1;potoroins_feeding(lc,bl,bw,hd,tc,pt,ay);}
ps("\nPotoroins breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=27+(i*1),bw=460+(i*20),hd=70+(i*10),tc=19+(i*1),pt=(i%5)+1,ay=(i%3)+1;potoroins_breeding(lc,bl,bw,hd,tc,pt,ay);}
ps("\nPotoroins health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=24+(i*2),bw=370+(i*35),hd=55+(i*10),tc=17+(i*1),pt=(i%10)+1,ay=(i%5)+1;potoroins_health(lc,bl,bw,hd,tc,pt,ay);}
ps("\nPotoroins market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=28+(i*1),bw=490+(i*20),hd=75+(i*10),tc=20+(i*1),pt=(i%4)+1,ay=(i%3)+1;potoroins_market(lc,bl,bw,hd,tc,pt,ay);}
ps("\n");potoroins_report();potoroins_state();ps("\n=== Demo Complete ===\n");return 0;}
