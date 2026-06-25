/* halibut_admin: Halibut (Hippoglossus hippoglossus) Atlantic halibut flatfish (v1.0)
 * Halibut tank, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, eye_dia_cm, scale_idx, swim_depth_m, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,eye_dia,sc_idx,sw_dp,age_yr,active;} halibut_t;
typedef struct{int n_tank,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_eye,t_sc,t_sw;} halibut_state_t;
static halibut_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static halibut_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(halibut_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ed,int si,int sd,int ay){if(*cnt>=mx)return -1;halibut_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->eye_dia=ed;x->sc_idx=si;x->sw_dp=sd;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HAL] Halibut ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ed=");pi(ed);ps(" si=");pi(si);ps(" sd=");pi(sd);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int halibut_init(void){if(init)return -1;st.n_tank=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_eye=0;st.t_sc=0;st.t_sw=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HAL] Halibut initialized\n");return 0;}
int halibut_tank(int lc,int bl,int bw,int ed,int si,int sd,int ay){return add(gpl,&st.n_tank,&st.t_ln,N,lc,bl,bw,ed,si,sd,ay);}
int halibut_feeding(int lc,int bl,int bw,int ed,int si,int sd,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ed,si,sd,ay);}
int halibut_breeding(int lc,int bl,int bw,int ed,int si,int sd,int ay){return add(gbr,&st.n_breed,&st.t_eye,N-4,lc,bl,bw,ed,si,sd,ay);}
int halibut_health(int lc,int bl,int bw,int ed,int si,int sd,int ay){return add(ghl,&st.n_health,&st.t_sc,N-6,lc,bl,bw,ed,si,sd,ay);}
int halibut_market(int lc,int bl,int bw,int ed,int si,int sd,int ay){return add(gmk,&st.n_mkt,&st.t_sw,N-6,lc,bl,bw,ed,si,sd,ay);}
void halibut_report(void){ps("[HAL] Tank: ");pi(st.n_tank);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Eye=");pi(st.t_eye);ps("\nHlth: ");pi(st.n_health);ps(" Sc=");pi(st.t_sc);ps("\nMkt: ");pi(st.n_mkt);ps(" Sw=");pi(st.t_sw);ps("\n");}
void halibut_state(void){ps("[HAL] Tank=");pi(st.n_tank);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Halibut Admin Demo ===\n\n");halibut_init();
/* 1=ocean 2=coast 3=fjord 4=farm 5=tank */
ps("Halibut tank...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*8),bw=5+(i*2),ed=2+(i%3),si=(i%5)+1,sd=50+(i*10),ay=(i%5)+1;halibut_tank(lc,bl,bw,ed,si,sd,ay);}
ps("\nHalibut feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=85+(i*6),bw=6+(i*2),ed=3+(i%3),si=(i%4)+1,sd=55+(i*8),ay=(i%4)+1;halibut_feeding(lc,bl,bw,ed,si,sd,ay);}
ps("\nHalibut breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=75+(i*10),bw=7+(i*2),ed=2+(i%4),si=(i%3)+2,sd=45+(i*12),ay=(i%3)+2;halibut_breeding(lc,bl,bw,ed,si,sd,ay);}
ps("\nHalibut health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=90+(i*5),bw=8+(i*2),ed=4+(i%2),si=(i%5)+1,sd=60+(i*7),ay=(i%4)+1;halibut_health(lc,bl,bw,ed,si,sd,ay);}
ps("\nHalibut market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=95+(i*4),bw=9+(i*2),ed=5+(i%2),si=(i%4)+1,sd=65+(i*6),ay=(i%3)+3;halibut_market(lc,bl,bw,ed,si,sd,ay);}
ps("\n");halibut_report();halibut_state();ps("\n=== Demo Complete ===\n");return 0;}
