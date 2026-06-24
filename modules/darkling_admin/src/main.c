/* darkling_admin: Darkling (Tenebrio molitor) darkling beetle farming (v1.0)
 * Darkling housing, feeding, breeding, harvest, market
 * Features: body_len_cm, body_wt_g, leg_count, shell_idx, larva_wt, age_day
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_ct,sh_idx,lv_wt,age_dy,active;} dark_t;
typedef struct{int n_house,n_feed,n_breed,n_harv,n_mkt,t_ln,t_wt,t_leg,t_sh,t_lv;} dark_state_t;
static dark_t dhs[N],dfd[N-2],dbr[N-4],dhv[N-6],dmk[N-6]; static dark_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dark_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lc2,int si,int lw,int ad){if(*cnt>=mx)return -1;dark_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_ct=lc2;x->sh_idx=si;x->lv_wt=lw;x->age_dy=ad;x->active=1;*sum+=bl;(*cnt)++;ps("[DARK] Darkling ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lc2=");pi(lc2);ps(" si=");pi(si);ps(" lw=");pi(lw);ps(" ad=");pi(ad);ps("\n");return *cnt-1;}
int dark_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_sh=0;st.t_lv=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dbr[i].active=0;for(int i=0;i<N-6;i++)dhv[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DARK] Darkling initialized\n");return 0;}
int dark_housing(int lc,int bl,int bw,int lc2,int si,int lw,int ad){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,lc2,si,lw,ad);}
int dark_feeding(int lc,int bl,int bw,int lc2,int si,int lw,int ad){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lc2,si,lw,ad);}
int dark_breeding(int lc,int bl,int bw,int lc2,int si,int lw,int ad){return add(dbr,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,lc2,si,lw,ad);}
int dark_harvest(int lc,int bl,int bw,int lc2,int si,int lw,int ad){return add(dhv,&st.n_harv,&st.t_sh,N-6,lc,bl,bw,lc2,si,lw,ad);}
int dark_market(int lc,int bl,int bw,int lc2,int si,int lw,int ad){return add(dmk,&st.n_mkt,&st.t_lv,N-6,lc,bl,bw,lc2,si,lw,ad);}
void dark_report(void){ps("[DARK] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHarv: ");pi(st.n_harv);ps(" Sh=");pi(st.t_sh);ps("\nMkt: ");pi(st.n_mkt);ps(" Lv=");pi(st.t_lv);ps("\n");}
void dark_state(void){ps("[DARK] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Darkling Admin Demo ===\n\n");dark_init();
/* 1=bin 2=farm 3=lab 4=container 5=market */
ps("Darkling housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=2+(i%2),bw=1+(i%3),lc2=6,si=(i%5)+1,lw=5+(i*3),ad=5+(i*4);dark_housing(lc,bl,bw,lc2,si,lw,ad);}
ps("\nDarkling feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=3+(i%2),bw=2+(i%2),lc2=6,si=(i%4)+1,lw=6+(i*2),ad=8+(i*3);dark_feeding(lc,bl,bw,lc2,si,lw,ad);}
ps("\nDarkling breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=2+(i%3),bw=1+(i%3),lc2=6,si=(i%6)+1,lw=4+(i*4),ad=10+(i*3);dark_breeding(lc,bl,bw,lc2,si,lw,ad);}
ps("\nDarkling harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=3+(i%2),bw=2+(i%2),lc2=6,si=(i%5)+1,lw=7+(i*3),ad=15+(i*4);dark_harvest(lc,bl,bw,lc2,si,lw,ad);}
ps("\nDarkling market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=4+(i%2),bw=2+(i%2),lc2=6,si=(i%3)+3,lw=8+(i*2),ad=20+(i*3);dark_market(lc,bl,bw,lc2,si,lw,ad);}
ps("\n");dark_report();dark_state();ps("\n=== Demo Complete ===\n");return 0;}
