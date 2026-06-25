/* tiger_admin: Tiger (Panthera tigris) big cat predator (v1.0)
 * Tiger jungle, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_cm, run_speed, stripe_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,run_sp,st_idx,age_yr,active;} tigr_t;
typedef struct{int n_jungle,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_run,t_st;} tigr_state_t;
static tigr_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static tigr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tigr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int rs,int si,int ay){if(*cnt>=mx)return -1;tigr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->run_sp=rs;x->st_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TGR] Tiger ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" rs=");pi(rs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int tigr_init(void){if(init)return -1;st.n_jungle=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_run=0;st.t_st=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[TGR] Tiger initialized\n");return 0;}
int tigr_jungle(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(spl,&st.n_jungle,&st.t_ln,N,lc,bl,bw,cc,rs,si,ay);}
int tigr_feeding(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,rs,si,ay);}
int tigr_breeding(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(sbr,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,rs,si,ay);}
int tigr_health(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(shl,&st.n_health,&st.t_run,N-6,lc,bl,bw,cc,rs,si,ay);}
int tigr_market(int lc,int bl,int bw,int cc,int rs,int si,int ay){return add(smk,&st.n_mkt,&st.t_st,N-6,lc,bl,bw,cc,rs,si,ay);}
void tigr_report(void){ps("[TGR] Jungle: ");pi(st.n_jungle);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void tigr_state(void){ps("[TGR] Jungle=");pi(st.n_jungle);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tiger Admin Demo ===\n\n");tigr_init();
ps("Tiger jungle...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=180+(i*15),bw=150+(i*30),cc=8+(i%4),rs=40+(i*5),si=(i%6)+1,ay=(i%5)+1;tigr_jungle(lc,bl,bw,cc,rs,si,ay);}
ps("\nTiger feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=200+(i*12),bw=170+(i*25),cc=9+(i%3),rs=45+(i*4),si=(i%5)+1,ay=(i%4)+1;tigr_feeding(lc,bl,bw,cc,rs,si,ay);}
ps("\nTiger breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=160+(i*18),bw=130+(i*35),cc=7+(i%5),rs=35+(i*6),si=(i%4)+1,ay=(i%3)+1;tigr_breeding(lc,bl,bw,cc,rs,si,ay);}
ps("\nTiger health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=220+(i*10),bw=200+(i*20),cc=10+(i%2),rs=50+(i*3),si=(i%7)+1,ay=(i%6)+2;tigr_health(lc,bl,bw,cc,rs,si,ay);}
ps("\nTiger market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=240+(i*8),bw=220+(i*15),cc=11+(i%2),rs=30+(i*5),si=(i%3)+3,ay=(i%4)+3;tigr_market(lc,bl,bw,cc,rs,si,ay);}
ps("\n");tigr_report();tigr_state();ps("\n=== Demo Complete ===\n");return 0;}
