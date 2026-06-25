/* ibis_admin: Ibis (Eudocimus albus) white ibis wading bird (v1.0)
 * Ibis wetland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, walk_speed, pl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,wk_sp,pl_idx,age_yr,active;} ibis_t;
typedef struct{int n_wetland,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_wk,t_pl;} ibis_state_t;
static ibis_t arr_w[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static ibis_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ibis_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int ws,int pli,int ay){if(*cnt>=mx)return -1;ibis_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->wk_sp=ws;x->pl_idx=pli;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[IBIS] Ibis ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" ws=");pi(ws);ps(" pli=");pi(pli);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ibis_init(void){if(init)return -1;st.n_wetland=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_wk=0;st.t_pl=0;for(int i=0;i<N;i++)arr_w[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[IBIS] Ibis initialized\n");return 0;}
int ibis_wetland(int lc,int bl,int bw,int bc,int ws,int pli,int ay){return add(arr_w,&st.n_wetland,&st.t_ln,N,lc,bl,bw,bc,ws,pli,ay);}
int ibis_feeding(int lc,int bl,int bw,int bc,int ws,int pli,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,ws,pli,ay);}
int ibis_breeding(int lc,int bl,int bw,int bc,int ws,int pli,int ay){return add(arr_b,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,ws,pli,ay);}
int ibis_health(int lc,int bl,int bw,int bc,int ws,int pli,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,bc,ws,pli,ay);}
int ibis_market(int lc,int bl,int bw,int bc,int ws,int pli,int ay){return add(arr_m,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,bc,ws,pli,ay);}
void ibis_report(void){ps("[IBIS] Wetland: ");pi(st.n_wetland);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void ibis_state(void){ps("[IBIS] Wetland=");pi(st.n_wetland);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ibis Admin Demo ===\n\n");ibis_init();
ps("Ibis wetland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=55+(i*3),bw=700+(i*40),bc=12+(i*1),ws=3+(i*1),pli=(i%6)+1,ay=(i%8)+1;ibis_wetland(lc,bl,bw,bc,ws,pli,ay);}
ps("\nIbis feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=57+(i*2),bw=720+(i*30),bc=13+(i*1),ws=4+(i*1),pli=(i%5)+1,ay=(i%7)+1;ibis_feeding(lc,bl,bw,bc,ws,pli,ay);}
ps("\nIbis breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=60+(i*2),bw=740+(i*25),bc=14+(i*1),ws=2+(i*2),pli=(i%4)+1,ay=(i%6)+1;ibis_breeding(lc,bl,bw,bc,ws,pli,ay);}
ps("\nIbis health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=52+(i*4),bw=680+(i*50),bc=11+(i*1),ws=2+(i*3),pli=(i%7)+1,ay=(i%5)+1;ibis_health(lc,bl,bw,bc,ws,pli,ay);}
ps("\nIbis market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=63+(i*2),bw=760+(i*20),bc=15+(i*1),ws=5+(i*1),pli=(i%3)+1,ay=(i%4)+1;ibis_market(lc,bl,bw,bc,ws,pli,ay);}
ps("\n");ibis_report();ibis_state();ps("\n=== Demo Complete ===\n");return 0;}
