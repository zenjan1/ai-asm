/* loon_admin: Loon (Gavia immer) common loon diving bird (v1.0)
 * Loon habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, dive_m, call_vol, plumage_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,dv_m,cl_vl,pl_idx,age_yr,active;} loon_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_dv,t_cl,t_pl;} loon_state_t;
static loon_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static loon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(loon_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int dm,int cv,int pi2,int ay){if(*cnt>=mx)return -1;loon_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->dv_m=dm;x->cl_vl=cv;x->pl_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[LOON] Loon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" dm=");pi(dm);ps(" cv=");pi(cv);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int loon_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_dv=0;st.t_cl=0;st.t_pl=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[LOON] Loon initialized\n");return 0;}
int loon_habitat(int lc,int bl,int bw,int dm,int cv,int pi2,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,dm,cv,pi2,ay);}
int loon_feeding(int lc,int bl,int bw,int dm,int cv,int pi2,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,dm,cv,pi2,ay);}
int loon_breeding(int lc,int bl,int bw,int dm,int cv,int pi2,int ay){return add(gbr,&st.n_breed,&st.t_dv,N-4,lc,bl,bw,dm,cv,pi2,ay);}
int loon_health(int lc,int bl,int bw,int dm,int cv,int pi2,int ay){return add(ghl,&st.n_health,&st.t_cl,N-6,lc,bl,bw,dm,cv,pi2,ay);}
int loon_market(int lc,int bl,int bw,int dm,int cv,int pi2,int ay){return add(gmk,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,dm,cv,pi2,ay);}
void loon_report(void){ps("[LOON] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Dv=");pi(st.t_dv);ps("\nHlth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void loon_state(void){ps("[LOON] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Loon Admin Demo ===\n\n");loon_init();
/* 1=lake 2=pond 3=river 4=island 5=shore */
ps("Loon habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=70+(i*4),bw=3+(i*2),dv=10+(i*3),cv=40+(i*6),pi2=(i%5)+1,ay=(i%6)+1;loon_habitat(lc,bl,bw,dv,cv,pi2,ay);}
ps("\nLoon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=72+(i*3),bw=3+(i*2),dv=12+(i*2),cv=45+(i*5),pi2=(i%4)+1,ay=(i%5)+1;loon_feeding(lc,bl,bw,dv,cv,pi2,ay);}
ps("\nLoon breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=68+(i*5),bw=4+(i*2),dv=8+(i*4),cv=35+(i*7),pi2=(i%3)+2,ay=(i%4)+1;loon_breeding(lc,bl,bw,dv,cv,pi2,ay);}
ps("\nLoon health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=75+(i*3),bw=4+(i*2),dv=15+(i*2),cv=50+(i*4),pi2=(i%5)+1,ay=(i%5)+1;loon_health(lc,bl,bw,dv,cv,pi2,ay);}
ps("\nLoon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=78+(i*2),bw=5+(i*2),dv=18+(i*2),cv=55+(i*3),pi2=(i%4)+1,ay=(i%3)+2;loon_market(lc,bl,bw,dv,cv,pi2,ay);}
ps("\n");loon_report();loon_state();ps("\n=== Demo Complete ===\n");return 0;}
