/* okapi_admin: Okapi (Okapia johnstoni) okapi forest giraffid mammal (v1.0)
 * Okapi forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, leg_cm, walk_speed, tg_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_cm,wk_sp,tg_idx,age_yr,active;} okap_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_leg,t_wk,t_tg;} okap_state_t;
static okap_t arr_f[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static okap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(okap_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lg,int ws,int ti,int ay){if(*cnt>=mx)return -1;okap_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_cm=lg;x->wk_sp=ws;x->tg_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[OKAP] Okapi ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lg=");pi(lg);ps(" ws=");pi(ws);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int okapi_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_wk=0;st.t_tg=0;for(int i=0;i<N;i++)arr_f[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[OKAP] Okapi initialized\n");return 0;}
int okapi_forest(int lc,int bl,int bw,int lg,int ws,int ti,int ay){return add(arr_f,&st.n_forest,&st.t_ln,N,lc,bl,bw,lg,ws,ti,ay);}
int okapi_feeding(int lc,int bl,int bw,int lg,int ws,int ti,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lg,ws,ti,ay);}
int okapi_breeding(int lc,int bl,int bw,int lg,int ws,int ti,int ay){return add(arr_b,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,lg,ws,ti,ay);}
int okapi_health(int lc,int bl,int bw,int lg,int ws,int ti,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,lg,ws,ti,ay);}
int okapi_market(int lc,int bl,int bw,int lg,int ws,int ti,int ay){return add(arr_m,&st.n_mkt,&st.t_tg,N-6,lc,bl,bw,lg,ws,ti,ay);}
void okapi_report(void){ps("[OKAP] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Tg=");pi(st.t_tg);ps("\n");}
void okapi_state(void){ps("[OKAP] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Okapi Admin Demo ===\n\n");okapi_init();
ps("Okapi forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=250+(i*20),lg=120+(i*8),ws=3+(i*1),ti=(i%6)+1,ay=(i%25)+1;okapi_forest(lc,bl,bw,lg,ws,ti,ay);}
ps("\nOkapi feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=260+(i*18),lg=125+(i*6),ws=4+(i*1),ti=(i%5)+1,ay=(i%22)+1;okapi_feeding(lc,bl,bw,lg,ws,ti,ay);}
ps("\nOkapi breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=270+(i*15),lg=130+(i*5),ws=2+(i*2),ti=(i%4)+1,ay=(i%20)+1;okapi_breeding(lc,bl,bw,lg,ws,ti,ay);}
ps("\nOkapi health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=190+(i*12),bw=240+(i*22),lg=115+(i*9),ws=2+(i*3),ti=(i%7)+1,ay=(i%15)+1;okapi_health(lc,bl,bw,lg,ws,ti,ay);}
ps("\nOkapi market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*5),bw=280+(i*12),lg=135+(i*4),ws=5+(i*1),ti=(i%3)+1,ay=(i%12)+1;okapi_market(lc,bl,bw,lg,ws,ti,ay);}
ps("\n");okapi_report();okapi_state();ps("\n=== Demo Complete ===\n");return 0;}
