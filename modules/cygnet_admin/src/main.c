/* cygnet_admin: Cygnet (Cygnus olor) young swan waterfowl husbandry (v1.0)
 * Cygnet housing, feeding, swimming, grooming, market
 * Features: body_len_cm, body_wt_g, neck_len_cm, feather_idx, swim_speed, age_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,neck_ln,fthr_idx,swm_spd,age_wk,active;} cygn_t;
typedef struct{int n_house,n_feed,n_swim,n_groom,n_mkt,t_ln,t_wt,t_neck,t_fthr,t_swim;} cygn_state_t;
static cygn_t cygnhs[N],cygnfd[N-2],cygnsw[N-4],cygngm[N-6],cygnmt[N-6]; static cygn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cygn_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nl,int fi,int ss,int aw){if(*cnt>=mx)return -1;cygn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->neck_ln=nl;x->fthr_idx=fi;x->swm_spd=ss;x->age_wk=aw;x->active=1;*sum+=bl;(*cnt)++;ps("[CYGN] Cygnet ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nl=");pi(nl);ps(" fi=");pi(fi);ps(" ss=");pi(ss);ps(" aw=");pi(aw);ps("\n");return *cnt-1;}
int cygn_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_swim=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_neck=0;st.t_fthr=0;st.t_swim=0;for(int i=0;i<N;i++)cygnhs[i].active=0;for(int i=0;i<N-2;i++)cygnfd[i].active=0;for(int i=0;i<N-4;i++)cygnsw[i].active=0;for(int i=0;i<N-6;i++)cygngm[i].active=0;for(int i=0;i<N-6;i++)cygnmt[i].active=0;init=1;ps("[CYGN] Cygnet initialized\n");return 0;}
int cygn_housing(int lc,int bl,int bw,int nl,int fi,int ss,int aw){return add(cygnhs,&st.n_house,&st.t_ln,N,lc,bl,bw,nl,fi,ss,aw);}
int cygn_feeding(int lc,int bl,int bw,int nl,int fi,int ss,int aw){return add(cygnfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nl,fi,ss,aw);}
int cygn_swimming(int lc,int bl,int bw,int nl,int fi,int ss,int aw){return add(cygnsw,&st.n_swim,&st.t_neck,N-4,lc,bl,bw,nl,fi,ss,aw);}
int cygn_grooming(int lc,int bl,int bw,int nl,int fi,int ss,int aw){return add(cygngm,&st.n_groom,&st.t_fthr,N-6,lc,bl,bw,nl,fi,ss,aw);}
int cygn_market(int lc,int bl,int bw,int nl,int fi,int ss,int aw){return add(cygnmt,&st.n_mkt,&st.t_swim,N-6,lc,bl,bw,nl,fi,ss,aw);}
void cygn_report(void){ps("[CYGN] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nSwim: ");pi(st.n_swim);ps(" Neck=");pi(st.t_neck);ps("\nGroom: ");pi(st.n_groom);ps(" Fthr=");pi(st.t_fthr);ps("\nMkt: ");pi(st.n_mkt);ps(" Swim=");pi(st.t_swim);ps("\n");}
void cygn_state(void){ps("[CYGN] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Swim=");pi(st.n_swim);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cygnet Admin Demo ===\n\n");cygn_init();
/* 1=pond 2=lake 3=aviary 4=sanctuary 5=market */
ps("Cygnet housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*5),bw=1000+(i*150),nl=30+(i*4),fi=(i%6)+1,ss=3+(i*2),aw=4+(i%8);cygn_housing(lc,bl,bw,nl,fi,ss,aw);}
ps("\nCygnet feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=55+(i*4),bw=1100+(i*130),nl=32+(i*3),fi=(i%5)+1,ss=4+(i*2),aw=5+(i%7);cygn_feeding(lc,bl,bw,nl,fi,ss,aw);}
ps("\nCygnet swimming...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=60+(i*4),bw=1200+(i*110),nl=34+(i*3),fi=(i%4)+1,ss=5+(i*2),aw=6+(i%6);cygn_swimming(lc,bl,bw,nl,fi,ss,aw);}
ps("\nCygnet grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=45+(i*6),bw=900+(i*180),nl=28+(i*5),fi=(i%7)+1,ss=2+(i*3),aw=3+(i%10);cygn_grooming(lc,bl,bw,nl,fi,ss,aw);}
ps("\nCygnet market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=65+(i*3),bw=1300+(i*90),nl=36+(i*3),fi=(i%3)+4,ss=6+(i*2),aw=7+(i%5);cygn_market(lc,bl,bw,nl,fi,ss,aw);}
ps("\n");cygn_report();cygn_state();ps("\n=== Demo Complete ===\n");return 0;}
