/* giraffe_admin: Giraffe (Giraffa camelopardalis) tallest mammal (v1.0)
 * Giraffe savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, neck_cm, walk_speed, spot_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,nck_cm,wk_sp,sp_idx,age_yr,active;} grf_t;
typedef struct{int n_savan,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_nck,t_wk,t_sp;} grf_state_t;
static grf_t svn[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static grf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grf_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nc,int ws,int si,int ay){if(*cnt>=mx)return -1;grf_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->nck_cm=nc;x->wk_sp=ws;x->sp_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GRF] Giraffe ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nc=");pi(nc);ps(" ws=");pi(ws);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int giraffe_init(void){if(init)return -1;st.n_savan=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_nck=0;st.t_wk=0;st.t_sp=0;for(int i=0;i<N;i++)svn[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[GRF] Giraffe initialized\n");return 0;}
int giraffe_savanna(int lc,int bl,int bw,int nc,int ws,int si,int ay){return add(svn,&st.n_savan,&st.t_ln,N,lc,bl,bw,nc,ws,si,ay);}
int giraffe_feeding(int lc,int bl,int bw,int nc,int ws,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nc,ws,si,ay);}
int giraffe_breeding(int lc,int bl,int bw,int nc,int ws,int si,int ay){return add(svb,&st.n_breed,&st.t_nck,N-4,lc,bl,bw,nc,ws,si,ay);}
int giraffe_health(int lc,int bl,int bw,int nc,int ws,int si,int ay){return add(svh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,nc,ws,si,ay);}
int giraffe_market(int lc,int bl,int bw,int nc,int ws,int si,int ay){return add(svm,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,nc,ws,si,ay);}
void giraffe_report(void){ps("[GRF] Savan: ");pi(st.n_savan);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Nck=");pi(st.t_nck);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void giraffe_state(void){ps("[GRF] Savan=");pi(st.n_savan);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Giraffe Admin Demo ===\n\n");giraffe_init();
ps("Giraffe savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=450+(i*15),bw=1000+(i*80),nc=200+(i*12),ws=4+(i*1),si=(i%10)+1,ay=(i%25)+1;giraffe_savanna(lc,bl,bw,nc,ws,si,ay);}
ps("\nGiraffe feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=460+(i*12),bw=1050+(i*60),nc=205+(i*10),ws=5+(i*1),si=(i%9)+1,ay=(i%22)+1;giraffe_feeding(lc,bl,bw,nc,ws,si,ay);}
ps("\nGiraffe breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=470+(i*10),bw=1100+(i*50),nc=210+(i*8),ws=3+(i*2),si=(i%8)+1,ay=(i%18)+1;giraffe_breeding(lc,bl,bw,nc,ws,si,ay);}
ps("\nGiraffe health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=445+(i*18),bw=980+(i*100),nc=198+(i*14),ws=3+(i*3),si=(i%11)+1,ay=(i%15)+1;giraffe_health(lc,bl,bw,nc,ws,si,ay);}
ps("\nGiraffe market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=480+(i*8),bw=1150+(i*40),nc=215+(i*6),ws=5+(i*1),si=(i%6)+1,ay=(i%12)+1;giraffe_market(lc,bl,bw,nc,ws,si,ay);}
ps("\n");giraffe_report();giraffe_state();ps("\n=== Demo Complete ===\n");return 0;}
