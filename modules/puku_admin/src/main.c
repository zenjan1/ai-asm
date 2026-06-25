/* puku_admin: Puku (Kobus vardonii) floodplain antelope (v1.0)
 * Puku floodplain, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, pk_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,rn_sp,pk_idx,age_yr,active;} puku_t;
typedef struct{int n_flood,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_rn,t_pk;} puku_state_t;
static puku_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static puku_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(puku_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int pi_idx,int ay){if(*cnt>=mx)return -1;puku_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->rn_sp=rs;x->pk_idx=pi_idx;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PUKU] Puku ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" pi=");pi(pi_idx);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int puku_init(void){if(init)return -1;st.n_flood=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_rn=0;st.t_pk=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PUKU] Puku initialized\n");return 0;}
int puku_floodplain(int lc,int bl,int bw,int hc,int rs,int pi_idx,int ay){return add(svl,&st.n_flood,&st.t_ln,N,lc,bl,bw,hc,rs,pi_idx,ay);}
int puku_feeding(int lc,int bl,int bw,int hc,int rs,int pi_idx,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,pi_idx,ay);}
int puku_breeding(int lc,int bl,int bw,int hc,int rs,int pi_idx,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,rs,pi_idx,ay);}
int puku_health(int lc,int bl,int bw,int hc,int rs,int pi_idx,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,pi_idx,ay);}
int puku_market(int lc,int bl,int bw,int hc,int rs,int pi_idx,int ay){return add(svm,&st.n_mkt,&st.t_pk,N-6,lc,bl,bw,hc,rs,pi_idx,ay);}
void puku_report(void){ps("[PUKU] Flood: ");pi(st.n_flood);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Pk=");pi(st.t_pk);ps("\n");}
void puku_state(void){ps("[PUKU] Flood=");pi(st.n_flood);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Puku Admin Demo ===\n\n");puku_init();
ps("Puku floodplain...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=140+(i*7),bw=70+(i*8),hc=50+(i*3),rs=20+(i*3),pi_idx=(i%8)+1,ay=(i%12)+1;puku_floodplain(lc,bl,bw,hc,rs,pi_idx,ay);}
ps("\nPuku feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=145+(i*5),bw=73+(i*6),hc=52+(i*2),rs=22+(i*2),pi_idx=(i%6)+1,ay=(i%10)+1;puku_feeding(lc,bl,bw,hc,rs,pi_idx,ay);}
ps("\nPuku breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=150+(i*4),bw=76+(i*5),hc=54+(i*3),rs=19+(i*3),pi_idx=(i%5)+1,ay=(i%8)+1;puku_breeding(lc,bl,bw,hc,rs,pi_idx,ay);}
ps("\nPuku health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=138+(i*9),bw=68+(i*10),hc=48+(i*4),rs=17+(i*4),pi_idx=(i%10)+1,ay=(i%7)+1;puku_health(lc,bl,bw,hc,rs,pi_idx,ay);}
ps("\nPuku market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=155+(i*3),bw=78+(i*4),hc=56+(i*2),rs=24+(i*2),pi_idx=(i%4)+1,ay=(i%6)+1;puku_market(lc,bl,bw,hc,rs,pi_idx,ay);}
ps("\n");puku_report();puku_state();ps("\n=== Demo Complete ===\n");return 0;}
