/* weasel_admin: Weasel (Mustela nivalis) small predator (v1.0)
 * Weasel meadow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,run_sp,fr_idx,age_yr,active;} wsel_t;
typedef struct{int n_meadow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_run,t_fr;} wsel_state_t;
static wsel_t wpl[N],wfd[N-2],wbr[N-4],whl[N-6],wmk[N-6]; static wsel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wsel_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int fi,int ay){if(*cnt>=mx)return -1;wsel_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->run_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WSEL] Weasel ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int wsel_init(void){if(init)return -1;st.n_meadow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_run=0;st.t_fr=0;for(int i=0;i<N;i++)wpl[i].active=0;for(int i=0;i<N-2;i++)wfd[i].active=0;for(int i=0;i<N-4;i++)wbr[i].active=0;for(int i=0;i<N-6;i++)whl[i].active=0;for(int i=0;i<N-6;i++)wmk[i].active=0;init=1;ps("[WSEL] Weasel initialized\n");return 0;}
int wsel_meadow(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(wpl,&st.n_meadow,&st.t_ln,N,lc,bl,bw,tc,rs,fi,ay);}
int wsel_feeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(wfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,fi,ay);}
int wsel_breeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(wbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,fi,ay);}
int wsel_health(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(whl,&st.n_health,&st.t_run,N-6,lc,bl,bw,tc,rs,fi,ay);}
int wsel_market(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(wmk,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,rs,fi,ay);}
void wsel_report(void){ps("[WSEL] Meadow: ");pi(st.n_meadow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void wsel_state(void){ps("[WSEL] Meadow=");pi(st.n_meadow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Weasel Admin Demo ===\n\n");wsel_init();
ps("Weasel meadow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*3),bw=50+(i*15),tc=8+(i*2),rs=15+(i*3),fi=(i%6)+1,ay=(i%3)+1;wsel_meadow(lc,bl,bw,tc,rs,fi,ay);}
ps("\nWeasel feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=22+(i*2),bw=60+(i*12),tc=9+(i%2),rs=17+(i*2),fi=(i%5)+1,ay=(i%3)+1;wsel_feeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nWeasel breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=18+(i*4),bw=40+(i*18),tc=7+(i%3),rs=13+(i*4),fi=(i%4)+1,ay=(i%2)+1;wsel_breeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nWeasel health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=25+(i*2),bw=70+(i*10),tc=10+(i%2),rs=19+(i),fi=(i%7)+1,ay=(i%4)+2;wsel_health(lc,bl,bw,tc,rs,fi,ay);}
ps("\nWeasel market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=28+(i),bw=80+(i*8),tc=11+(i%2),rs=10+(i*2),fi=(i%3)+3,ay=(i%2)+2;wsel_market(lc,bl,bw,tc,rs,fi,ay);}
ps("\n");wsel_report();wsel_state();ps("\n=== Demo Complete ===\n");return 0;}
