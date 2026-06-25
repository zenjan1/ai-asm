/* numbat_admin: Numbat (Myrmecobius fasciatus) striped termite-eating marsupial (v1.0)
 * Numbat forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, tongue_cm, nm_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,tg_cm,nm_idx,age_yr,active;} numb_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_tg,t_nm;} numb_state_t;
static numb_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static numb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(numb_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int tgc,int ni,int ay){if(*cnt>=mx)return -1;numb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->tg_cm=tgc;x->nm_idx=ni;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[NUMB] Numbat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" tgc=");pi(tgc);ps(" ni=");pi(ni);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int numbat_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_tg=0;st.t_nm=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[NUMB] Numbat initialized\n");return 0;}
int numbat_forest(int lc,int bl,int bw,int tc,int tgc,int ni,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,tc,tgc,ni,ay);}
int numbat_feeding(int lc,int bl,int bw,int tc,int tgc,int ni,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,tgc,ni,ay);}
int numbat_breeding(int lc,int bl,int bw,int tc,int tgc,int ni,int ay){return add(svb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,tgc,ni,ay);}
int numbat_health(int lc,int bl,int bw,int tc,int tgc,int ni,int ay){return add(svh,&st.n_health,&st.t_tg,N-6,lc,bl,bw,tc,tgc,ni,ay);}
int numbat_market(int lc,int bl,int bw,int tc,int tgc,int ni,int ay){return add(svm,&st.n_mkt,&st.t_nm,N-6,lc,bl,bw,tc,tgc,ni,ay);}
void numbat_report(void){ps("[NUMB] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Tg=");pi(st.t_tg);ps("\nMkt: ");pi(st.n_mkt);ps(" Nm=");pi(st.t_nm);ps("\n");}
void numbat_state(void){ps("[NUMB] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Numbat Admin Demo ===\n\n");numbat_init();
ps("Numbat forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*2),bw=200+(i*20),tc=12+(i*1),tgc=8+(i*1),ni=(i%8)+1,ay=(i%8)+1;numbat_forest(lc,bl,bw,tc,tgc,ni,ay);}
ps("\nNumbat feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=22+(i*1),bw=210+(i*15),tc=13+(i*1),tgc=9+(i*1),ni=(i%6)+1,ay=(i%7)+1;numbat_feeding(lc,bl,bw,tc,tgc,ni,ay);}
ps("\nNumbat breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=24+(i*1),bw=220+(i*10),tc=14+(i*1),tgc=8+(i*1),ni=(i%5)+1,ay=(i%6)+1;numbat_breeding(lc,bl,bw,tc,tgc,ni,ay);}
ps("\nNumbat health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=18+(i*3),bw=190+(i*25),tc=11+(i*1),tgc=7+(i*1),ni=(i%10)+1,ay=(i%5)+1;numbat_health(lc,bl,bw,tc,tgc,ni,ay);}
ps("\nNumbat market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=26+(i*1),bw=230+(i*8),tc=15+(i*1),tgc=10+(i*1),ni=(i%4)+1,ay=(i%4)+1;numbat_market(lc,bl,bw,tc,tgc,ni,ay);}
ps("\n");numbat_report();numbat_state();ps("\n=== Demo Complete ===\n");return 0;}
