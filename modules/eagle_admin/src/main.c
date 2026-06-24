/* eagle_admin: Eagle (Aquila chrysaetos) golden eagle raptor bird (v1.0)
 * Eagle housing, feeding, training, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, talon_idx, dive_speed, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,tl_idx,dv_spd,age_yr,active;} eagl_t;
typedef struct{int n_house,n_feed,n_train,n_hlth,n_mkt,t_ln,t_wt,t_wg,t_tl,t_dv;} eagl_state_t;
static eagl_t ehs[N],efd[N-2],etr[N-4],ehl[N-6],emk[N-6]; static eagl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eagl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int ti,int ds,int ay){if(*cnt>=mx)return -1;eagl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->tl_idx=ti;x->dv_spd=ds;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[EAGL] Eagle ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" ti=");pi(ti);ps(" ds=");pi(ds);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int eagl_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_train=0;st.n_hlth=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wg=0;st.t_tl=0;st.t_dv=0;for(int i=0;i<N;i++)ehs[i].active=0;for(int i=0;i<N-2;i++)efd[i].active=0;for(int i=0;i<N-4;i++)etr[i].active=0;for(int i=0;i<N-6;i++)ehl[i].active=0;for(int i=0;i<N-6;i++)emk[i].active=0;init=1;ps("[EAGL] Eagle initialized\n");return 0;}
int eagl_housing(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(ehs,&st.n_house,&st.t_ln,N,lc,bl,bw,ws,ti,ds,ay);}
int eagl_feeding(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(efd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,ti,ds,ay);}
int eagl_training(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(etr,&st.n_train,&st.t_wg,N-4,lc,bl,bw,ws,ti,ds,ay);}
int eagl_health(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(ehl,&st.n_hlth,&st.t_tl,N-6,lc,bl,bw,ws,ti,ds,ay);}
int eagl_market(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(emk,&st.n_mkt,&st.t_dv,N-6,lc,bl,bw,ws,ti,ds,ay);}
void eagl_report(void){ps("[EAGL] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nTrain: ");pi(st.n_train);ps(" Wg=");pi(st.t_wg);ps("\nHlth: ");pi(st.n_hlth);ps(" Tl=");pi(st.t_tl);ps("\nMkt: ");pi(st.n_mkt);ps(" Dv=");pi(st.t_dv);ps("\n");}
void eagl_state(void){ps("[EAGL] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Train=");pi(st.n_train);ps(" Hlth=");pi(st.n_hlth);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Eagle Admin Demo ===\n\n");eagl_init();
/* 1=aerie 2=sanctuary 3=falconry 4=reserve 5=market */
ps("Eagle housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=75+(i*5),bw=4+(i*2),ws=180+(i*10),ti=(i%6)+1,ds=200+(i*15),ay=1+(i%10);eagl_housing(lc,bl,bw,ws,ti,ds,ay);}
ps("\nEagle feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=80+(i*4),bw=5+(i*2),ws=190+(i*8),ti=(i%5)+1,ds=210+(i*12),ay=2+(i%8);eagl_feeding(lc,bl,bw,ws,ti,ds,ay);}
ps("\nEagle training...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=85+(i*4),bw=5+(i%2),ws=200+(i*7),ti=(i%4)+1,ds=220+(i*10),ay=2+(i%6);eagl_training(lc,bl,bw,ws,ti,ds,ay);}
ps("\nEagle health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=70+(i*6),bw=3+(i*3),ws=175+(i*12),ti=(i%7)+1,ds=195+(i*18),ay=3+(i%7);eagl_health(lc,bl,bw,ws,ti,ds,ay);}
ps("\nEagle market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=90+(i*3),bw=6+(i*2),ws=210+(i*6),ti=(i%3)+4,ds=230+(i*8),ay=3+(i%4);eagl_market(lc,bl,bw,ws,ti,ds,ay);}
ps("\n");eagl_report();eagl_state();ps("\n=== Demo Complete ===\n");return 0;}
