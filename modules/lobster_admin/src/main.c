/* lobster_admin: Lobster (Homarus americanus) American lobster crustacean (v1.0)
 * Lobster tank, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_cm, shell_hardness, color_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,cl_cm,sh_hd,cl_idx,age_yr,active;} lobster_t;
typedef struct{int n_tank,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_cl,t_sh,t_ci;} lobster_state_t;
static lobster_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static lobster_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lobster_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int sh,int ci,int ay){if(*cnt>=mx)return -1;lobster_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->cl_cm=cc;x->sh_hd=sh;x->cl_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[LOB] Lobster ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" sh=");pi(sh);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int lobster_init(void){if(init)return -1;st.n_tank=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_cl=0;st.t_sh=0;st.t_ci=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[LOB] Lobster initialized\n");return 0;}
int lobster_tank(int lc,int bl,int bw,int cc,int sh,int ci,int ay){return add(gpl,&st.n_tank,&st.t_ln,N,lc,bl,bw,cc,sh,ci,ay);}
int lobster_feeding(int lc,int bl,int bw,int cc,int sh,int ci,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,sh,ci,ay);}
int lobster_breeding(int lc,int bl,int bw,int cc,int sh,int ci,int ay){return add(gbr,&st.n_breed,&st.t_cl,N-4,lc,bl,bw,cc,sh,ci,ay);}
int lobster_health(int lc,int bl,int bw,int cc,int sh,int ci,int ay){return add(ghl,&st.n_health,&st.t_sh,N-6,lc,bl,bw,cc,sh,ci,ay);}
int lobster_market(int lc,int bl,int bw,int cc,int sh,int ci,int ay){return add(gmk,&st.n_mkt,&st.t_ci,N-6,lc,bl,bw,cc,sh,ci,ay);}
void lobster_report(void){ps("[LOB] Tank: ");pi(st.n_tank);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Cl=");pi(st.t_cl);ps("\nHlth: ");pi(st.n_health);ps(" Sh=");pi(st.t_sh);ps("\nMkt: ");pi(st.n_mkt);ps(" Ci=");pi(st.t_ci);ps("\n");}
void lobster_state(void){ps("[LOB] Tank=");pi(st.n_tank);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Lobster Admin Demo ===\n\n");lobster_init();
/* 1=tank 2=pound 3=ocean 4=trap 5=market */
ps("Lobster tank...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*3),bw=1+(i*2),cc=15+(i*2),sh=(i%5)+1,ci=(i%4)+1,ay=(i%5)+1;lobster_tank(lc,bl,bw,cc,sh,ci,ay);}
ps("\nLobster feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=32+(i*2),bw=1+(i*2),cc=16+(i*2),sh=(i%4)+1,ci=(i%3)+1,ay=(i%4)+1;lobster_feeding(lc,bl,bw,cc,sh,ci,ay);}
ps("\nLobster breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=28+(i*4),bw=2+(i*2),cc=14+(i*3),sh=(i%3)+2,ci=(i%3)+1,ay=(i%3)+1;lobster_breeding(lc,bl,bw,cc,sh,ci,ay);}
ps("\nLobster health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=35+(i*2),bw=2+(i*2),cc=18+(i*2),sh=(i%5)+1,ci=(i%4)+1,ay=(i%4)+1;lobster_health(lc,bl,bw,cc,sh,ci,ay);}
ps("\nLobster market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=38+(i*2),bw=3+(i*2),cc=20+(i*2),sh=(i%4)+1,ci=(i%3)+1,ay=(i%3)+1;lobster_market(lc,bl,bw,cc,sh,ci,ay);}
ps("\n");lobster_report();lobster_state();ps("\n=== Demo Complete ===\n");return 0;}
