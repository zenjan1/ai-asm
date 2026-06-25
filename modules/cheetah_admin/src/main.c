/* cheetah_admin: Cheetah (Acinonyx jubatus) fast feline predator (v1.0)
 * Cheetah savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_cm, run_speed, spot_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,run_sp,sp_idx,age_yr,active;} ctha_t;
typedef struct{int n_savan,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_run,t_sp;} ctha_state_t;
static ctha_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static ctha_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ctha_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int rs,int si,int ay){if(*cnt>=mx)return -1;ctha_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->run_sp=rs;x->sp_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CTHA] Cheetah ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" rs=");pi(rs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int cheetah_init(void){if(init)return -1;st.n_savan=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_run=0;st.t_sp=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[CTHA] Cheetah initialized\n");return 0;}
int cheetah_savanna(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(svl,&st.n_savan,&st.t_ln,N,lc,bl,bw,cc,rs,si,ay);}
int cheetah_feeding(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,rs,si,ay);}
int cheetah_breeding(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(svb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,rs,si,ay);}
int cheetah_health(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(svh,&st.n_health,&st.t_run,N-6,lc,bl,bw,cc,rs,si,ay);}
int cheetah_market(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(svm,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,cc,rs,si,ay);}
void cheetah_report(void){ps("[CTHA] Savan: ");pi(st.n_savan);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void cheetah_state(void){ps("[CTHA] Savan=");pi(st.n_savan);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cheetah Admin Demo ===\n\n");cheetah_init();
ps("Cheetah savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*8),bw=40+(i*5),cc=2+(i%3),rs=80+(i*5),si=(i%20)+1,ay=(i%12)+1;cheetah_savanna(lc,bl,bw,cc,rs,si,ay);}
ps("\nCheetah feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=125+(i*6),bw=43+(i*4),cc=2+(i%2),rs=85+(i*4),si=(i%15)+1,ay=(i%10)+1;cheetah_feeding(lc,bl,bw,cc,rs,si,ay);}
ps("\nCheetah breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=130+(i*5),bw=46+(i*3),cc=3+(i%2),rs=75+(i*6),si=(i%12)+1,ay=(i%8)+1;cheetah_breeding(lc,bl,bw,cc,rs,si,ay);}
ps("\nCheetah health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=118+(i*9),bw=38+(i*6),cc=2+(i%4),rs=70+(i*7),si=(i%25)+1,ay=(i%7)+1;cheetah_health(lc,bl,bw,cc,rs,si,ay);}
ps("\nCheetah market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=135+(i*4),bw=48+(i*3),cc=3+(i%2),rs=90+(i*3),si=(i%8)+1,ay=(i%6)+1;cheetah_market(lc,bl,bw,cc,rs,si,ay);}
ps("\n");cheetah_report();cheetah_state();ps("\n=== Demo Complete ===\n");return 0;}
