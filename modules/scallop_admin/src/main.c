/* scallop_admin: Scallop (Pecten maximus) bivalve mollusk (v1.0)
 * Scallop seabed, feeding, breeding, health, market
 * Features: shell_cm, body_wt_g, adductor_cm, swim_speed, shell_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,shl_cm,bdy_wt,add_cm,sw_sp,sh_idx,age_yr,active;} sclp_t;
typedef struct{int n_seabd,n_feed,n_breed,n_health,n_mkt,t_sh,t_wt,t_ad,t_sw,t_si;} sclp_state_t;
static sclp_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static sclp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sclp_t*a,int*cnt,int*sum,int mx,int lc,int sc,int bw,int ac,int ss,int si,int ay){if(*cnt>=mx)return -1;sclp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->shl_cm=sc;x->bdy_wt=bw;x->add_cm=ac;x->sw_sp=ss;x->sh_idx=si;x->age_yr=ay;x->active=1;*sum+=sc;(*cnt)++;ps("[SCLP] Scallop ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sc=");pi(sc);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int sclp_init(void){if(init)return -1;st.n_seabd=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_sh=0;st.t_wt=0;st.t_ad=0;st.t_sw=0;st.t_si=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SCLP] Scallop initialized\n");return 0;}
int sclp_seabed(int lc,int sc,int bw,int ac,int ss,int si,int ay){return add(spl,&st.n_seabd,&st.t_sh,N,lc,sc,bw,ac,ss,si,ay);}
int sclp_feeding(int lc,int sc,int bw,int ac,int ss,int si,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,sc,bw,ac,ss,si,ay);}
int sclp_breeding(int lc,int sc,int bw,int ac,int ss,int si,int ay){return add(sbr,&st.n_breed,&st.t_ad,N-4,lc,sc,bw,ac,ss,si,ay);}
int sclp_health(int lc,int sc,int bw,int ac,int ss,int si,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,sc,bw,ac,ss,si,ay);}
int sclp_market(int lc,int sc,int bw,int ac,int ss,int si,int ay){return add(smk,&st.n_mkt,&st.t_si,N-6,lc,sc,bw,ac,ss,si,ay);}
void sclp_report(void){ps("[SCLP] Seabed: ");pi(st.n_seabd);ps(" Sh=");pi(st.t_sh);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ad=");pi(st.t_ad);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Si=");pi(st.t_si);ps("\n");}
void sclp_state(void){ps("[SCLP] Seabd=");pi(st.n_seabd);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Scallop Admin Demo ===\n\n");sclp_init();
ps("Scallop seabed...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sc=8+(i*2),bw=40+(i*10),ac=3+(i%3),ss=1+(i%3),si=(i%6)+1,ay=(i%4)+1;sclp_seabed(lc,sc,bw,ac,ss,si,ay);}
ps("\nScallop feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,sc=10+(i*2),bw=50+(i*8),ac=4+(i%2),ss=1+(i%2),si=(i%5)+1,ay=(i%3)+1;sclp_feeding(lc,sc,bw,ac,ss,si,ay);}
ps("\nScallop breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sc=12+(i),bw=60+(i*6),ac=5+(i%2),ss=0+(i%2),si=(i%4)+1,ay=(i%3)+2;sclp_breeding(lc,sc,bw,ac,ss,si,ay);}
ps("\nScallop health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sc=7+(i*3),bw=35+(i*12),ac=2+(i%4),ss=2+(i%2),si=(i%7)+1,ay=(i%5)+1;sclp_health(lc,sc,bw,ac,ss,si,ay);}
ps("\nScallop market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sc=14+(i),bw=70+(i*5),ac=6+(i%2),ss=0+(i%3),si=(i%3)+3,ay=(i%2)+3;sclp_market(lc,sc,bw,ac,ss,si,ay);}
ps("\n");sclp_report();sclp_state();ps("\n=== Demo Complete ===\n");return 0;}
