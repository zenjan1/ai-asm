/* planigale_admin: Planigale (Planigale) tiny marsupial (v1.0)
 * Planigale grassland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, head_mm, tail_cm, pl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,head_mm,tail_cm,pl_idx,age_yr,active;} plng_t;
typedef struct{int n_grass,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_head,t_tail,t_pl;} plng_state_t;
static plng_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static plng_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(plng_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hm,int tc,int pl,int ay){if(*cnt>=mx)return -1;plng_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->head_mm=hm;x->tail_cm=tc;x->pl_idx=pl;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PLNG] Planigale ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hm=");pi(hm);ps(" tc=");pi(tc);ps(" pl=");pi(pl);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int planigale_init(void){if(init)return -1;st.n_grass=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_head=0;st.t_tail=0;st.t_pl=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PLNG] Planigale initialized\n");return 0;}
int planigale_grassland(int lc,int bl,int bw,int hm,int tc,int pl,int ay){return add(svl,&st.n_grass,&st.t_ln,N,lc,bl,bw,hm,tc,pl,ay);}
int planigale_feeding(int lc,int bl,int bw,int hm,int tc,int pl,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hm,tc,pl,ay);}
int planigale_breeding(int lc,int bl,int bw,int hm,int tc,int pl,int ay){return add(svb,&st.n_breed,&st.t_head,N-4,lc,bl,bw,hm,tc,pl,ay);}
int planigale_health(int lc,int bl,int bw,int hm,int tc,int pl,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,hm,tc,pl,ay);}
int planigale_market(int lc,int bl,int bw,int hm,int tc,int pl,int ay){return add(svm,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,hm,tc,pl,ay);}
void planigale_report(void){ps("[PLNG] Grass: ");pi(st.n_grass);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Head=");pi(st.t_head);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void planigale_state(void){ps("[PLNG] Grass=");pi(st.n_grass);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Planigale Admin Demo ===\n\n");planigale_init();
ps("Planigale grassland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=5+(i*1),bw=4+(i*1),hm=8+(i%3),tc=4+(i%2),pl=(i%8)+1,ay=(i%3)+1;planigale_grassland(lc,bl,bw,hm,tc,pl,ay);}
ps("\nPlanigale feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=6+(i*1),bw=5+(i*1),hm=9+(i%2),tc=5+(i%2),pl=(i%6)+1,ay=(i%3)+1;planigale_feeding(lc,bl,bw,hm,tc,pl,ay);}
ps("\nPlanigale breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=7+(i*1),bw=5+(i*1),hm=9+(i%2),tc=5+(i%2),pl=(i%5)+1,ay=(i%2)+1;planigale_breeding(lc,bl,bw,hm,tc,pl,ay);}
ps("\nPlanigale health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=4+(i*1),bw=3+(i*1),hm=7+(i%3),tc=3+(i%2),pl=(i%10)+1,ay=(i%3)+1;planigale_health(lc,bl,bw,hm,tc,pl,ay);}
ps("\nPlanigale market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=8+(i*1),bw=6+(i*1),hm=10+(i%2),tc=6+(i%2),pl=(i%4)+1,ay=(i%2)+1;planigale_market(lc,bl,bw,hm,tc,pl,ay);}
ps("\n");planigale_report();planigale_state();ps("\n=== Demo Complete ===\n");return 0;}
