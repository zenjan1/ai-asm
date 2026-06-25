/* wombat_admin: Wombat (Vombatus ursinus) burrowing marsupial (v1.0)
 * Wombat burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_cm, dig_speed, pouch_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,dg_sp,po_idx,age_yr,active;} wmbt_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_dg,t_po;} wmbt_state_t;
static wmbt_t wpl[N],wfd[N-2],wbr[N-4],whl[N-6],wmk[N-6]; static wmbt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wmbt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int ds,int pi2,int ay){if(*cnt>=mx)return -1;wmbt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->dg_sp=ds;x->po_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WMBT] Wombat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" ds=");pi(ds);ps(" po=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int wmbt_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_dg=0;st.t_po=0;for(int i=0;i<N;i++)wpl[i].active=0;for(int i=0;i<N-2;i++)wfd[i].active=0;for(int i=0;i<N-4;i++)wbr[i].active=0;for(int i=0;i<N-6;i++)whl[i].active=0;for(int i=0;i<N-6;i++)wmk[i].active=0;init=1;ps("[WMBT] Wombat initialized\n");return 0;}
int wmbt_burrow(int lc,int bl,int bw,int cc,int ds,int pi2,int ay){return add(wpl,&st.n_burrow,&st.t_ln,N,lc,bl,bw,cc,ds,pi2,ay);}
int wmbt_feeding(int lc,int bl,int bw,int cc,int ds,int pi2,int ay){return add(wfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,ds,pi2,ay);}
int wmbt_breeding(int lc,int bl,int bw,int cc,int ds,int pi2,int ay){return add(wbr,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,ds,pi2,ay);}
int wmbt_health(int lc,int bl,int bw,int cc,int ds,int pi2,int ay){return add(whl,&st.n_health,&st.t_dg,N-6,lc,bl,bw,cc,ds,pi2,ay);}
int wmbt_market(int lc,int bl,int bw,int cc,int ds,int pi2,int ay){return add(wmk,&st.n_mkt,&st.t_po,N-6,lc,bl,bw,cc,ds,pi2,ay);}
void wmbt_report(void){ps("[WMBT] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Dg=");pi(st.t_dg);ps("\nMkt: ");pi(st.n_mkt);ps(" Po=");pi(st.t_po);ps("\n");}
void wmbt_state(void){ps("[WMBT] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Wombat Admin Demo ===\n\n");wmbt_init();
ps("Wombat burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*5),bw=25+(i*5),cc=5+(i%3),ds=3+(i*2),po=(i%6)+1,ay=(i%4)+1;wmbt_burrow(lc,bl,bw,cc,ds,po,ay);}
ps("\nWombat feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=85+(i*4),bw=28+(i*4),cc=6+(i%2),ds=4+(i*2),po=(i%5)+1,ay=(i%3)+1;wmbt_feeding(lc,bl,bw,cc,ds,po,ay);}
ps("\nWombat breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=75+(i*6),bw=22+(i*6),cc=4+(i%4),ds=2+(i*3),po=(i%4)+1,ay=(i%3)+1;wmbt_breeding(lc,bl,bw,cc,ds,po,ay);}
ps("\nWombat health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=90+(i*3),bw=30+(i*3),cc=7+(i%2),ds=5+(i),po=(i%7)+1,ay=(i%5)+2;wmbt_health(lc,bl,bw,cc,ds,po,ay);}
ps("\nWombat market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=95+(i*2),bw=35+(i*2),cc=8+(i%2),ds=1+(i*2),po=(i%3)+3,ay=(i%4)+3;wmbt_market(lc,bl,bw,cc,ds,po,ay);}
ps("\n");wmbt_report();wmbt_state();ps("\n=== Demo Complete ===\n");return 0;}
