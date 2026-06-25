/* bontebok_admin: Bontebok (Damaliscus dorcas) rare Cape flat antelope (v1.0)
 * Bontebok fynbos, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, bb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,rn_sp,bb_idx,age_yr,active;} bont_t;
typedef struct{int n_fynbos,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_rn,t_bb;} bont_state_t;
static bont_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static bont_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bont_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int bi,int ay){if(*cnt>=mx)return -1;bont_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->rn_sp=rs;x->bb_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BONT] Bontebok ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bontebok_init(void){if(init)return -1;st.n_fynbos=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_rn=0;st.t_bb=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BONT] Bontebok initialized\n");return 0;}
int bontebok_fynbos(int lc,int bl,int bw,int hc,int rs,int bi,int ay){return add(svl,&st.n_fynbos,&st.t_ln,N,lc,bl,bw,hc,rs,bi,ay);}
int bontebok_feeding(int lc,int bl,int bw,int hc,int rs,int bi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,bi,ay);}
int bontebok_breeding(int lc,int bl,int bw,int hc,int rs,int bi,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,rs,bi,ay);}
int bontebok_health(int lc,int bl,int bw,int hc,int rs,int bi,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,bi,ay);}
int bontebok_market(int lc,int bl,int bw,int hc,int rs,int bi,int ay){return add(svm,&st.n_mkt,&st.t_bb,N-6,lc,bl,bw,hc,rs,bi,ay);}
void bontebok_report(void){ps("[BONT] Fynbos: ");pi(st.n_fynbos);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Bb=");pi(st.t_bb);ps("\n");}
void bontebok_state(void){ps("[BONT] Fynbos=");pi(st.n_fynbos);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bontebok Admin Demo ===\n\n");bontebok_init();
ps("Bontebok fynbos...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*6),bw=60+(i*6),hc=45+(i*3),rs=24+(i*2),bi=(i%8)+1,ay=(i%12)+1;bontebok_fynbos(lc,bl,bw,hc,rs,bi,ay);}
ps("\nBontebok feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=123+(i*5),bw=63+(i*5),hc=47+(i*2),rs=25+(i*2),bi=(i%6)+1,ay=(i%10)+1;bontebok_feeding(lc,bl,bw,hc,rs,bi,ay);}
ps("\nBontebok breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=126+(i*4),bw=66+(i*4),hc=49+(i*3),rs=23+(i*2),bi=(i%5)+1,ay=(i%8)+1;bontebok_breeding(lc,bl,bw,hc,rs,bi,ay);}
ps("\nBontebok health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=118+(i*8),bw=58+(i*8),hc=43+(i*4),rs=21+(i*3),bi=(i%10)+1,ay=(i%7)+1;bontebok_health(lc,bl,bw,hc,rs,bi,ay);}
ps("\nBontebok market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=130+(i*3),bw=68+(i*3),hc=51+(i*2),rs=27+(i*1),bi=(i%4)+1,ay=(i%6)+1;bontebok_market(lc,bl,bw,hc,rs,bi,ay);}
ps("\n");bontebok_report();bontebok_state();ps("\n=== Demo Complete ===\n");return 0;}
