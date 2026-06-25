/* rat_admin: Rat (Rattus norvegicus) brown rat small rodent (v1.0)
 * Rat urban, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, run_speed, wh_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,rn_sp,wh_idx,age_yr,active;} rat_t;
typedef struct{int n_urban,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_rn,t_wh;} rat_state_t;
static rat_t arr_u[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static rat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rat_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int whi,int ay){if(*cnt>=mx)return -1;rat_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->rn_sp=rs;x->wh_idx=whi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[RAT] Rat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" whi=");pi(whi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rat_init(void){if(init)return -1;st.n_urban=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_rn=0;st.t_wh=0;for(int i=0;i<N;i++)arr_u[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[RAT] Rat initialized\n");return 0;}
int rat_urban(int lc,int bl,int bw,int tc,int rs,int whi,int ay){return add(arr_u,&st.n_urban,&st.t_ln,N,lc,bl,bw,tc,rs,whi,ay);}
int rat_feeding(int lc,int bl,int bw,int tc,int rs,int whi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,whi,ay);}
int rat_breeding(int lc,int bl,int bw,int tc,int rs,int whi,int ay){return add(arr_b,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,whi,ay);}
int rat_health(int lc,int bl,int bw,int tc,int rs,int whi,int ay){return add(arr_h,&st.n_health,&st.t_rn,N-6,lc,bl,bw,tc,rs,whi,ay);}
int rat_market(int lc,int bl,int bw,int tc,int rs,int whi,int ay){return add(arr_m,&st.n_mkt,&st.t_wh,N-6,lc,bl,bw,tc,rs,whi,ay);}
void rat_report(void){ps("[RAT] Urban: ");pi(st.n_urban);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Wh=");pi(st.t_wh);ps("\n");}
void rat_state(void){ps("[RAT] Urban=");pi(st.n_urban);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Rat Admin Demo ===\n\n");rat_init();
ps("Rat urban...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*2),bw=200+(i*20),tc=15+(i*2),rs=6+(i*1),whi=(i%6)+1,ay=(i%3)+1;rat_urban(lc,bl,bw,tc,rs,whi,ay);}
ps("\nRat feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=22+(i*1),bw=210+(i*15),tc=16+(i*1),rs=7+(i*1),whi=(i%5)+1,ay=(i%3)+1;rat_feeding(lc,bl,bw,tc,rs,whi,ay);}
ps("\nRat breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=24+(i*1),bw=220+(i*12),tc=17+(i*1),rs=5+(i*2),whi=(i%4)+1,ay=(i%2)+1;rat_breeding(lc,bl,bw,tc,rs,whi,ay);}
ps("\nRat health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=18+(i*3),bw=190+(i*25),tc=14+(i*2),rs=4+(i*2),whi=(i%7)+1,ay=(i%2)+1;rat_health(lc,bl,bw,tc,rs,whi,ay);}
ps("\nRat market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=26+(i*1),bw=230+(i*10),tc=18+(i*1),rs=8+(i*1),whi=(i%3)+1,ay=(i%2)+1;rat_market(lc,bl,bw,tc,rs,whi,ay);}
ps("\n");rat_report();rat_state();ps("\n=== Demo Complete ===\n");return 0;}
