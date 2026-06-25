/* echidna_admin: Echidna (Tachyglossus aculeatus) short-beaked spiny mammal (v1.0)
 * Echidna bush, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, spine_cm, dig_speed, quill_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,spine_cm,dg_sp,ql_idx,age_yr,active;} echd_t;
typedef struct{int n_bush,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_spine,t_dg,t_ql;} echd_state_t;
static echd_t bsl[N],bsf[N-2],bsb[N-4],bsh[N-6],bsm[N-6]; static echd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(echd_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int ds,int qi,int ay){if(*cnt>=mx)return -1;echd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->spine_cm=sc;x->dg_sp=ds;x->ql_idx=qi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ECHD] Echidna ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" ds=");pi(ds);ps(" qi=");pi(qi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int echidna_init(void){if(init)return -1;st.n_bush=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_spine=0;st.t_dg=0;st.t_ql=0;for(int i=0;i<N;i++)bsl[i].active=0;for(int i=0;i<N-2;i++)bsf[i].active=0;for(int i=0;i<N-4;i++)bsb[i].active=0;for(int i=0;i<N-6;i++)bsh[i].active=0;for(int i=0;i<N-6;i++)bsm[i].active=0;init=1;ps("[ECHD] Echidna initialized\n");return 0;}
int echidna_bush(int lc,int bl,int bw,int sc,int ds,int qi,int ay){return add(bsl,&st.n_bush,&st.t_ln,N,lc,bl,bw,sc,ds,qi,ay);}
int echidna_feeding(int lc,int bl,int bw,int sc,int ds,int qi,int ay){return add(bsf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,ds,qi,ay);}
int echidna_breeding(int lc,int bl,int bw,int sc,int ds,int qi,int ay){return add(bsb,&st.n_breed,&st.t_spine,N-4,lc,bl,bw,sc,ds,qi,ay);}
int echidna_health(int lc,int bl,int bw,int sc,int ds,int qi,int ay){return add(bsh,&st.n_health,&st.t_dg,N-6,lc,bl,bw,sc,ds,qi,ay);}
int echidna_market(int lc,int bl,int bw,int sc,int ds,int qi,int ay){return add(bsm,&st.n_mkt,&st.t_ql,N-6,lc,bl,bw,sc,ds,qi,ay);}
void echidna_report(void){ps("[ECHD] Bush: ");pi(st.n_bush);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Spine=");pi(st.t_spine);ps("\nHealth: ");pi(st.n_health);ps(" Dg=");pi(st.t_dg);ps("\nMkt: ");pi(st.n_mkt);ps(" Ql=");pi(st.t_ql);ps("\n");}
void echidna_state(void){ps("[ECHD] Bush=");pi(st.n_bush);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Echidna Admin Demo ===\n\n");echidna_init();
ps("Echidna bush...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*3),bw=3+(i*1),sc=2+(i%3),ds=2+(i*2),qi=(i%8)+1,ay=(i%15)+1;echidna_bush(lc,bl,bw,sc,ds,qi,ay);}
ps("\nEchidna feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*2),bw=4+(i*1),sc=2+(i%2),ds=3+(i*1),qi=(i%6)+1,ay=(i%12)+1;echidna_feeding(lc,bl,bw,sc,ds,qi,ay);}
ps("\nEchidna breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*2),bw=4+(i*1),sc=3+(i%2),ds=2+(i*3),qi=(i%5)+1,ay=(i%10)+1;echidna_breeding(lc,bl,bw,sc,ds,qi,ay);}
ps("\nEchidna health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*4),bw=3+(i*2),sc=2+(i%4),ds=1+(i*4),qi=(i%10)+1,ay=(i%8)+1;echidna_health(lc,bl,bw,sc,ds,qi,ay);}
ps("\nEchidna market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*2),bw=5+(i*1),sc=3+(i%2),ds=4+(i*2),qi=(i%4)+1,ay=(i%7)+1;echidna_market(lc,bl,bw,sc,ds,qi,ay);}
ps("\n");echidna_report();echidna_state();ps("\n=== Demo Complete ===\n");return 0;}
