/* topi_admin: Topi (Damaliscus lunatus) fast-running savanna antelope (v1.0)
 * Topi grassland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, sprint_speed, tp_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,sp_sp,tp_idx,age_yr,active;} topi_t;
typedef struct{int n_grass,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_sp,t_tp;} topi_state_t;
static topi_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static topi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(topi_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ss,int ti,int ay){if(*cnt>=mx)return -1;topi_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->sp_sp=ss;x->tp_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TOPI] Topi ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ss=");pi(ss);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int topi_init(void){if(init)return -1;st.n_grass=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_sp=0;st.t_tp=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[TOPI] Topi initialized\n");return 0;}
int topi_grassland(int lc,int bl,int bw,int hc,int ss,int ti,int ay){return add(svl,&st.n_grass,&st.t_ln,N,lc,bl,bw,hc,ss,ti,ay);}
int topi_feeding(int lc,int bl,int bw,int hc,int ss,int ti,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ss,ti,ay);}
int topi_breeding(int lc,int bl,int bw,int hc,int ss,int ti,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,ss,ti,ay);}
int topi_health(int lc,int bl,int bw,int hc,int ss,int ti,int ay){return add(svh,&st.n_health,&st.t_sp,N-6,lc,bl,bw,hc,ss,ti,ay);}
int topi_market(int lc,int bl,int bw,int hc,int ss,int ti,int ay){return add(svm,&st.n_mkt,&st.t_tp,N-6,lc,bl,bw,hc,ss,ti,ay);}
void topi_report(void){ps("[TOPI] Grass: ");pi(st.n_grass);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Sp=");pi(st.t_sp);ps("\nMkt: ");pi(st.n_mkt);ps(" Tp=");pi(st.t_tp);ps("\n");}
void topi_state(void){ps("[TOPI] Grass=");pi(st.n_grass);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Topi Admin Demo ===\n\n");topi_init();
ps("Topi grassland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*7),bw=100+(i*12),hc=55+(i*4),ss=32+(i*3),ti=(i%8)+1,ay=(i%12)+1;topi_grassland(lc,bl,bw,hc,ss,ti,ay);}
ps("\nTopi feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=155+(i*5),bw=105+(i*10),hc=57+(i*3),ss=34+(i*2),ti=(i%6)+1,ay=(i%10)+1;topi_feeding(lc,bl,bw,hc,ss,ti,ay);}
ps("\nTopi breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=160+(i*4),bw=110+(i*8),hc=60+(i*3),ss=31+(i*3),ti=(i%5)+1,ay=(i%8)+1;topi_breeding(lc,bl,bw,hc,ss,ti,ay);}
ps("\nTopi health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=148+(i*9),bw=95+(i*15),hc=53+(i*5),ss=29+(i*4),ti=(i%10)+1,ay=(i%7)+1;topi_health(lc,bl,bw,hc,ss,ti,ay);}
ps("\nTopi market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=165+(i*3),bw=115+(i*6),hc=62+(i*2),ss=36+(i*2),ti=(i%4)+1,ay=(i%6)+1;topi_market(lc,bl,bw,hc,ss,ti,ay);}
ps("\n");topi_report();topi_state();ps("\n=== Demo Complete ===\n");return 0;}
