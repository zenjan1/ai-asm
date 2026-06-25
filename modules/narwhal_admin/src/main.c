/* narwhal_admin: Narwhal (Monodon monoceros) narwhal Arctic whale (v1.0)
 * Narwhal arctic, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tusk_cm, dive_m, tusk_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tusk_cm,dv_m,tk_idx,age_yr,active;} narw_t;
typedef struct{int n_arctic,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tusk,t_dv,t_tk;} narw_state_t;
static narw_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static narw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(narw_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int dm,int ti,int ay){if(*cnt>=mx)return -1;narw_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tusk_cm=tc;x->dv_m=dm;x->tk_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[NARW] Narwhal ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" dm=");pi(dm);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int narw_init(void){if(init)return -1;st.n_arctic=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tusk=0;st.t_dv=0;st.t_tk=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[NARW] Narwhal initialized\n");return 0;}
int narw_arctic(int lc,int bl,int bw,int tc,int dm,int ti,int ay){return add(apl,&st.n_arctic,&st.t_ln,N,lc,bl,bw,tc,dm,ti,ay);}
int narw_feeding(int lc,int bl,int bw,int tc,int dm,int ti,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,dm,ti,ay);}
int narw_breeding(int lc,int bl,int bw,int tc,int dm,int ti,int ay){return add(abd,&st.n_breed,&st.t_tusk,N-4,lc,bl,bw,tc,dm,ti,ay);}
int narw_health(int lc,int bl,int bw,int tc,int dm,int ti,int ay){return add(ahd,&st.n_health,&st.t_dv,N-6,lc,bl,bw,tc,dm,ti,ay);}
int narw_market(int lc,int bl,int bw,int tc,int dm,int ti,int ay){return add(amd,&st.n_mkt,&st.t_tk,N-6,lc,bl,bw,tc,dm,ti,ay);}
void narw_report(void){ps("[NARW] Arctic: ");pi(st.n_arctic);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tusk=");pi(st.t_tusk);ps("\nHealth: ");pi(st.n_health);ps(" Dv=");pi(st.t_dv);ps("\nMkt: ");pi(st.n_mkt);ps(" Tk=");pi(st.t_tk);ps("\n");}
void narw_state(void){ps("[NARW] Arctic=");pi(st.n_arctic);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Narwhal Admin Demo ===\n\n");narw_init();
ps("Narwhal arctic...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=300+(i*20),bw=1000+(i*100),tc=150+(i*15),dm=300+(i*30),ti=(i%7)+1,ay=3+(i%20);narw_arctic(lc,bl,bw,tc,dm,ti,ay);}
ps("\nNarwhal feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=310+(i*20),bw=1050+(i*100),tc=155+(i*15),dm=310+(i*30),ti=(i%6)+2,ay=4+(i%18);narw_feeding(lc,bl,bw,tc,dm,ti,ay);}
ps("\nNarwhal breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=320+(i*20),bw=1100+(i*100),tc=160+(i*15),dm=320+(i*30),ti=(i%5)+1,ay=5+(i%15);narw_breeding(lc,bl,bw,tc,dm,ti,ay);}
ps("\nNarwhal health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=290+(i*25),bw=950+(i*120),tc=145+(i*18),dm=280+(i*35),ti=(i%4)+3,ay=6+(i%12);narw_health(lc,bl,bw,tc,dm,ti,ay);}
ps("\nNarwhal market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=340+(i*20),bw=1200+(i*100),tc=170+(i*15),dm=340+(i*30),ti=(i%3)+4,ay=7+(i%10);narw_market(lc,bl,bw,tc,dm,ti,ay);}
ps("\n");narw_report();narw_state();ps("\n=== Demo Complete ===\n");return 0;}
