/* bandicoot_bettong_admin: Bandicoot-bettong hybrid marsupials (v1.0)
 * Bandicoot-bettong forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, nose_len, tail_cm, btb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,nose_len,tail_cm,btb_idx,age_yr,active;} btbt_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_nose,t_tail,t_bt;} btbt_state_t;
static btbt_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static btbt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(btbt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nl,int tc,int bt,int ay){if(*cnt>=mx)return -1;btbt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->nose_len=nl;x->tail_cm=tc;x->btb_idx=bt;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BTBT] Bandicoot-bettong ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nl=");pi(nl);ps(" tc=");pi(tc);ps(" bt=");pi(bt);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bandicoot_bettong_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_nose=0;st.t_tail=0;st.t_bt=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BTBT] Bandicoot-bettong initialized\n");return 0;}
int bandicoot_bettong_forest(int lc,int bl,int bw,int nl,int tc,int bt,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,nl,tc,bt,ay);}
int bandicoot_bettong_feeding(int lc,int bl,int bw,int nl,int tc,int bt,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nl,tc,bt,ay);}
int bandicoot_bettong_breeding(int lc,int bl,int bw,int nl,int tc,int bt,int ay){return add(svb,&st.n_breed,&st.t_nose,N-4,lc,bl,bw,nl,tc,bt,ay);}
int bandicoot_bettong_health(int lc,int bl,int bw,int nl,int tc,int bt,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,nl,tc,bt,ay);}
int bandicoot_bettong_market(int lc,int bl,int bw,int nl,int tc,int bt,int ay){return add(svm,&st.n_mkt,&st.t_bt,N-6,lc,bl,bw,nl,tc,bt,ay);}
void bandicoot_bettong_report(void){ps("[BTBT] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Nose=");pi(st.t_nose);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Bt=");pi(st.t_bt);ps("\n");}
void bandicoot_bettong_state(void){ps("[BTBT] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bandicoot-Bettong Admin Demo ===\n\n");bandicoot_bettong_init();
ps("Bandicoot-bettong forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*2),bw=1000+(i*70),nl=8+(i%4),tc=20+(i*1),bt=(i%8)+1,ay=(i%5)+1;bandicoot_bettong_forest(lc,bl,bw,nl,tc,bt,ay);}
ps("\nBandicoot-bettong feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*2),bw=1050+(i*60),nl=8+(i%4),tc=21+(i*1),bt=(i%6)+1,ay=(i%4)+1;bandicoot_bettong_feeding(lc,bl,bw,nl,tc,bt,ay);}
ps("\nBandicoot-bettong breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=39+(i*2),bw=1100+(i*50),nl=9+(i%4),tc=21+(i*1),bt=(i%5)+1,ay=(i%3)+1;bandicoot_bettong_breeding(lc,bl,bw,nl,tc,bt,ay);}
ps("\nBandicoot-bettong health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=33+(i*3),bw=950+(i*80),nl=7+(i%4),tc=19+(i*1),bt=(i%10)+1,ay=(i%5)+1;bandicoot_bettong_health(lc,bl,bw,nl,tc,bt,ay);}
ps("\nBandicoot-bettong market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=41+(i*2),bw=1150+(i*50),nl=9+(i%4),tc=22+(i*1),bt=(i%4)+1,ay=(i%3)+1;bandicoot_bettong_market(lc,bl,bw,nl,tc,bt,ay);}
ps("\n");bandicoot_bettong_report();bandicoot_bettong_state();ps("\n=== Demo Complete ===\n");return 0;}
