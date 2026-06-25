/* potoroin_admin: Potoroin (Potoroin) rat-kangaroos (v1.0)
 * Potoroin forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, nose_cm, tail_cm, pt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,nose_cm,tail_cm,pt_idx,age_yr,active;} ptro_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_nose,t_tail,t_pt;} ptro_state_t;
static ptro_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static ptro_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ptro_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nc,int tc,int pt,int ay){if(*cnt>=mx)return -1;ptro_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->nose_cm=nc;x->tail_cm=tc;x->pt_idx=pt;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PTRO] Potoroin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nc=");pi(nc);ps(" tc=");pi(tc);ps(" pt=");pi(pt);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int potoroin_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_nose=0;st.t_tail=0;st.t_pt=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PTRO] Potoroin initialized\n");return 0;}
int potoroin_forest(int lc,int bl,int bw,int nc,int tc,int pt,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,nc,tc,pt,ay);}
int potoroin_feeding(int lc,int bl,int bw,int nc,int tc,int pt,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nc,tc,pt,ay);}
int potoroin_breeding(int lc,int bl,int bw,int nc,int tc,int pt,int ay){return add(svb,&st.n_breed,&st.t_nose,N-4,lc,bl,bw,nc,tc,pt,ay);}
int potoroin_health(int lc,int bl,int bw,int nc,int tc,int pt,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,nc,tc,pt,ay);}
int potoroin_market(int lc,int bl,int bw,int nc,int tc,int pt,int ay){return add(svm,&st.n_mkt,&st.t_pt,N-6,lc,bl,bw,nc,tc,pt,ay);}
void potoroin_report(void){ps("[PTRO] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Nose=");pi(st.t_nose);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Pt=");pi(st.t_pt);ps("\n");}
void potoroin_state(void){ps("[PTRO] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Potoroin Admin Demo ===\n\n");potoroin_init();
ps("Potoroin forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*2),bw=200+(i*15),nc=3+(i%3),tc=12+(i*2),pt=(i%8)+1,ay=(i%5)+1;potoroin_forest(lc,bl,bw,nc,tc,pt,ay);}
ps("\nPotoroin feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*1),bw=210+(i*12),nc=4+(i%2),tc=13+(i%1),pt=(i%6)+1,ay=(i%4)+1;potoroin_feeding(lc,bl,bw,nc,tc,pt,ay);}
ps("\nPotoroin breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*1),bw=220+(i*10),nc=4+(i%2),tc=14+(i%1),pt=(i%5)+1,ay=(i%3)+1;potoroin_breeding(lc,bl,bw,nc,tc,pt,ay);}
ps("\nPotoroin health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*3),bw=190+(i*20),nc=2+(i%3),tc=11+(i*2),pt=(i%10)+1,ay=(i%5)+1;potoroin_health(lc,bl,bw,nc,tc,pt,ay);}
ps("\nPotoroin market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*1),bw=240+(i*8),nc=5+(i%2),tc=15+(i%1),pt=(i%4)+1,ay=(i%3)+1;potoroin_market(lc,bl,bw,nc,tc,pt,ay);}
ps("\n");potoroin_report();potoroin_state();ps("\n=== Demo Complete ===\n");return 0;}
