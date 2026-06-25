/* planigale_mouse_admin: Planigale-mouse flat-headed mammals (v1.0)
 * Planigale-mouse forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, head_width, tail_cm, pmm_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,head_width,tail_cm,pmm_idx,age_yr,active;} pmmm_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_head,t_tail,t_pm;} pmmm_state_t;
static pmmm_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static pmmm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pmmm_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hw,int tc,int pm,int ay){if(*cnt>=mx)return -1;pmmm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->head_width=hw;x->tail_cm=tc;x->pmm_idx=pm;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PMMM] Planigale-mouse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hw=");pi(hw);ps(" tc=");pi(tc);ps(" pm=");pi(pm);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int planigale_mouse_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_head=0;st.t_tail=0;st.t_pm=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PMMM] Planigale-mouse initialized\n");return 0;}
int planigale_mouse_forest(int lc,int bl,int bw,int hw,int tc,int pm,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,hw,tc,pm,ay);}
int planigale_mouse_feeding(int lc,int bl,int bw,int hw,int tc,int pm,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hw,tc,pm,ay);}
int planigale_mouse_breeding(int lc,int bl,int bw,int hw,int tc,int pm,int ay){return add(svb,&st.n_breed,&st.t_head,N-4,lc,bl,bw,hw,tc,pm,ay);}
int planigale_mouse_health(int lc,int bl,int bw,int hw,int tc,int pm,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,hw,tc,pm,ay);}
int planigale_mouse_market(int lc,int bl,int bw,int hw,int tc,int pm,int ay){return add(svm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,hw,tc,pm,ay);}
void planigale_mouse_report(void){ps("[PMMM] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Head=");pi(st.t_head);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void planigale_mouse_state(void){ps("[PMMM] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Planigale-Mouse Admin Demo ===\n\n");planigale_mouse_init();
ps("Planigale-mouse forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=5+(i*1),bw=4+(i*1),hw=3+(i%2),tc=4+(i*1),pm=(i%8)+1,ay=(i%5)+1;planigale_mouse_forest(lc,bl,bw,hw,tc,pm,ay);}
ps("\nPlanigale-mouse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=6+(i*1),bw=5+(i*1),hw=3+(i%2),tc=4+(i*1),pm=(i%6)+1,ay=(i%4)+1;planigale_mouse_feeding(lc,bl,bw,hw,tc,pm,ay);}
ps("\nPlanigale-mouse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=7+(i*1),bw=6+(i*1),hw=4+(i%2),tc=5+(i*1),pm=(i%5)+1,ay=(i%3)+1;planigale_mouse_breeding(lc,bl,bw,hw,tc,pm,ay);}
ps("\nPlanigale-mouse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=4+(i*1),bw=3+(i*1),hw=2+(i%2),tc=3+(i*1),pm=(i%10)+1,ay=(i%5)+1;planigale_mouse_health(lc,bl,bw,hw,tc,pm,ay);}
ps("\nPlanigale-mouse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=8+(i*1),bw=7+(i*1),hw=4+(i%2),tc=5+(i*1),pm=(i%4)+1,ay=(i%3)+1;planigale_mouse_market(lc,bl,bw,hw,tc,pm,ay);}
ps("\n");planigale_mouse_report();planigale_mouse_state();ps("\n=== Demo Complete ===\n");return 0;}
