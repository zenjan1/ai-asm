/* narwhal_admin: Narwhal (Monodon monoceros) arctic tusked whale (v1.0)
 * Narwhal arctic, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tusk_cm, swim_speed, tusk_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tusk_cm,sw_sp,ts_idx,age_yr,active;} narw_t;
typedef struct{int n_arctic,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tusk,t_sw,t_ts;} narw_state_t;
static narw_t arc[N],acf[N-2],acb[N-4],ach[N-6],acm[N-6]; static narw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(narw_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int ti,int ay){if(*cnt>=mx)return -1;narw_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tusk_cm=tc;x->sw_sp=ss;x->ts_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[NARW] Narwhal ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int narwhal_init(void){if(init)return -1;st.n_arctic=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tusk=0;st.t_sw=0;st.t_ts=0;for(int i=0;i<N;i++)arc[i].active=0;for(int i=0;i<N-2;i++)acf[i].active=0;for(int i=0;i<N-4;i++)acb[i].active=0;for(int i=0;i<N-6;i++)ach[i].active=0;for(int i=0;i<N-6;i++)acm[i].active=0;init=1;ps("[NARW] Narwhal initialized\n");return 0;}
int narwhal_arctic(int lc,int bl,int bw,int tc,int ss,int ti,int ay){return add(arc,&st.n_arctic,&st.t_ln,N,lc,bl,bw,tc,ss,ti,ay);}
int narwhal_feeding(int lc,int bl,int bw,int tc,int ss,int ti,int ay){return add(acf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,ti,ay);}
int narwhal_breeding(int lc,int bl,int bw,int tc,int ss,int ti,int ay){return add(acb,&st.n_breed,&st.t_tusk,N-4,lc,bl,bw,tc,ss,ti,ay);}
int narwhal_health(int lc,int bl,int bw,int tc,int ss,int ti,int ay){return add(ach,&st.n_health,&st.t_sw,N-6,lc,bl,bw,tc,ss,ti,ay);}
int narwhal_market(int lc,int bl,int bw,int tc,int ss,int ti,int ay){return add(acm,&st.n_mkt,&st.t_ts,N-6,lc,bl,bw,tc,ss,ti,ay);}
void narwhal_report(void){ps("[NARW] Arctic: ");pi(st.n_arctic);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tusk=");pi(st.t_tusk);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Ts=");pi(st.t_ts);ps("\n");}
void narwhal_state(void){ps("[NARW] Arctic=");pi(st.n_arctic);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Narwhal Admin Demo ===\n\n");narwhal_init();
ps("Narwhal arctic...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=300+(i*12),bw=1200+(i*80),tc=180+(i*10),ss=4+(i*1),ti=(i%8)+1,ay=(i%25)+1;narwhal_arctic(lc,bl,bw,tc,ss,ti,ay);}
ps("\nNarwhal feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=305+(i*10),bw=1230+(i*60),tc=185+(i*8),ss=4+(i*1),ti=(i%7)+1,ay=(i%22)+1;narwhal_feeding(lc,bl,bw,tc,ss,ti,ay);}
ps("\nNarwhal breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=310+(i*8),bw=1260+(i*50),tc=190+(i*6),ss=3+(i*2),ti=(i%6)+1,ay=(i%18)+1;narwhal_breeding(lc,bl,bw,tc,ss,ti,ay);}
ps("\nNarwhal health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=295+(i*15),bw=1180+(i*100),tc=175+(i*12),ss=3+(i*2),ti=(i%9)+1,ay=(i%15)+1;narwhal_health(lc,bl,bw,tc,ss,ti,ay);}
ps("\nNarwhal market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=315+(i*7),bw=1280+(i*40),tc=195+(i*5),ss=5+(i*1),ti=(i%5)+1,ay=(i%12)+1;narwhal_market(lc,bl,bw,tc,ss,ti,ay);}
ps("\n");narwhal_report();narwhal_state();ps("\n=== Demo Complete ===\n");return 0;}
