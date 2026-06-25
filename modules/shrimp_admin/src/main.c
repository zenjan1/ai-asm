/* shrimp_admin: Shrimp (Penaeus monodon) crustacean (v1.0)
 * Shrimp estuary, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, swim_speed, shell_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,sw_sp,sh_idx,age_yr,active;} shrm_t;
typedef struct{int n_est,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_sw,t_sh;} shrm_state_t;
static shrm_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static shrm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(shrm_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int si,int ay){if(*cnt>=mx)return -1;shrm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->sw_sp=ss;x->sh_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SHRM] Shrimp ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int shrm_init(void){if(init)return -1;st.n_est=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_sw=0;st.t_sh=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SHRM] Shrimp initialized\n");return 0;}
int shrm_estuary(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(spl,&st.n_est,&st.t_ln,N,lc,bl,bw,tc,ss,si,ay);}
int shrm_feeding(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,si,ay);}
int shrm_breeding(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(sbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,ss,si,ay);}
int shrm_health(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,tc,ss,si,ay);}
int shrm_market(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(smk,&st.n_mkt,&st.t_sh,N-6,lc,bl,bw,tc,ss,si,ay);}
void shrm_report(void){ps("[SHRM] Estuary: ");pi(st.n_est);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sh=");pi(st.t_sh);ps("\n");}
void shrm_state(void){ps("[SHRM] Est=");pi(st.n_est);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Shrimp Admin Demo ===\n\n");shrm_init();
ps("Shrimp estuary...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=8+(i*2),bw=10+(i*5),tc=6+(i%3),ss=4+(i*2),si=(i%6)+1,ay=(i%2)+1;shrm_estuary(lc,bl,bw,tc,ss,si,ay);}
ps("\nShrimp feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=10+(i*2),bw=15+(i*4),tc=7+(i%2),ss=5+(i*2),si=(i%5)+1,ay=(i%2)+1;shrm_feeding(lc,bl,bw,tc,ss,si,ay);}
ps("\nShrimp breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i),bw=20+(i*3),tc=8+(i%2),ss=3+(i%3),si=(i%4)+1,ay=(i%2)+1;shrm_breeding(lc,bl,bw,tc,ss,si,ay);}
ps("\nShrimp health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=6+(i*3),bw=8+(i*6),tc=5+(i%4),ss=6+(i%2),si=(i%7)+1,ay=(i%3)+2;shrm_health(lc,bl,bw,tc,ss,si,ay);}
ps("\nShrimp market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=14+(i),bw=25+(i*3),tc=9+(i%2),ss=2+(i*4),si=(i%3)+3,ay=(i%2)+2;shrm_market(lc,bl,bw,tc,ss,si,ay);}
ps("\n");shrm_report();shrm_state();ps("\n=== Demo Complete ===\n");return 0;}
