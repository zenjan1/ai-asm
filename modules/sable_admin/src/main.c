/* sable_admin: Sable (Hippotragus niger) majestic horned antelope (v1.0)
 * Sable savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, sprint_speed, sa_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,sp_sp,sa_idx,age_yr,active;} sabl_t;
typedef struct{int n_savan,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_sp,t_sa;} sabl_state_t;
static sabl_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static sabl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sabl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ss,int si,int ay){if(*cnt>=mx)return -1;sabl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->sp_sp=ss;x->sa_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SABL] Sable ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int sable_init(void){if(init)return -1;st.n_savan=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_sp=0;st.t_sa=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[SABL] Sable initialized\n");return 0;}
int sable_savanna(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svl,&st.n_savan,&st.t_ln,N,lc,bl,bw,hc,ss,si,ay);}
int sable_feeding(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ss,si,ay);}
int sable_breeding(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,ss,si,ay);}
int sable_health(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svh,&st.n_health,&st.t_sp,N-6,lc,bl,bw,hc,ss,si,ay);}
int sable_market(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svm,&st.n_mkt,&st.t_sa,N-6,lc,bl,bw,hc,ss,si,ay);}
void sable_report(void){ps("[SABL] Savan: ");pi(st.n_savan);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Sp=");pi(st.t_sp);ps("\nMkt: ");pi(st.n_mkt);ps(" Sa=");pi(st.t_sa);ps("\n");}
void sable_state(void){ps("[SABL] Savan=");pi(st.n_savan);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Sable Admin Demo ===\n\n");sable_init();
ps("Sable savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=200+(i*18),hc=100+(i*6),ss=25+(i*3),si=(i%8)+1,ay=(i%15)+1;sable_savanna(lc,bl,bw,hc,ss,si,ay);}
ps("\nSable feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=210+(i*15),hc=105+(i*5),ss=27+(i*2),si=(i%6)+1,ay=(i%12)+1;sable_feeding(lc,bl,bw,hc,ss,si,ay);}
ps("\nSable breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=220+(i*12),hc=110+(i*4),ss=24+(i*3),si=(i%5)+1,ay=(i%10)+1;sable_breeding(lc,bl,bw,hc,ss,si,ay);}
ps("\nSable health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=195+(i*22),hc=98+(i*7),ss=22+(i*4),si=(i%10)+1,ay=(i%8)+1;sable_health(lc,bl,bw,hc,ss,si,ay);}
ps("\nSable market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*5),bw=230+(i*10),hc=115+(i*3),ss=29+(i*2),si=(i%4)+1,ay=(i%7)+1;sable_market(lc,bl,bw,hc,ss,si,ay);}
ps("\n");sable_report();sable_state();ps("\n=== Demo Complete ===\n");return 0;}
