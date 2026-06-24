/* cormorant_admin: Cormorant (Phalacrocorax carbo) fishing bird husbandry (v1.0)
 * Cormorant housing, feeding, training, fishing, market
 * Features: body_len_cm, body_wt_g, wing_span_cm, dive_depth_m, fish_caught, feather_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,dive_dp,fish_ct,fthr_idx,active;} crmt_t;
typedef struct{int n_house,n_feed,n_train,n_fish,n_mkt,t_ln,t_wt,t_wing,t_dive,t_fish;} crmt_state_t;
static crmt_t crmth[N],crmtf[N-2],crmtr[N-4],crmtfs[N-6],crmtm[N-6]; static crmt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crmt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int dd,int fc,int fi){if(*cnt>=mx)return -1;crmt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->dive_dp=dd;x->fish_ct=fc;x->fthr_idx=fi;x->active=1;*sum+=bl;(*cnt)++;ps("[CRMT] Cormorant ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" dd=");pi(dd);ps(" fc=");pi(fc);ps(" fi=");pi(fi);ps("\n");return *cnt-1;}
int crmt_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_train=0;st.n_fish=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_dive=0;st.t_fish=0;for(int i=0;i<N;i++)crmth[i].active=0;for(int i=0;i<N-2;i++)crmtf[i].active=0;for(int i=0;i<N-4;i++)crmtr[i].active=0;for(int i=0;i<N-6;i++)crmtfs[i].active=0;for(int i=0;i<N-6;i++)crmtm[i].active=0;init=1;ps("[CRMT] Cormorant initialized\n");return 0;}
int crmt_housing(int lc,int bl,int bw,int ws,int dd,int fc,int fi){return add(crmth,&st.n_house,&st.t_ln,N,lc,bl,bw,ws,dd,fc,fi);}
int crmt_feeding(int lc,int bl,int bw,int ws,int dd,int fc,int fi){return add(crmtf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,dd,fc,fi);}
int crmt_training(int lc,int bl,int bw,int ws,int dd,int fc,int fi){return add(crmtr,&st.n_train,&st.t_wing,N-4,lc,bl,bw,ws,dd,fc,fi);}
int crmt_fishing(int lc,int bl,int bw,int ws,int dd,int fc,int fi){return add(crmtfs,&st.n_fish,&st.t_dive,N-6,lc,bl,bw,ws,dd,fc,fi);}
int crmt_market(int lc,int bl,int bw,int ws,int dd,int fc,int fi){return add(crmtm,&st.n_mkt,&st.t_fish,N-6,lc,bl,bw,ws,dd,fc,fi);}
void crmt_report(void){ps("[CRMT] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nTrain: ");pi(st.n_train);ps(" Wing=");pi(st.t_wing);ps("\nFish: ");pi(st.n_fish);ps(" Dive=");pi(st.t_dive);ps("\nMkt: ");pi(st.n_mkt);ps(" Fish=");pi(st.t_fish);ps("\n");}
void crmt_state(void){ps("[CRMT] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Train=");pi(st.n_train);ps(" Fish=");pi(st.n_fish);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cormorant Admin Demo ===\n\n");crmt_init();
/* 1=lakeside 2=river 3=aviary 4=fishing_boat 5=market */
ps("Cormorant housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=70+(i*8),bw=1500+(i*200),ws=100+(i*10),dd=3+(i*2),fc=5+(i*3),fi=(i%6)+1;crmt_housing(lc,bl,bw,ws,dd,fc,fi);}
ps("\nCormorant feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=75+(i*7),bw=1600+(i*180),ws=105+(i*8),dd=4+(i*2),fc=6+(i*2),fi=(i%5)+1;crmt_feeding(lc,bl,bw,ws,dd,fc,fi);}
ps("\nCormorant training...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=80+(i*6),bw=1700+(i*150),ws=110+(i*7),dd=5+(i*2),fc=7+(i*2),fi=(i%4)+1;crmt_training(lc,bl,bw,ws,dd,fc,fi);}
ps("\nCormorant fishing...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=65+(i*10),bw=1400+(i*250),ws=95+(i*12),dd=2+(i*3),fc=4+(i*4),fi=(i%7)+1;crmt_fishing(lc,bl,bw,ws,dd,fc,fi);}
ps("\nCormorant market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=85+(i*5),bw=1800+(i*120),ws=115+(i*6),dd=6+(i*2),fc=8+(i*2),fi=(i%3)+4;crmt_market(lc,bl,bw,ws,dd,fc,fi);}
ps("\n");crmt_report();crmt_state();ps("\n=== Demo Complete ===\n");return 0;}
