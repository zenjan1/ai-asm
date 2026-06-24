/* echidna_admin: Echidna (Tachyglossus aculeatus) spiny anteater monotreme (v1.0)
 * Echidna housing, feeding, burrowing, health, market
 * Features: body_len_cm, body_wt_kg, spine_len_mm, claw_idx, snout_cm, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,sp_mm,clw_idx,snt_cm,age_yr,active;} echd_t;
typedef struct{int n_house,n_feed,n_burr,n_hlth,n_mkt,t_ln,t_wt,t_sp,t_cl,t_sn;} echd_state_t;
static echd_t ehs[N],efd[N-2],ebr[N-4],ehl[N-6],emk[N-6]; static echd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(echd_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sm,int ci,int sn,int ay){if(*cnt>=mx)return -1;echd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->sp_mm=sm;x->clw_idx=ci;x->snt_cm=sn;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ECHD] Echidna ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sm=");pi(sm);ps(" ci=");pi(ci);ps(" sn=");pi(sn);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int echd_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_burr=0;st.n_hlth=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_sp=0;st.t_cl=0;st.t_sn=0;for(int i=0;i<N;i++)ehs[i].active=0;for(int i=0;i<N-2;i++)efd[i].active=0;for(int i=0;i<N-4;i++)ebr[i].active=0;for(int i=0;i<N-6;i++)ehl[i].active=0;for(int i=0;i<N-6;i++)emk[i].active=0;init=1;ps("[ECHD] Echidna initialized\n");return 0;}
int echd_housing(int lc,int bl,int bw,int sm,int ci,int sn,int ay){return add(ehs,&st.n_house,&st.t_ln,N,lc,bl,bw,sm,ci,sn,ay);}
int echd_feeding(int lc,int bl,int bw,int sm,int ci,int sn,int ay){return add(efd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sm,ci,sn,ay);}
int echd_burrowing(int lc,int bl,int bw,int sm,int ci,int sn,int ay){return add(ebr,&st.n_burr,&st.t_sp,N-4,lc,bl,bw,sm,ci,sn,ay);}
int echd_health(int lc,int bl,int bw,int sm,int ci,int sn,int ay){return add(ehl,&st.n_hlth,&st.t_cl,N-6,lc,bl,bw,sm,ci,sn,ay);}
int echd_market(int lc,int bl,int bw,int sm,int ci,int sn,int ay){return add(emk,&st.n_mkt,&st.t_sn,N-6,lc,bl,bw,sm,ci,sn,ay);}
void echd_report(void){ps("[ECHD] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBurr: ");pi(st.n_burr);ps(" Sp=");pi(st.t_sp);ps("\nHlth: ");pi(st.n_hlth);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sn=");pi(st.t_sn);ps("\n");}
void echd_state(void){ps("[ECHD] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Burr=");pi(st.n_burr);ps(" Hlth=");pi(st.n_hlth);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Echidna Admin Demo ===\n\n");echd_init();
/* 1=enclosure 2=bush 3=sanctuary 4=zoo 5=market */
ps("Echidna housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=5+(i*2),sm=20+(i*3),ci=(i%5)+1,sn=6+(i*2),ay=1+(i%8);echd_housing(lc,bl,bw,sm,ci,sn,ay);}
ps("\nEchidna feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*3),bw=6+(i*2),sm=22+(i*2),ci=(i%4)+1,sn=7+(i%2),ay=2+(i%6);echd_feeding(lc,bl,bw,sm,ci,sn,ay);}
ps("\nEchidna burrowing...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=38+(i*4),bw=4+(i*3),sm=18+(i*4),ci=(i%6)+1,sn=5+(i%3),ay=2+(i%5);echd_burrowing(lc,bl,bw,sm,ci,sn,ay);}
ps("\nEchidna health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=45+(i*3),bw=7+(i*2),sm=25+(i*3),ci=(i%7)+1,sn=8+(i%2),ay=3+(i%7);echd_health(lc,bl,bw,sm,ci,sn,ay);}
ps("\nEchidna market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=48+(i*2),bw=8+(i%2),sm=28+(i*2),ci=(i%3)+4,sn=9+(i%2),ay=3+(i%4);echd_market(lc,bl,bw,sm,ci,sn,ay);}
ps("\n");echd_report();echd_state();ps("\n=== Demo Complete ===\n");return 0;}
