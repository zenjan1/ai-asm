/* crab_admin: Crab (Cancer pagurus) edible decapod crustacean (v1.0)
 * Crab shore, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, claw_cm, walk_speed, shell_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,wk_sp,sh_idx,age_yr,active;} crab_t;
typedef struct{int n_shore,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_wk,t_sh;} crab_state_t;
static crab_t shrl[N],shrf[N-2],shrb[N-4],shrh[N-6],shrm[N-6]; static crab_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crab_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int ws,int si,int ay){if(*cnt>=mx)return -1;crab_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->wk_sp=ws;x->sh_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CRAB] Crab ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" ws=");pi(ws);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int crab_init(void){if(init)return -1;st.n_shore=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_wk=0;st.t_sh=0;for(int i=0;i<N;i++)shrl[i].active=0;for(int i=0;i<N-2;i++)shrf[i].active=0;for(int i=0;i<N-4;i++)shrb[i].active=0;for(int i=0;i<N-6;i++)shrh[i].active=0;for(int i=0;i<N-6;i++)shrm[i].active=0;init=1;ps("[CRAB] Crab initialized\n");return 0;}
int crab_shore(int lc,int bl,int bw,int cc,int ws,int si,int ay){return add(shrl,&st.n_shore,&st.t_ln,N,lc,bl,bw,cc,ws,si,ay);}
int crab_feeding(int lc,int bl,int bw,int cc,int ws,int si,int ay){return add(shrf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,ws,si,ay);}
int crab_breeding(int lc,int bl,int bw,int cc,int ws,int si,int ay){return add(shrb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,ws,si,ay);}
int crab_health(int lc,int bl,int bw,int cc,int ws,int si,int ay){return add(shrh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,cc,ws,si,ay);}
int crab_market(int lc,int bl,int bw,int cc,int ws,int si,int ay){return add(shrm,&st.n_mkt,&st.t_sh,N-6,lc,bl,bw,cc,ws,si,ay);}
void crab_report(void){ps("[CRAB] Shore: ");pi(st.n_shore);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Sh=");pi(st.t_sh);ps("\n");}
void crab_state(void){ps("[CRAB] Shore=");pi(st.n_shore);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Crab Admin Demo ===\n\n");crab_init();
ps("Crab shore...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*2),bw=100+(i*30),cc=5+(i*2),ws=1+(i%3),si=(i%8)+1,ay=(i%8)+1;crab_shore(lc,bl,bw,cc,ws,si,ay);}
ps("\nCrab feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=12+(i*1),bw=120+(i*25),cc=6+(i*1),ws=1+(i%2),si=(i%6)+1,ay=(i%7)+1;crab_feeding(lc,bl,bw,cc,ws,si,ay);}
ps("\nCrab breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=14+(i*1),bw=140+(i*20),cc=7+(i%2),ws=1+(i%4),si=(i%5)+1,ay=(i%6)+1;crab_breeding(lc,bl,bw,cc,ws,si,ay);}
ps("\nCrab health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*3),bw=90+(i*40),cc=4+(i*3),ws=1+(i%5),si=(i%10)+1,ay=(i%5)+1;crab_health(lc,bl,bw,cc,ws,si,ay);}
ps("\nCrab market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=15+(i*1),bw=160+(i*15),cc=8+(i%2),ws=2+(i%2),si=(i%4)+1,ay=(i%4)+1;crab_market(lc,bl,bw,cc,ws,si,ay);}
ps("\n");crab_report();crab_state();ps("\n=== Demo Complete ===\n");return 0;}
