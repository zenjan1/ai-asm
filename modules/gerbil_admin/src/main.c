/* gerbil_admin: Gerbil (Meriones unguiculatus) mongolian gerbil rodent (v1.0)
 * Gerbil housing, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, burrow_idx, chirp_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,brw_idx,chrp_vl,age_yr,active;} gerbil_t;
typedef struct{int n_hous,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_brw,t_chrp;} gerbil_state_t;
static gerbil_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static gerbil_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gerbil_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int bi,int cv,int ay){if(*cnt>=mx)return -1;gerbil_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->brw_idx=bi;x->chrp_vl=cv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GERB] Gerbil ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" bi=");pi(bi);ps(" cv=");pi(cv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gerbil_init(void){if(init)return -1;st.n_hous=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_brw=0;st.t_chrp=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GERB] Gerbil initialized\n");return 0;}
int gerbil_housing(int lc,int bl,int bw,int tc,int bi,int cv,int ay){return add(gpl,&st.n_hous,&st.t_ln,N,lc,bl,bw,tc,bi,cv,ay);}
int gerbil_feeding(int lc,int bl,int bw,int tc,int bi,int cv,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,bi,cv,ay);}
int gerbil_breeding(int lc,int bl,int bw,int tc,int bi,int cv,int ay){return add(gbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,bi,cv,ay);}
int gerbil_health(int lc,int bl,int bw,int tc,int bi,int cv,int ay){return add(ghl,&st.n_health,&st.t_brw,N-6,lc,bl,bw,tc,bi,cv,ay);}
int gerbil_market(int lc,int bl,int bw,int tc,int bi,int cv,int ay){return add(gmk,&st.n_mkt,&st.t_chrp,N-6,lc,bl,bw,tc,bi,cv,ay);}
void gerbil_report(void){ps("[GERB] Hous: ");pi(st.n_hous);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHlth: ");pi(st.n_health);ps(" Brw=");pi(st.t_brw);ps("\nMkt: ");pi(st.n_mkt);ps(" Chrp=");pi(st.t_chrp);ps("\n");}
void gerbil_state(void){ps("[GERB] Hous=");pi(st.n_hous);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gerbil Admin Demo ===\n\n");gerbil_init();
/* 1=cage 2=burrow 3=tank 4=tube 5=pen */
ps("Gerbil housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=8+(i*2),bw=40+(i*5),tc=6+(i*2),bi=(i%5)+1,cv=10+(i*6),ay=(i%4)+1;gerbil_housing(lc,bl,bw,tc,bi,cv,ay);}
ps("\nGerbil feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=9+(i*2),bw=45+(i*4),tc=7+(i*2),bi=(i%4)+1,cv=15+(i*5),ay=(i%3)+1;gerbil_feeding(lc,bl,bw,tc,bi,cv,ay);}
ps("\nGerbil breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=7+(i*3),bw=38+(i*6),tc=5+(i*2),bi=(i%3)+2,cv=12+(i*7),ay=(i%3)+2;gerbil_breeding(lc,bl,bw,tc,bi,cv,ay);}
ps("\nGerbil health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=10+(i*2),bw=50+(i*4),tc=8+(i),bi=(i%5)+1,cv=20+(i*5),ay=(i%4)+1;gerbil_health(lc,bl,bw,tc,bi,cv,ay);}
ps("\nGerbil market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=12+(i*2),bw=55+(i*3),tc=9+(i),bi=(i%4)+1,cv=25+(i*4),ay=(i%3)+2;gerbil_market(lc,bl,bw,tc,bi,cv,ay);}
ps("\n");gerbil_report();gerbil_state();ps("\n=== Demo Complete ===\n");return 0;}
