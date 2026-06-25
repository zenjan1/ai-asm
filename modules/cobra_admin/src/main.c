/* cobra_admin: Cobra (Naja naja) Indian venomous snake (v1.0)
 * Cobra jungle, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, hood_cm, strike_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hood_cm,stk_sp,sc_idx,age_yr,active;} cbra_t;
typedef struct{int n_jungl,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hood,t_stk,t_sc;} cbra_state_t;
static cbra_t jgl[N],jgf[N-2],jgb[N-4],jgh[N-6],jgm[N-6]; static cbra_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cbra_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ss,int si,int ay){if(*cnt>=mx)return -1;cbra_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hood_cm=hc;x->stk_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CBRA] Cobra ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int cobra_init(void){if(init)return -1;st.n_jungl=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hood=0;st.t_stk=0;st.t_sc=0;for(int i=0;i<N;i++)jgl[i].active=0;for(int i=0;i<N-2;i++)jgf[i].active=0;for(int i=0;i<N-4;i++)jgb[i].active=0;for(int i=0;i<N-6;i++)jgh[i].active=0;for(int i=0;i<N-6;i++)jgm[i].active=0;init=1;ps("[CBRA] Cobra initialized\n");return 0;}
int cobra_jungle(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(jgl,&st.n_jungl,&st.t_ln,N,lc,bl,bw,hc,ss,si,ay);}
int cobra_feeding(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(jgf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ss,si,ay);}
int cobra_breeding(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(jgb,&st.n_breed,&st.t_hood,N-4,lc,bl,bw,hc,ss,si,ay);}
int cobra_health(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(jgh,&st.n_health,&st.t_stk,N-6,lc,bl,bw,hc,ss,si,ay);}
int cobra_market(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(jgm,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,hc,ss,si,ay);}
void cobra_report(void){ps("[CBRA] Jungl: ");pi(st.n_jungl);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hood=");pi(st.t_hood);ps("\nHealth: ");pi(st.n_health);ps(" Stk=");pi(st.t_stk);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void cobra_state(void){ps("[CBRA] Jungl=");pi(st.n_jungl);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cobra Admin Demo ===\n\n");cobra_init();
ps("Cobra jungle...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*10),bw=200+(i*30),hc=15+(i*3),ss=30+(i*5),si=(i%8)+1,ay=(i%15)+1;cobra_jungle(lc,bl,bw,hc,ss,si,ay);}
ps("\nCobra feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=125+(i*8),bw=220+(i*25),hc=16+(i*2),ss=35+(i*4),si=(i%6)+1,ay=(i%12)+1;cobra_feeding(lc,bl,bw,hc,ss,si,ay);}
ps("\nCobra breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=130+(i*6),bw=240+(i*20),hc=18+(i*2),ss=25+(i*6),si=(i%5)+1,ay=(i%10)+1;cobra_breeding(lc,bl,bw,hc,ss,si,ay);}
ps("\nCobra health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=118+(i*12),bw=190+(i*40),hc=14+(i*4),ss=20+(i*7),si=(i%10)+1,ay=(i%8)+1;cobra_health(lc,bl,bw,hc,ss,si,ay);}
ps("\nCobra market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=135+(i*5),bw=260+(i*15),hc=19+(i*2),ss=40+(i*4),si=(i%4)+1,ay=(i%7)+1;cobra_market(lc,bl,bw,hc,ss,si,ay);}
ps("\n");cobra_report();cobra_state();ps("\n=== Demo Complete ===\n");return 0;}
