/* elephant_admin: Elephant (Loxodonta africana) African elephant management (v1.0)
 * Elephant housing, feeding, training, health, market
 * Features: body_len_cm, body_wt_kg, trunk_len_cm, tusk_cm, ear_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,trnk_ln,tsk_cm,ear_idx,age_yr,active;} elep_t;
typedef struct{int n_house,n_feed,n_train,n_hlth,n_mkt,t_ln,t_wt,t_tr,t_ts,t_er;} elep_state_t;
static elep_t ehs[N],efd[N-2],etr[N-4],ehl[N-6],emk[N-6]; static elep_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(elep_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int tc,int ei,int ay){if(*cnt>=mx)return -1;elep_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->trnk_ln=tl;x->tsk_cm=tc;x->ear_idx=ei;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ELEP] Elephant ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" tc=");pi(tc);ps(" ei=");pi(ei);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int elep_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_train=0;st.n_hlth=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tr=0;st.t_ts=0;st.t_er=0;for(int i=0;i<N;i++)ehs[i].active=0;for(int i=0;i<N-2;i++)efd[i].active=0;for(int i=0;i<N-4;i++)etr[i].active=0;for(int i=0;i<N-6;i++)ehl[i].active=0;for(int i=0;i<N-6;i++)emk[i].active=0;init=1;ps("[ELEP] Elephant initialized\n");return 0;}
int elep_housing(int lc,int bl,int bw,int tl,int tc,int ei,int ay){return add(ehs,&st.n_house,&st.t_ln,N,lc,bl,bw,tl,tc,ei,ay);}
int elep_feeding(int lc,int bl,int bw,int tl,int tc,int ei,int ay){return add(efd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,tc,ei,ay);}
int elep_training(int lc,int bl,int bw,int tl,int tc,int ei,int ay){return add(etr,&st.n_train,&st.t_tr,N-4,lc,bl,bw,tl,tc,ei,ay);}
int elep_health(int lc,int bl,int bw,int tl,int tc,int ei,int ay){return add(ehl,&st.n_hlth,&st.t_ts,N-6,lc,bl,bw,tl,tc,ei,ay);}
int elep_market(int lc,int bl,int bw,int tl,int tc,int ei,int ay){return add(emk,&st.n_mkt,&st.t_er,N-6,lc,bl,bw,tl,tc,ei,ay);}
void elep_report(void){ps("[ELEP] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nTrain: ");pi(st.n_train);ps(" Tr=");pi(st.t_tr);ps("\nHlth: ");pi(st.n_hlth);ps(" Ts=");pi(st.t_ts);ps("\nMkt: ");pi(st.n_mkt);ps(" Er=");pi(st.t_er);ps("\n");}
void elep_state(void){ps("[ELEP] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Train=");pi(st.n_train);ps(" Hlth=");pi(st.n_hlth);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Elephant Admin Demo ===\n\n");elep_init();
/* 1=sanctuary 2=reserve 3=zoo 4=camp 5=market */
ps("Elephant housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=500+(i*25),bw=400+(i*50),tl=150+(i*10),tc=50+(i*5),ei=(i%5)+1,ay=2+(i%8);elep_housing(lc,bl,bw,tl,tc,ei,ay);}
ps("\nElephant feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=520+(i*20),bw=420+(i*40),tl=160+(i*8),tc=55+(i*4),ei=(i%4)+1,ay=3+(i%6);elep_feeding(lc,bl,bw,tl,tc,ei,ay);}
ps("\nElephant training...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=540+(i*18),bw=440+(i*35),tl=170+(i*7),tc=60+(i*3),ei=(i%6)+1,ay=3+(i%5);elep_training(lc,bl,bw,tl,tc,ei,ay);}
ps("\nElephant health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=480+(i*30),bw=380+(i*60),tl=140+(i*12),tc=45+(i*6),ei=(i%7)+1,ay=4+(i%7);elep_health(lc,bl,bw,tl,tc,ei,ay);}
ps("\nElephant market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=560+(i*15),bw=460+(i*30),tl=180+(i*6),tc=65+(i*3),ei=(i%3)+4,ay=4+(i%4);elep_market(lc,bl,bw,tl,tc,ei,ay);}
ps("\n");elep_report();elep_state();ps("\n=== Demo Complete ===\n");return 0;}
