/* xantus_admin: Xantus small exotic mammals (v1.0)
 * Xantus forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, spine_ct, tail_cm, xt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,spine_ct,tail_cm,xt_idx,age_yr,active;} xant_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_spine,t_tail,t_xt;} xant_state_t;
static xant_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static xant_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(xant_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int tc,int xt,int ay){if(*cnt>=mx)return -1;xant_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->spine_ct=sc;x->tail_cm=tc;x->xt_idx=xt;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[XANT] Xantus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" tc=");pi(tc);ps(" xt=");pi(xt);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int xantus_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_spine=0;st.t_tail=0;st.t_xt=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[XANT] Xantus initialized\n");return 0;}
int xantus_forest(int lc,int bl,int bw,int sc,int tc,int xt,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,sc,tc,xt,ay);}
int xantus_feeding(int lc,int bl,int bw,int sc,int tc,int xt,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,tc,xt,ay);}
int xantus_breeding(int lc,int bl,int bw,int sc,int tc,int xt,int ay){return add(svb,&st.n_breed,&st.t_spine,N-4,lc,bl,bw,sc,tc,xt,ay);}
int xantus_health(int lc,int bl,int bw,int sc,int tc,int xt,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,sc,tc,xt,ay);}
int xantus_market(int lc,int bl,int bw,int sc,int tc,int xt,int ay){return add(svm,&st.n_mkt,&st.t_xt,N-6,lc,bl,bw,sc,tc,xt,ay);}
void xantus_report(void){ps("[XANT] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Spine=");pi(st.t_spine);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Xt=");pi(st.t_xt);ps("\n");}
void xantus_state(void){ps("[XANT] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Xantus Admin Demo ===\n\n");xantus_init();
ps("Xantus forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=12+(i*1),bw=25+(i*3),sc=20+(i%15),tc=8+(i*1),xt=(i%8)+1,ay=(i%5)+1;xantus_forest(lc,bl,bw,sc,tc,xt,ay);}
ps("\nXantus feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=13+(i*1),bw=28+(i*3),sc=22+(i%15),tc=9+(i*1),xt=(i%6)+1,ay=(i%4)+1;xantus_feeding(lc,bl,bw,sc,tc,xt,ay);}
ps("\nXantus breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=14+(i*1),bw=31+(i*2),sc=24+(i%15),tc=9+(i*1),xt=(i%5)+1,ay=(i%3)+1;xantus_breeding(lc,bl,bw,sc,tc,xt,ay);}
ps("\nXantus health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=11+(i*2),bw=22+(i*4),sc=18+(i%15),tc=7+(i*1),xt=(i%10)+1,ay=(i%5)+1;xantus_health(lc,bl,bw,sc,tc,xt,ay);}
ps("\nXantus market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=15+(i*1),bw=34+(i*2),sc=26+(i%15),tc=10+(i*1),xt=(i%4)+1,ay=(i%3)+1;xantus_market(lc,bl,bw,sc,tc,xt,ay);}
ps("\n");xantus_report();xantus_state();ps("\n=== Demo Complete ===\n");return 0;}
