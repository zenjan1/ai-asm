/* squid_admin: Squid (Dosidicus gigas) cephalopod (v1.0)
 * Squid deep_sea, feeding, breeding, health, market
 * Features: mantle_cm, body_wt_kg, tentacle_cm, swim_speed, spot_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,mntl_cm,bdy_wt,tnt_cm,sw_sp,sp_idx,age_yr,active;} squid_t;
typedef struct{int n_deep,n_feed,n_breed,n_health,n_mkt,t_mn,t_wt,t_tn,t_sw,t_sp;} squid_state_t;
static squid_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static squid_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(squid_t*a,int*cnt,int*sum,int mx,int lc,int mc,int bw,int tc,int ss,int si,int ay){if(*cnt>=mx)return -1;squid_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->mntl_cm=mc;x->bdy_wt=bw;x->tnt_cm=tc;x->sw_sp=ss;x->sp_idx=si;x->age_yr=ay;x->active=1;*sum+=mc;(*cnt)++;ps("[SQID] Squid ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" mc=");pi(mc);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int squid_init(void){if(init)return -1;st.n_deep=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_mn=0;st.t_wt=0;st.t_tn=0;st.t_sw=0;st.t_sp=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SQID] Squid initialized\n");return 0;}
int squid_deep_sea(int lc,int mc,int bw,int tc,int ss,int si,int ay){return add(spl,&st.n_deep,&st.t_mn,N,lc,mc,bw,tc,ss,si,ay);}
int squid_feeding(int lc,int mc,int bw,int tc,int ss,int si,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,mc,bw,tc,ss,si,ay);}
int squid_breeding(int lc,int mc,int bw,int tc,int ss,int si,int ay){return add(sbr,&st.n_breed,&st.t_tn,N-4,lc,mc,bw,tc,ss,si,ay);}
int squid_health(int lc,int mc,int bw,int tc,int ss,int si,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,mc,bw,tc,ss,si,ay);}
int squid_market(int lc,int mc,int bw,int tc,int ss,int si,int ay){return add(smk,&st.n_mkt,&st.t_sp,N-6,lc,mc,bw,tc,ss,si,ay);}
void squid_report(void){ps("[SQID] Deep: ");pi(st.n_deep);ps(" Mn=");pi(st.t_mn);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tn=");pi(st.t_tn);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void squid_state(void){ps("[SQID] Deep=");pi(st.n_deep);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Squid Admin Demo ===\n\n");squid_init();
ps("Squid deep_sea...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,mc=40+(i*10),bw=5+(i*3),tc=30+(i*8),ss=15+(i*5),si=(i%6)+1,ay=(i%3)+1;squid_deep_sea(lc,mc,bw,tc,ss,si,ay);}
ps("\nSquid feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,mc=50+(i*8),bw=8+(i*2),tc=35+(i*6),ss=18+(i*4),si=(i%5)+1,ay=(i%3)+1;squid_feeding(lc,mc,bw,tc,ss,si,ay);}
ps("\nSquid breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,mc=35+(i*12),bw=4+(i*4),tc=25+(i*10),ss=12+(i*6),si=(i%4)+1,ay=(i%2)+1;squid_breeding(lc,mc,bw,tc,ss,si,ay);}
ps("\nSquid health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,mc=60+(i*6),bw=10+(i*2),tc=40+(i*5),ss=20+(i*3),si=(i%7)+1,ay=(i%4)+2;squid_health(lc,mc,bw,tc,ss,si,ay);}
ps("\nSquid market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,mc=70+(i*5),bw=12+(i*2),tc=45+(i*4),ss=10+(i*5),si=(i%3)+3,ay=(i%2)+2;squid_market(lc,mc,bw,tc,ss,si,ay);}
ps("\n");squid_report();squid_state();ps("\n=== Demo Complete ===\n");return 0;}
