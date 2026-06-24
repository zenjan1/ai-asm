/* earwig_admin: Earwig (Forficula auricularia) earwig insect management (v1.0)
 * Earwig housing, feeding, breeding, molt, market
 * Features: body_len_mm, body_wt_mg, pincer_mm, wing_idx, molt_stage, age_day
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,pncr_mm,wg_idx,mlt_st,age_dy,active;} earw_t;
typedef struct{int n_house,n_feed,n_breed,n_molt,n_mkt,t_ln,t_wt,t_pn,t_wg,t_ml;} earw_state_t;
static earw_t ehs[N],efd[N-2],ebr[N-4],eml[N-6],emk[N-6]; static earw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(earw_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int pm,int wi,int ms,int ad){if(*cnt>=mx)return -1;earw_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->pncr_mm=pm;x->wg_idx=wi;x->mlt_st=ms;x->age_dy=ad;x->active=1;*sum+=bl;(*cnt)++;ps("[EARW] Earwig ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" pm=");pi(pm);ps(" wi=");pi(wi);ps(" ms=");pi(ms);ps(" ad=");pi(ad);ps("\n");return *cnt-1;}
int earw_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_molt=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_pn=0;st.t_wg=0;st.t_ml=0;for(int i=0;i<N;i++)ehs[i].active=0;for(int i=0;i<N-2;i++)efd[i].active=0;for(int i=0;i<N-4;i++)ebr[i].active=0;for(int i=0;i<N-6;i++)eml[i].active=0;for(int i=0;i<N-6;i++)emk[i].active=0;init=1;ps("[EARW] Earwig initialized\n");return 0;}
int earw_housing(int lc,int bl,int bw,int pm,int wi,int ms,int ad){return add(ehs,&st.n_house,&st.t_ln,N,lc,bl,bw,pm,wi,ms,ad);}
int earw_feeding(int lc,int bl,int bw,int pm,int wi,int ms,int ad){return add(efd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,pm,wi,ms,ad);}
int earw_breeding(int lc,int bl,int bw,int pm,int wi,int ms,int ad){return add(ebr,&st.n_breed,&st.t_pn,N-4,lc,bl,bw,pm,wi,ms,ad);}
int earw_molt(int lc,int bl,int bw,int pm,int wi,int ms,int ad){return add(eml,&st.n_molt,&st.t_wg,N-6,lc,bl,bw,pm,wi,ms,ad);}
int earw_market(int lc,int bl,int bw,int pm,int wi,int ms,int ad){return add(emk,&st.n_mkt,&st.t_ml,N-6,lc,bl,bw,pm,wi,ms,ad);}
void earw_report(void){ps("[EARW] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Pn=");pi(st.t_pn);ps("\nMolt: ");pi(st.n_molt);ps(" Wg=");pi(st.t_wg);ps("\nMkt: ");pi(st.n_mkt);ps(" Ml=");pi(st.t_ml);ps("\n");}
void earw_state(void){ps("[EARW] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Molt=");pi(st.n_molt);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Earwig Admin Demo ===\n\n");earw_init();
/* 1=container 2=garden 3=under-rock 4=greenhouse 5=market */
ps("Earwig housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=15+(i*2),bw=30+(i*5),pm=3+(i%3),wi=(i%4)+1,ms=(i%5)+1,ad=3+(i*4);earw_housing(lc,bl,bw,pm,wi,ms,ad);}
ps("\nEarwig feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=17+(i%2),bw=35+(i*4),pm=4+(i%2),wi=(i%3)+1,ms=(i%4)+1,ad=5+(i*3);earw_feeding(lc,bl,bw,pm,wi,ms,ad);}
ps("\nEarwig breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=16+(i*2),bw=32+(i*5),pm=3+(i%3),wi=(i%5)+1,ms=(i%3)+1,ad=7+(i*3);earw_breeding(lc,bl,bw,pm,wi,ms,ad);}
ps("\nEarwig molt...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i*3),bw=28+(i*6),pm=2+(i%4),wi=(i%6)+1,ms=(i%5)+1,ad=2+(i*5);earw_molt(lc,bl,bw,pm,wi,ms,ad);}
ps("\nEarwig market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=19+(i%2),bw=40+(i*3),pm=5+(i%2),wi=(i%3)+3,ms=(i%4)+2,ad=10+(i*3);earw_market(lc,bl,bw,pm,wi,ms,ad);}
ps("\n");earw_report();earw_state();ps("\n=== Demo Complete ===\n");return 0;}
