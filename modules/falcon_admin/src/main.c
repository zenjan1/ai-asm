/* falcon_admin: Falcon (Falco peregrinus) peregrine falcon raptor management (v1.0)
 * Falcon housing, feeding, training, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, talon_idx, dive_speed, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,tl_idx,dv_spd,age_yr,active;} falc_t;
typedef struct{int n_house,n_feed,n_train,n_hlth,n_mkt,t_ln,t_wt,t_wg,t_tl,t_dv;} falc_state_t;
static falc_t fhs[N],ffd[N-2],ftr[N-4],fhl[N-6],fmk[N-6]; static falc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(falc_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int ti,int ds,int ay){if(*cnt>=mx)return -1;falc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->tl_idx=ti;x->dv_spd=ds;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FALC] Falcon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" ti=");pi(ti);ps(" ds=");pi(ds);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int falc_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_train=0;st.n_hlth=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wg=0;st.t_tl=0;st.t_dv=0;for(int i=0;i<N;i++)fhs[i].active=0;for(int i=0;i<N-2;i++)ffd[i].active=0;for(int i=0;i<N-4;i++)ftr[i].active=0;for(int i=0;i<N-6;i++)fhl[i].active=0;for(int i=0;i<N-6;i++)fmk[i].active=0;init=1;ps("[FALC] Falcon initialized\n");return 0;}
int falc_housing(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(fhs,&st.n_house,&st.t_ln,N,lc,bl,bw,ws,ti,ds,ay);}
int falc_feeding(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(ffd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,ti,ds,ay);}
int falc_training(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(ftr,&st.n_train,&st.t_wg,N-4,lc,bl,bw,ws,ti,ds,ay);}
int falc_health(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(fhl,&st.n_hlth,&st.t_tl,N-6,lc,bl,bw,ws,ti,ds,ay);}
int falc_market(int lc,int bl,int bw,int ws,int ti,int ds,int ay){return add(fmk,&st.n_mkt,&st.t_dv,N-6,lc,bl,bw,ws,ti,ds,ay);}
void falc_report(void){ps("[FALC] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nTrain: ");pi(st.n_train);ps(" Wg=");pi(st.t_wg);ps("\nHlth: ");pi(st.n_hlth);ps(" Tl=");pi(st.t_tl);ps("\nMkt: ");pi(st.n_mkt);ps(" Dv=");pi(st.t_dv);ps("\n");}
void falc_state(void){ps("[FALC] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Train=");pi(st.n_train);ps(" Hlth=");pi(st.n_hlth);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Falcon Admin Demo ===\n\n");falc_init();
/* 1=mews 2=aerie 3=falconry 4=reserve 5=market */
ps("Falcon housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=1+(i%3),ws=90+(i*8),ti=(i%6)+1,ds=300+(i*20),ay=1+(i%8);falc_housing(lc,bl,bw,ws,ti,ds,ay);}
ps("\nFalcon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*3),bw=1+(i%2),ws=95+(i*7),ti=(i%5)+1,ds=310+(i*18),ay=2+(i%6);falc_feeding(lc,bl,bw,ws,ti,ds,ay);}
ps("\nFalcon training...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*2),bw=2+(i%2),ws=100+(i*6),ti=(i%4)+1,ds=320+(i*15),ay=2+(i%5);falc_training(lc,bl,bw,ws,ti,ds,ay);}
ps("\nFalcon health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=1+(i%3),ws=85+(i*9),ti=(i%7)+1,ds=290+(i*25),ay=3+(i%7);falc_health(lc,bl,bw,ws,ti,ds,ay);}
ps("\nFalcon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=48+(i*2),bw=2+(i%2),ws=105+(i*5),ti=(i%3)+4,ds=340+(i*12),ay=3+(i%4);falc_market(lc,bl,bw,ws,ti,ds,ay);}
ps("\n");falc_report();falc_state();ps("\n=== Demo Complete ===\n");return 0;}
