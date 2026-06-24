/* cobra_admin: Cobra (Naja naja) venomous snake husbandry (v1.0)
 * Cobra housing, feeding, shedding, milking, market
 * Features: body_len_cm, body_wt_g, venom_mg, shed_cycle_wk, feed_interval, health_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,venom_mg,shed_cyc,fed_int,hlth_idx,active;} cbr_t;
typedef struct{int n_house,n_feed,n_shed,n_milk,n_mkt,t_ln,t_wt,t_venom,t_shed,t_feed;} cbr_state_t;
static cbr_t cbrhs[N],cbrfd[N-2],cbrsd[N-4],cbrmk[N-6],cbrmt[N-6]; static cbr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cbr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int vm,int sc,int fi,int hi){if(*cnt>=mx)return -1;cbr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->venom_mg=vm;x->shed_cyc=sc;x->fed_int=fi;x->hlth_idx=hi;x->active=1;*sum+=bl;(*cnt)++;ps("[CBR] Cobra ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" vm=");pi(vm);ps(" sc=");pi(sc);ps(" fi=");pi(fi);ps(" hi=");pi(hi);ps("\n");return *cnt-1;}
int cbr_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_shed=0;st.n_milk=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_venom=0;st.t_shed=0;st.t_feed=0;for(int i=0;i<N;i++)cbrhs[i].active=0;for(int i=0;i<N-2;i++)cbrfd[i].active=0;for(int i=0;i<N-4;i++)cbrsd[i].active=0;for(int i=0;i<N-6;i++)cbrmk[i].active=0;for(int i=0;i<N-6;i++)cbrmt[i].active=0;init=1;ps("[CBR] Cobra initialized\n");return 0;}
int cbr_housing(int lc,int bl,int bw,int vm,int sc,int fi,int hi){return add(cbrhs,&st.n_house,&st.t_ln,N,lc,bl,bw,vm,sc,fi,hi);}
int cbr_feeding(int lc,int bl,int bw,int vm,int sc,int fi,int hi){return add(cbrfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,vm,sc,fi,hi);}
int cbr_shedding(int lc,int bl,int bw,int vm,int sc,int fi,int hi){return add(cbrsd,&st.n_shed,&st.t_venom,N-4,lc,bl,bw,vm,sc,fi,hi);}
int cbr_milking(int lc,int bl,int bw,int vm,int sc,int fi,int hi){return add(cbrmk,&st.n_milk,&st.t_shed,N-6,lc,bl,bw,vm,sc,fi,hi);}
int cbr_market(int lc,int bl,int bw,int vm,int sc,int fi,int hi){return add(cbrmt,&st.n_mkt,&st.t_feed,N-6,lc,bl,bw,vm,sc,fi,hi);}
void cbr_report(void){ps("[CBR] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nShed: ");pi(st.n_shed);ps(" Venom=");pi(st.t_venom);ps("\nMilk: ");pi(st.n_milk);ps(" Shed=");pi(st.t_shed);ps("\nMkt: ");pi(st.n_mkt);ps(" Feed=");pi(st.t_feed);ps("\n");}
void cbr_state(void){ps("[CBR] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Shed=");pi(st.n_shed);ps(" Milk=");pi(st.n_milk);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cobra Admin Demo ===\n\n");cbr_init();
/* 1=enclosure 2=terrarium 3=lab 4=farm 5=market */
ps("Cobra housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*20),bw=500+(i*100),vm=50+(i*20),sc=8+(i%6),fi=5+(i%4),hi=7+(i%4);cbr_housing(lc,bl,bw,vm,sc,fi,hi);}
ps("\nCobra feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=130+(i*18),bw=550+(i*90),vm=55+(i*18),sc=9+(i%5),fi=6+(i%3),hi=7+(i%3);cbr_feeding(lc,bl,bw,vm,sc,fi,hi);}
ps("\nCobra shedding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=140+(i*15),bw=600+(i*80),vm=60+(i*15),sc=10+(i%4),fi=7+(i%3),hi=8+(i%3);cbr_shedding(lc,bl,bw,vm,sc,fi,hi);}
ps("\nCobra milking...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=110+(i*25),bw=450+(i*120),vm=40+(i*25),sc=7+(i%7),fi=4+(i%5),hi=6+(i%4);cbr_milking(lc,bl,bw,vm,sc,fi,hi);}
ps("\nCobra market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=150+(i*15),bw=650+(i*70),vm=65+(i*12),sc=11+(i%3),fi=8+(i%2),hi=9+(i%2);cbr_market(lc,bl,bw,vm,sc,fi,hi);}
ps("\n");cbr_report();cbr_state();ps("\n=== Demo Complete ===\n");return 0;}
