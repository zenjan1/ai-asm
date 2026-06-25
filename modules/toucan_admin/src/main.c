/* toucan_admin: Toucan (Ramphastos toco) tropical bird (v1.0)
 * Toucan canopy, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, bill_cm, fly_speed, plumage_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,bill_cm,fl_sp,pg_idx,age_yr,active;} tcn_t;
typedef struct{int n_canopy,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_bill,t_fl,t_pg;} tcn_state_t;
static tcn_t tpl[N],tfd[N-2],tbr[N-4],thl[N-6],tmk[N-6]; static tcn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tcn_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int fs,int pi2,int ay){if(*cnt>=mx)return -1;tcn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->bill_cm=bc;x->fl_sp=fs;x->pg_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TCAN] Toucan ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" fs=");pi(fs);ps(" pg=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int tcn_init(void){if(init)return -1;st.n_canopy=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_bill=0;st.t_fl=0;st.t_pg=0;for(int i=0;i<N;i++)tpl[i].active=0;for(int i=0;i<N-2;i++)tfd[i].active=0;for(int i=0;i<N-4;i++)tbr[i].active=0;for(int i=0;i<N-6;i++)thl[i].active=0;for(int i=0;i<N-6;i++)tmk[i].active=0;init=1;ps("[TCAN] Toucan initialized\n");return 0;}
int tcn_canopy(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(tpl,&st.n_canopy,&st.t_ln,N,lc,bl,bw,bc,fs,pi2,ay);}
int tcn_feeding(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(tfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,fs,pi2,ay);}
int tcn_breeding(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(tbr,&st.n_breed,&st.t_bill,N-4,lc,bl,bw,bc,fs,pi2,ay);}
int tcn_health(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(thl,&st.n_health,&st.t_fl,N-6,lc,bl,bw,bc,fs,pi2,ay);}
int tcn_market(int lc,int bl,int bw,int bc,int fs,int pi2,int ay){return add(tmk,&st.n_mkt,&st.t_pg,N-6,lc,bl,bw,bc,fs,pi2,ay);}
void tcn_report(void){ps("[TCAN] Canopy: ");pi(st.n_canopy);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Bill=");pi(st.t_bill);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pg=");pi(st.t_pg);ps("\n");}
void tcn_state(void){ps("[TCAN] Canopy=");pi(st.n_canopy);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Toucan Admin Demo ===\n\n");tcn_init();
ps("Toucan canopy...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*5),bw=300+(i*50),bc=15+(i*3),fs=20+(i*4),pg=(i%6)+1,ay=(i%4)+1;tcn_canopy(lc,bl,bw,bc,fs,pg,ay);}
ps("\nToucan feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=45+(i*4),bw=350+(i*40),bc=18+(i*2),fs=22+(i*3),pg=(i%5)+1,ay=(i%3)+1;tcn_feeding(lc,bl,bw,bc,fs,pg,ay);}
ps("\nToucan breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=35+(i*6),bw=250+(i*60),bc=12+(i*4),fs=18+(i*5),pg=(i%4)+1,ay=(i%3)+1;tcn_breeding(lc,bl,bw,bc,fs,pg,ay);}
ps("\nToucan health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=50+(i*3),bw=400+(i*30),bc=20+(i%3),fs=25+(i*2),pg=(i%7)+1,ay=(i%5)+2;tcn_health(lc,bl,bw,bc,fs,pg,ay);}
ps("\nToucan market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=55+(i*2),bw=450+(i*20),bc=22+(i%2),fs=15+(i*4),pg=(i%3)+3,ay=(i%4)+3;tcn_market(lc,bl,bw,bc,fs,pg,ay);}
ps("\n");tcn_report();tcn_state();ps("\n=== Demo Complete ===\n");return 0;}
