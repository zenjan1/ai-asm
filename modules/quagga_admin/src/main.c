/* quagga_admin: Quagga (Equus quagga quagga) extinct plains zebra subspecies (v1.0)
 * Quagga plains, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, mane_cm, run_speed, qu_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,mn_cm,rn_sp,qu_idx,age_yr,active;} quag_t;
typedef struct{int n_plains,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_mn,t_rn,t_qu;} quag_state_t;
static quag_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static quag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(quag_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int mc,int rs,int qi,int ay){if(*cnt>=mx)return -1;quag_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->mn_cm=mc;x->rn_sp=rs;x->qu_idx=qi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[QUAG] Quagga ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" mc=");pi(mc);ps(" rs=");pi(rs);ps(" qi=");pi(qi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int quagga_init(void){if(init)return -1;st.n_plains=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_mn=0;st.t_rn=0;st.t_qu=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[QUAG] Quagga initialized\n");return 0;}
int quagga_plains(int lc,int bl,int bw,int mc,int rs,int qi,int ay){return add(svl,&st.n_plains,&st.t_ln,N,lc,bl,bw,mc,rs,qi,ay);}
int quagga_feeding(int lc,int bl,int bw,int mc,int rs,int qi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,mc,rs,qi,ay);}
int quagga_breeding(int lc,int bl,int bw,int mc,int rs,int qi,int ay){return add(svb,&st.n_breed,&st.t_mn,N-4,lc,bl,bw,mc,rs,qi,ay);}
int quagga_health(int lc,int bl,int bw,int mc,int rs,int qi,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,mc,rs,qi,ay);}
int quagga_market(int lc,int bl,int bw,int mc,int rs,int qi,int ay){return add(svm,&st.n_mkt,&st.t_qu,N-6,lc,bl,bw,mc,rs,qi,ay);}
void quagga_report(void){ps("[QUAG] Plains: ");pi(st.n_plains);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Mn=");pi(st.t_mn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Qu=");pi(st.t_qu);ps("\n");}
void quagga_state(void){ps("[QUAG] Plains=");pi(st.n_plains);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Quagga Admin Demo ===\n\n");quagga_init();
ps("Quagga plains...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=200+(i*18),mc=12+(i*1),rs=25+(i*3),qi=(i%8)+1,ay=(i%15)+1;quagga_plains(lc,bl,bw,mc,rs,qi,ay);}
ps("\nQuagga feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=210+(i*15),mc=13+(i*1),rs=27+(i*2),qi=(i%6)+1,ay=(i%12)+1;quagga_feeding(lc,bl,bw,mc,rs,qi,ay);}
ps("\nQuagga breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=220+(i*12),mc=14+(i*1),rs=23+(i*3),qi=(i%5)+1,ay=(i%10)+1;quagga_breeding(lc,bl,bw,mc,rs,qi,ay);}
ps("\nQuagga health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=190+(i*22),mc=11+(i*2),rs=21+(i*4),qi=(i%10)+1,ay=(i%8)+1;quagga_health(lc,bl,bw,mc,rs,qi,ay);}
ps("\nQuagga market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*5),bw=230+(i*10),mc=15+(i*1),rs=29+(i*2),qi=(i%4)+1,ay=(i%7)+1;quagga_market(lc,bl,bw,mc,rs,qi,ay);}
ps("\n");quagga_report();quagga_state();ps("\n=== Demo Complete ===\n");return 0;}
