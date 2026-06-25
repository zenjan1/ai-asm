/* numbats_admin: Numbats (Myrmecobius) striped termite-eating marsupials (v1.0)
 * Numbats forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, stripe_n, tongue_cm, nb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,stripe_n,tg_cm,nb_idx,age_yr,active;} nbts_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_stripe,t_tg,t_nb;} nbts_state_t;
static nbts_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static nbts_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nbts_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sn,int tc,int nb,int ay){if(*cnt>=mx)return -1;nbts_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->stripe_n=sn;x->tg_cm=tc;x->nb_idx=nb;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[NBTS] Numbat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sn=");pi(sn);ps(" tc=");pi(tc);ps(" nb=");pi(nb);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int numbats_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_stripe=0;st.t_tg=0;st.t_nb=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[NBTS] Numbats initialized\n");return 0;}
int numbats_forest(int lc,int bl,int bw,int sn,int tc,int nb,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,sn,tc,nb,ay);}
int numbats_feeding(int lc,int bl,int bw,int sn,int tc,int nb,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sn,tc,nb,ay);}
int numbats_breeding(int lc,int bl,int bw,int sn,int tc,int nb,int ay){return add(svb,&st.n_breed,&st.t_stripe,N-4,lc,bl,bw,sn,tc,nb,ay);}
int numbats_health(int lc,int bl,int bw,int sn,int tc,int nb,int ay){return add(svh,&st.n_health,&st.t_tg,N-6,lc,bl,bw,sn,tc,nb,ay);}
int numbats_market(int lc,int bl,int bw,int sn,int tc,int nb,int ay){return add(svm,&st.n_mkt,&st.t_nb,N-6,lc,bl,bw,sn,tc,nb,ay);}
void numbats_report(void){ps("[NBTS] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Stripe=");pi(st.t_stripe);ps("\nHealth: ");pi(st.n_health);ps(" Tg=");pi(st.t_tg);ps("\nMkt: ");pi(st.n_mkt);ps(" Nb=");pi(st.t_nb);ps("\n");}
void numbats_state(void){ps("[NBTS] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Numbats Admin Demo ===\n\n");numbats_init();
ps("Numbats forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*2),bw=200+(i*20),sn=4+(i%4),tc=8+(i*1),nb=(i%8)+1,ay=(i%6)+1;numbats_forest(lc,bl,bw,sn,tc,nb,ay);}
ps("\nNumbats feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=22+(i*1),bw=210+(i*15),sn=5+(i%3),tc=9+(i%1),nb=(i%6)+1,ay=(i%5)+1;numbats_feeding(lc,bl,bw,sn,tc,nb,ay);}
ps("\nNumbats breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=24+(i*1),bw=220+(i*10),sn=5+(i%3),tc=9+(i%1),nb=(i%5)+1,ay=(i%4)+1;numbats_breeding(lc,bl,bw,sn,tc,nb,ay);}
ps("\nNumbats health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=18+(i*3),bw=190+(i*25),sn=3+(i%5),tc=7+(i%2),nb=(i%10)+1,ay=(i%5)+1;numbats_health(lc,bl,bw,sn,tc,nb,ay);}
ps("\nNumbats market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=26+(i*1),bw=230+(i*8),sn=6+(i%2),tc=10+(i%1),nb=(i%4)+1,ay=(i%4)+1;numbats_market(lc,bl,bw,sn,tc,nb,ay);}
ps("\n");numbats_report();numbats_state();ps("\n=== Demo Complete ===\n");return 0;}
