/* swan_admin: Swan (Cygnus olor) large waterfowl (v1.0)
 * Swan lake, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_cm, swim_speed, neck_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,sw_sp,nk_idx,age_yr,active;} swan_t;
typedef struct{int n_lake,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_sw,t_nk;} swan_state_t;
static swan_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static swan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(swan_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int ss,int ni,int ay){if(*cnt>=mx)return -1;swan_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->sw_sp=ss;x->nk_idx=ni;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SWAN] Swan ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" ss=");pi(ss);ps(" ni=");pi(ni);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int swan_init(void){if(init)return -1;st.n_lake=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_sw=0;st.t_nk=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SWAN] Swan initialized\n");return 0;}
int swan_lake(int lc,int bl,int bw,int wc,int ss,int ni,int ay){return add(spl,&st.n_lake,&st.t_ln,N,lc,bl,bw,wc,ss,ni,ay);}
int swan_feeding(int lc,int bl,int bw,int wc,int ss,int ni,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,ss,ni,ay);}
int swan_breeding(int lc,int bl,int bw,int wc,int ss,int ni,int ay){return add(sbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,ss,ni,ay);}
int swan_health(int lc,int bl,int bw,int wc,int ss,int ni,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,wc,ss,ni,ay);}
int swan_market(int lc,int bl,int bw,int wc,int ss,int ni,int ay){return add(smk,&st.n_mkt,&st.t_nk,N-6,lc,bl,bw,wc,ss,ni,ay);}
void swan_report(void){ps("[SWAN] Lake: ");pi(st.n_lake);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Nk=");pi(st.t_nk);ps("\n");}
void swan_state(void){ps("[SWAN] Lake=");pi(st.n_lake);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Swan Admin Demo ===\n\n");swan_init();
ps("Swan lake...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*10),bw=8+(i*3),wc=180+(i*8),ss=5+(i*2),ni=(i%6)+1,ay=(i%5)+1;swan_lake(lc,bl,bw,wc,ss,ni,ay);}
ps("\nSwan feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=130+(i*8),bw=10+(i*2),wc=190+(i*6),ss=6+(i*2),ni=(i%5)+1,ay=(i%4)+1;swan_feeding(lc,bl,bw,wc,ss,ni,ay);}
ps("\nSwan breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=110+(i*12),bw=6+(i*4),wc=170+(i*10),ss=4+(i*3),ni=(i%4)+1,ay=(i%3)+1;swan_breeding(lc,bl,bw,wc,ss,ni,ay);}
ps("\nSwan health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=140+(i*6),bw=12+(i*2),wc=200+(i*5),ss=7+(i),ni=(i%7)+1,ay=(i%6)+2;swan_health(lc,bl,bw,wc,ss,ni,ay);}
ps("\nSwan market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=150+(i*5),bw=14+(i),wc=210+(i*4),ss=3+(i*2),ni=(i%3)+3,ay=(i%4)+3;swan_market(lc,bl,bw,wc,ss,ni,ay);}
ps("\n");swan_report();swan_state();ps("\n=== Demo Complete ===\n");return 0;}
