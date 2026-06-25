/* ferret_admin: Ferret (Mustela furo) domesticated small predator (v1.0)
 * Ferret burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,rn_sp,fr_idx,age_yr,active;} frt_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_rn,t_fr;} frt_state_t;
static frt_t brw[N],brf[N-2],brb[N-4],brh[N-6],brm[N-6]; static frt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(frt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int fi,int ay){if(*cnt>=mx)return -1;frt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->rn_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FRT] Ferret ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ferret_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_rn=0;st.t_fr=0;for(int i=0;i<N;i++)brw[i].active=0;for(int i=0;i<N-2;i++)brf[i].active=0;for(int i=0;i<N-4;i++)brb[i].active=0;for(int i=0;i<N-6;i++)brh[i].active=0;for(int i=0;i<N-6;i++)brm[i].active=0;init=1;ps("[FRT] Ferret initialized\n");return 0;}
int ferret_burrow(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(brw,&st.n_burrow,&st.t_ln,N,lc,bl,bw,tc,rs,fi,ay);}
int ferret_feeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(brf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,fi,ay);}
int ferret_breeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(brb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,fi,ay);}
int ferret_health(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(brh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,tc,rs,fi,ay);}
int ferret_market(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(brm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,rs,fi,ay);}
void ferret_report(void){ps("[FRT] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void ferret_state(void){ps("[FRT] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ferret Admin Demo ===\n\n");ferret_init();
ps("Ferret burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*3),bw=600+(i*50),tc=12+(i*2),rs=8+(i*2),fi=(i%8)+1,ay=(i%10)+1;ferret_burrow(lc,bl,bw,tc,rs,fi,ay);}
ps("\nFerret feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*2),bw=620+(i*40),tc=13+(i*2),rs=9+(i*2),fi=(i%7)+1,ay=(i%9)+1;ferret_feeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nFerret breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=39+(i*2),bw=640+(i*30),tc=14+(i*1),rs=7+(i*3),fi=(i%6)+1,ay=(i%8)+1;ferret_breeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nFerret health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=34+(i*4),bw=580+(i*60),tc=11+(i*3),rs=6+(i*4),fi=(i%9)+1,ay=(i%7)+1;ferret_health(lc,bl,bw,tc,rs,fi,ay);}
ps("\nFerret market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=41+(i*2),bw=660+(i*25),tc=15+(i*1),rs=10+(i*2),fi=(i%5)+1,ay=(i%6)+1;ferret_market(lc,bl,bw,tc,rs,fi,ay);}
ps("\n");ferret_report();ferret_state();ps("\n=== Demo Complete ===\n");return 0;}
