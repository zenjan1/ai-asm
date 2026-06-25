/* armadillo_admin: Armadillo (Dasypus novemcinctus) nine-banded armored mammal (v1.0)
 * Armadillo burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, shell_cm, dig_speed, plate_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,shl_cm,dg_sp,pl_idx,age_yr,active;} arm_t;
typedef struct{int n_burr,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_shl,t_dg,t_pl;} arm_state_t;
static arm_t brl[N],brf[N-2],brb[N-4],brh[N-6],brm[N-6]; static arm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(arm_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int ds,int pi2,int ay){if(*cnt>=mx)return -1;arm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->shl_cm=sc;x->dg_sp=ds;x->pl_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ARM] Armadillo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" ds=");pi(ds);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int armadillo_init(void){if(init)return -1;st.n_burr=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_shl=0;st.t_dg=0;st.t_pl=0;for(int i=0;i<N;i++)brl[i].active=0;for(int i=0;i<N-2;i++)brf[i].active=0;for(int i=0;i<N-4;i++)brb[i].active=0;for(int i=0;i<N-6;i++)brh[i].active=0;for(int i=0;i<N-6;i++)brm[i].active=0;init=1;ps("[ARM] Armadillo initialized\n");return 0;}
int armadillo_burrow(int lc,int bl,int bw,int sc,int ds,int pi2,int ay){return add(brl,&st.n_burr,&st.t_ln,N,lc,bl,bw,sc,ds,pi2,ay);}
int armadillo_feeding(int lc,int bl,int bw,int sc,int ds,int pi2,int ay){return add(brf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,ds,pi2,ay);}
int armadillo_breeding(int lc,int bl,int bw,int sc,int ds,int pi2,int ay){return add(brb,&st.n_breed,&st.t_shl,N-4,lc,bl,bw,sc,ds,pi2,ay);}
int armadillo_health(int lc,int bl,int bw,int sc,int ds,int pi2,int ay){return add(brh,&st.n_health,&st.t_dg,N-6,lc,bl,bw,sc,ds,pi2,ay);}
int armadillo_market(int lc,int bl,int bw,int sc,int ds,int pi2,int ay){return add(brm,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,sc,ds,pi2,ay);}
void armadillo_report(void){ps("[ARM] Burr: ");pi(st.n_burr);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Shl=");pi(st.t_shl);ps("\nHealth: ");pi(st.n_health);ps(" Dg=");pi(st.t_dg);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void armadillo_state(void){ps("[ARM] Burr=");pi(st.n_burr);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Armadillo Admin Demo ===\n\n");armadillo_init();
ps("Armadillo burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=3+(i*2),sc=20+(i*2),ds=5+(i*2),pi2=(i%9)+1,ay=(i%12)+1;armadillo_burrow(lc,bl,bw,sc,ds,pi2,ay);}
ps("\nArmadillo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*3),bw=4+(i*2),sc=21+(i*2),ds=6+(i*2),pi2=(i%7)+1,ay=(i%10)+1;armadillo_feeding(lc,bl,bw,sc,ds,pi2,ay);}
ps("\nArmadillo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=44+(i*2),bw=5+(i*2),sc=22+(i*2),ds=4+(i*3),pi2=(i%5)+1,ay=(i%8)+1;armadillo_breeding(lc,bl,bw,sc,ds,pi2,ay);}
ps("\nArmadillo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=3+(i*3),sc=19+(i*3),ds=3+(i*4),pi2=(i%11)+1,ay=(i%7)+1;armadillo_health(lc,bl,bw,sc,ds,pi2,ay);}
ps("\nArmadillo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=46+(i*2),bw=6+(i*2),sc=23+(i*2),ds=7+(i*2),pi2=(i%4)+1,ay=(i%6)+1;armadillo_market(lc,bl,bw,sc,ds,pi2,ay);}
ps("\n");armadillo_report();armadillo_state();ps("\n=== Demo Complete ===\n");return 0;}
