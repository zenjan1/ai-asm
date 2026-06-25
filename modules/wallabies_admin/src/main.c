/* wallabies_admin: Wallabies (Macropus) small macropod marsupials (v1.0)
 * Wallabies scrub, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, hop_m, wb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,hop_m,wb_idx,age_yr,active;} wlbs_t;
typedef struct{int n_scrub,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_hop,t_wb;} wlbs_state_t;
static wlbs_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static wlbs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wlbs_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int hm,int wb,int ay){if(*cnt>=mx)return -1;wlbs_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->hop_m=hm;x->wb_idx=wb;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WLBS] Wallaby ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" hm=");pi(hm);ps(" wb=");pi(wb);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int wallabies_init(void){if(init)return -1;st.n_scrub=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_hop=0;st.t_wb=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[WLBS] Wallabies initialized\n");return 0;}
int wallabies_scrub(int lc,int bl,int bw,int ec,int hm,int wb,int ay){return add(svl,&st.n_scrub,&st.t_ln,N,lc,bl,bw,ec,hm,wb,ay);}
int wallabies_feeding(int lc,int bl,int bw,int ec,int hm,int wb,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,hm,wb,ay);}
int wallabies_breeding(int lc,int bl,int bw,int ec,int hm,int wb,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,hm,wb,ay);}
int wallabies_health(int lc,int bl,int bw,int ec,int hm,int wb,int ay){return add(svh,&st.n_health,&st.t_hop,N-6,lc,bl,bw,ec,hm,wb,ay);}
int wallabies_market(int lc,int bl,int bw,int ec,int hm,int wb,int ay){return add(svm,&st.n_mkt,&st.t_wb,N-6,lc,bl,bw,ec,hm,wb,ay);}
void wallabies_report(void){ps("[WLBS] Scrub: ");pi(st.n_scrub);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Hop=");pi(st.t_hop);ps("\nMkt: ");pi(st.n_mkt);ps(" Wb=");pi(st.t_wb);ps("\n");}
void wallabies_state(void){ps("[WLBS] Scrub=");pi(st.n_scrub);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Wallabies Admin Demo ===\n\n");wallabies_init();
ps("Wallabies scrub...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*3),bw=5+(i*1),ec=7+(i%3),hm=3+(i%3),wb=(i%8)+1,ay=(i%6)+1;wallabies_scrub(lc,bl,bw,ec,hm,wb,ay);}
ps("\nWallabies feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=48+(i*2),bw=6+(i%1),ec=8+(i%2),hm=3+(i%2),wb=(i%6)+1,ay=(i%5)+1;wallabies_feeding(lc,bl,bw,ec,hm,wb,ay);}
ps("\nWallabies breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=50+(i*2),bw=6+(i%1),ec=8+(i%2),hm=4+(i%2),wb=(i%5)+1,ay=(i%4)+1;wallabies_breeding(lc,bl,bw,ec,hm,wb,ay);}
ps("\nWallabies health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=43+(i*4),bw=5+(i*1),ec=6+(i%3),hm=2+(i%3),wb=(i%10)+1,ay=(i%5)+1;wallabies_health(lc,bl,bw,ec,hm,wb,ay);}
ps("\nWallabies market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=52+(i*1),bw=7+(i%1),ec=9+(i%2),hm=4+(i%2),wb=(i%4)+1,ay=(i%4)+1;wallabies_market(lc,bl,bw,ec,hm,wb,ay);}
ps("\n");wallabies_report();wallabies_state();ps("\n=== Demo Complete ===\n");return 0;}
