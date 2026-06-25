/* swordfish_admin: Swordfish (Xiphias gladius) billfish (v1.0)
 * Swordfish ocean, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, sword_cm, swim_speed, fin_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,sword_cm,sw_sp,fn_idx,age_yr,active;} swfd_t;
typedef struct{int n_ocean,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_swd,t_sw,t_fn;} swfd_state_t;
static swfd_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static swfd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(swfd_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int ss,int fi,int ay){if(*cnt>=mx)return -1;swfd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->sword_cm=sc;x->sw_sp=ss;x->fn_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SWFD] Swordfish ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" ss=");pi(ss);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int swfd_init(void){if(init)return -1;st.n_ocean=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_swd=0;st.t_sw=0;st.t_fn=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SWFD] Swordfish initialized\n");return 0;}
int swfd_ocean(int lc,int bl,int bw,int sc,int ss,int fi,int ay){return add(spl,&st.n_ocean,&st.t_ln,N,lc,bl,bw,sc,ss,fi,ay);}
int swfd_feeding(int lc,int bl,int bw,int sc,int ss,int fi,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,ss,fi,ay);}
int swfd_breeding(int lc,int bl,int bw,int sc,int ss,int fi,int ay){return add(sbr,&st.n_breed,&st.t_swd,N-4,lc,bl,bw,sc,ss,fi,ay);}
int swfd_health(int lc,int bl,int bw,int sc,int ss,int fi,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,sc,ss,fi,ay);}
int swfd_market(int lc,int bl,int bw,int sc,int ss,int fi,int ay){return add(smk,&st.n_mkt,&st.t_fn,N-6,lc,bl,bw,sc,ss,fi,ay);}
void swfd_report(void){ps("[SWFD] Ocean: ");pi(st.n_ocean);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Swd=");pi(st.t_swd);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Fn=");pi(st.t_fn);ps("\n");}
void swfd_state(void){ps("[SWFD] Ocean=");pi(st.n_ocean);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Swordfish Admin Demo ===\n\n");swfd_init();
ps("Swordfish ocean...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*30),bw=80+(i*40),sc=80+(i*15),ss=25+(i*5),fi=(i%6)+1,ay=(i%5)+1;swfd_ocean(lc,bl,bw,sc,ss,fi,ay);}
ps("\nSwordfish feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=250+(i*20),bw=100+(i*30),sc=90+(i*10),ss=30+(i*4),fi=(i%5)+1,ay=(i%4)+1;swfd_feeding(lc,bl,bw,sc,ss,fi,ay);}
ps("\nSwordfish breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=180+(i*35),bw=60+(i*50),sc=70+(i*18),ss=20+(i*6),fi=(i%4)+1,ay=(i%3)+1;swfd_breeding(lc,bl,bw,sc,ss,fi,ay);}
ps("\nSwordfish health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=300+(i*15),bw=150+(i*20),sc=100+(i*8),ss=35+(i*3),fi=(i%7)+1,ay=(i%6)+2;swfd_health(lc,bl,bw,sc,ss,fi,ay);}
ps("\nSwordfish market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=350+(i*10),bw=200+(i*15),sc=110+(i*6),ss=15+(i*5),fi=(i%3)+3,ay=(i%4)+3;swfd_market(lc,bl,bw,sc,ss,fi,ay);}
ps("\n");swfd_report();swfd_state();ps("\n=== Demo Complete ===\n");return 0;}
