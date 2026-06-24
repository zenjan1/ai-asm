/* cricket_admin: Cricket (Gryllus assimilis) insect farming (v1.0)
 * Cricket breeding, feeding, housing, harvest, market
 * Features: body_len_mm, body_wt_mg, leg_len_mm, chirp_rate, protein_pct, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_ln,chirp_rt,prot_pct,harv_wk,active;} crkt_t;
typedef struct{int n_breed,n_feed,n_house,n_harv,n_mkt,t_ln,t_wt,t_leg,t_chirp,t_prot;} crkt_state_t;
static crkt_t crktbr[N],crktfd[N-2],crkths[N-4],crkthv[N-6],crktmk[N-6]; static crkt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crkt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ll,int cr,int pp,int hw){if(*cnt>=mx)return -1;crkt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_ln=ll;x->chirp_rt=cr;x->prot_pct=pp;x->harv_wk=hw;x->active=1;*sum+=bl;(*cnt)++;ps("[CRKT] Cricket ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ll=");pi(ll);ps(" cr=");pi(cr);ps(" pp=");pi(pp);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int crkt_init(void){if(init)return -1;st.n_breed=0;st.n_feed=0;st.n_house=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_chirp=0;st.t_prot=0;for(int i=0;i<N;i++)crktbr[i].active=0;for(int i=0;i<N-2;i++)crktfd[i].active=0;for(int i=0;i<N-4;i++)crkths[i].active=0;for(int i=0;i<N-6;i++)crkthv[i].active=0;for(int i=0;i<N-6;i++)crktmk[i].active=0;init=1;ps("[CRKT] Cricket initialized\n");return 0;}
int crkt_breeding(int lc,int bl,int bw,int ll,int cr,int pp,int hw){return add(crktbr,&st.n_breed,&st.t_ln,N,lc,bl,bw,ll,cr,pp,hw);}
int crkt_feeding(int lc,int bl,int bw,int ll,int cr,int pp,int hw){return add(crktfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ll,cr,pp,hw);}
int crkt_housing(int lc,int bl,int bw,int ll,int cr,int pp,int hw){return add(crkths,&st.n_house,&st.t_leg,N-4,lc,bl,bw,ll,cr,pp,hw);}
int crkt_harvest(int lc,int bl,int bw,int ll,int cr,int pp,int hw){return add(crkthv,&st.n_harv,&st.t_chirp,N-6,lc,bl,bw,ll,cr,pp,hw);}
int crkt_market(int lc,int bl,int bw,int ll,int cr,int pp,int hw){return add(crktmk,&st.n_mkt,&st.t_prot,N-6,lc,bl,bw,ll,cr,pp,hw);}
void crkt_report(void){ps("[CRKT] Breed: ");pi(st.n_breed);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nHouse: ");pi(st.n_house);ps(" Leg=");pi(st.t_leg);ps("\nHarv: ");pi(st.n_harv);ps(" Chirp=");pi(st.t_chirp);ps("\nMkt: ");pi(st.n_mkt);ps(" Prot=");pi(st.t_prot);ps("\n");}
void crkt_state(void){ps("[CRKT] Breed=");pi(st.n_breed);ps(" Feed=");pi(st.n_feed);ps(" House=");pi(st.n_house);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cricket Admin Demo ===\n\n");crkt_init();
/* 1=farm 2=container 3=greenhouse 4=indoor 5=market */
ps("Cricket breeding...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=15+(i*2),bw=30+(i*8),ll=8+(i*2),cr=40+(i*10),pp=55+(i*3),hw=4+(i%4);crkt_breeding(lc,bl,bw,ll,cr,pp,hw);}
ps("\nCricket feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=16+(i*2),bw=32+(i*7),ll=9+(i*2),cr=42+(i*9),pp=56+(i*3),hw=5+(i%3);crkt_feeding(lc,bl,bw,ll,cr,pp,hw);}
ps("\nCricket housing...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=17+(i*2),bw=34+(i*6),ll=10+(i*2),cr=44+(i*8),pp=57+(i*2),hw=6+(i%3);crkt_housing(lc,bl,bw,ll,cr,pp,hw);}
ps("\nCricket harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i*3),bw=28+(i*10),ll=7+(i*3),cr=38+(i*12),pp=53+(i*4),hw=3+(i%5);crkt_harvest(lc,bl,bw,ll,cr,pp,hw);}
ps("\nCricket market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=18+(i*2),bw=36+(i*5),ll=11+(i*2),cr=46+(i*7),pp=58+(i*2),hw=7+(i%2);crkt_market(lc,bl,bw,ll,cr,pp,hw);}
ps("\n");crkt_report();crkt_state();ps("\n=== Demo Complete ===\n");return 0;}
