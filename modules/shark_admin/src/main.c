/* shark_admin: Shark (Carcharodon carcharias) apex predator (v1.0)
 * Shark pelagic, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tooth_cm, swim_speed, fin_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tooth_cm,sw_sp,fn_idx,age_yr,active;} shrk_t;
typedef struct{int n_pelag,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tooth,t_sw,t_fn;} shrk_state_t;
static shrk_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static shrk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(shrk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int fi,int ay){if(*cnt>=mx)return -1;shrk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tooth_cm=tc;x->sw_sp=ss;x->fn_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SHRK] Shark ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int shrk_init(void){if(init)return -1;st.n_pelag=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tooth=0;st.t_sw=0;st.t_fn=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SHRK] Shark initialized\n");return 0;}
int shrk_pelagic(int lc,int bl,int bw,int tc,int ss,int fi,int ay){return add(spl,&st.n_pelag,&st.t_ln,N,lc,bl,bw,tc,ss,fi,ay);}
int shrk_feeding(int lc,int bl,int bw,int tc,int ss,int fi,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,fi,ay);}
int shrk_breeding(int lc,int bl,int bw,int tc,int ss,int fi,int ay){return add(sbr,&st.n_breed,&st.t_tooth,N-4,lc,bl,bw,tc,ss,fi,ay);}
int shrk_health(int lc,int bl,int bw,int tc,int ss,int fi,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,tc,ss,fi,ay);}
int shrk_market(int lc,int bl,int bw,int tc,int ss,int fi,int ay){return add(smk,&st.n_mkt,&st.t_fn,N-6,lc,bl,bw,tc,ss,fi,ay);}
void shrk_report(void){ps("[SHRK] Pelagic: ");pi(st.n_pelag);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tooth=");pi(st.t_tooth);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Fn=");pi(st.t_fn);ps("\n");}
void shrk_state(void){ps("[SHRK] Pelag=");pi(st.n_pelag);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Shark Admin Demo ===\n\n");shrk_init();
ps("Shark pelagic...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*30),bw=500+(i*100),tc=5+(i%4),ss=20+(i*5),fi=(i%5)+1,ay=(i%8)+2;shrk_pelagic(lc,bl,bw,tc,ss,fi,ay);}
ps("\nShark feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=250+(i*20),bw=600+(i*80),tc=6+(i%3),ss=25+(i*4),fi=(i%4)+1,ay=(i%6)+3;shrk_feeding(lc,bl,bw,tc,ss,fi,ay);}
ps("\nShark breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=180+(i*25),bw=400+(i*120),tc=4+(i%5),ss=15+(i*6),fi=(i%6)+1,ay=(i%5)+5;shrk_breeding(lc,bl,bw,tc,ss,fi,ay);}
ps("\nShark health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=300+(i*15),bw=800+(i*60),tc=7+(i%2),ss=18+(i*3),fi=(i%3)+1,ay=(i%7)+4;shrk_health(lc,bl,bw,tc,ss,fi,ay);}
ps("\nShark market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=350+(i*10),bw=900+(i*50),tc=8+(i%2),ss=12+(i*4),fi=(i%7)+2,ay=(i%4)+6;shrk_market(lc,bl,bw,tc,ss,fi,ay);}
ps("\n");shrk_report();shrk_state();ps("\n=== Demo Complete ===\n");return 0;}
