/* avocet_admin: Avocet (Recurvirostra avosetta) pied wading shorebird (v1.0)
 * Avocet wetland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, bill_cm, wade_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,bill_cm,wd_sp,pm_idx,age_yr,active;} avct_t;
typedef struct{int n_wet,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_bill,t_wd,t_pm;} avct_state_t;
static avct_t wtl[N],wtf[N-2],wtb[N-4],wth[N-6],wtm[N-6]; static avct_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(avct_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int ws,int pi2,int ay){if(*cnt>=mx)return -1;avct_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->bill_cm=bc;x->wd_sp=ws;x->pm_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[AVCT] Avocet ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" ws=");pi(ws);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int avocet_init(void){if(init)return -1;st.n_wet=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_bill=0;st.t_wd=0;st.t_pm=0;for(int i=0;i<N;i++)wtl[i].active=0;for(int i=0;i<N-2;i++)wtf[i].active=0;for(int i=0;i<N-4;i++)wtb[i].active=0;for(int i=0;i<N-6;i++)wth[i].active=0;for(int i=0;i<N-6;i++)wtm[i].active=0;init=1;ps("[AVCT] Avocet initialized\n");return 0;}
int avocet_wetland(int lc,int bl,int bw,int bc,int ws,int pi2,int ay){return add(wtl,&st.n_wet,&st.t_ln,N,lc,bl,bw,bc,ws,pi2,ay);}
int avocet_feeding(int lc,int bl,int bw,int bc,int ws,int pi2,int ay){return add(wtf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,ws,pi2,ay);}
int avocet_breeding(int lc,int bl,int bw,int bc,int ws,int pi2,int ay){return add(wtb,&st.n_breed,&st.t_bill,N-4,lc,bl,bw,bc,ws,pi2,ay);}
int avocet_health(int lc,int bl,int bw,int bc,int ws,int pi2,int ay){return add(wth,&st.n_health,&st.t_wd,N-6,lc,bl,bw,bc,ws,pi2,ay);}
int avocet_market(int lc,int bl,int bw,int bc,int ws,int pi2,int ay){return add(wtm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,bc,ws,pi2,ay);}
void avocet_report(void){ps("[AVCT] Wet: ");pi(st.n_wet);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Bill=");pi(st.t_bill);ps("\nHealth: ");pi(st.n_health);ps(" Wd=");pi(st.t_wd);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void avocet_state(void){ps("[AVCT] Wet=");pi(st.n_wet);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Avocet Admin Demo ===\n\n");avocet_init();
ps("Avocet wetland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*2),bw=25+(i*5),bc=8+(i*1),ws=3+(i*1),pi2=(i%8)+1,ay=(i%10)+1;avocet_wetland(lc,bl,bw,bc,ws,pi2,ay);}
ps("\nAvocet feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*2),bw=28+(i*4),bc=9+(i*1),ws=4+(i*1),pi2=(i%6)+1,ay=(i%8)+1;avocet_feeding(lc,bl,bw,bc,ws,pi2,ay);}
ps("\nAvocet breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=39+(i*2),bw=30+(i*3),bc=10+(i*1),ws=2+(i*2),pi2=(i%5)+1,ay=(i%7)+1;avocet_breeding(lc,bl,bw,bc,ws,pi2,ay);}
ps("\nAvocet health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=34+(i*3),bw=24+(i*6),bc=7+(i*2),ws=2+(i*2),pi2=(i%10)+1,ay=(i%6)+1;avocet_health(lc,bl,bw,bc,ws,pi2,ay);}
ps("\nAvocet market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=40+(i*2),bw=32+(i*3),bc=11+(i*1),ws=5+(i*1),pi2=(i%4)+1,ay=(i%5)+1;avocet_market(lc,bl,bw,bc,ws,pi2,ay);}
ps("\n");avocet_report();avocet_state();ps("\n=== Demo Complete ===\n");return 0;}
