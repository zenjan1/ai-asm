/* dugong_admin: Dugong (Dugong dugon) sea cow marine mammal management (v1.0)
 * Dugong habitat, feeding, health, breeding, market
 * Features: body_len_cm, body_wt_kg, flipper_cm, tusk_idx, dive_depth, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,flip_cm,tsk_idx,dv_dp,age_yr,active;} dugn_t;
typedef struct{int n_hab,n_feed,n_hlth,n_breed,n_mkt,t_ln,t_wt,t_fl,t_ts,t_dv;} dugn_state_t;
static dugn_t dhab[N],dfd[N-2],dhl[N-4],dbr[N-6],dmk[N-6]; static dugn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dugn_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fm,int ti,int dd,int ay){if(*cnt>=mx)return -1;dugn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->flip_cm=fm;x->tsk_idx=ti;x->dv_dp=dd;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DUGN] Dugong ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fm=");pi(fm);ps(" ti=");pi(ti);ps(" dd=");pi(dd);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dugn_init(void){if(init)return -1;st.n_hab=0;st.n_feed=0;st.n_hlth=0;st.n_breed=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fl=0;st.t_ts=0;st.t_dv=0;for(int i=0;i<N;i++)dhab[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dhl[i].active=0;for(int i=0;i<N-6;i++)dbr[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DUGN] Dugong initialized\n");return 0;}
int dugn_habitat(int lc,int bl,int bw,int fm,int ti,int dd,int ay){return add(dhab,&st.n_hab,&st.t_ln,N,lc,bl,bw,fm,ti,dd,ay);}
int dugn_feeding(int lc,int bl,int bw,int fm,int ti,int dd,int ay){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fm,ti,dd,ay);}
int dugn_health(int lc,int bl,int bw,int fm,int ti,int dd,int ay){return add(dhl,&st.n_hlth,&st.t_fl,N-4,lc,bl,bw,fm,ti,dd,ay);}
int dugn_breeding(int lc,int bl,int bw,int fm,int ti,int dd,int ay){return add(dbr,&st.n_breed,&st.t_ts,N-6,lc,bl,bw,fm,ti,dd,ay);}
int dugn_market(int lc,int bl,int bw,int fm,int ti,int dd,int ay){return add(dmk,&st.n_mkt,&st.t_dv,N-6,lc,bl,bw,fm,ti,dd,ay);}
void dugn_report(void){ps("[DUGN] Hab: ");pi(st.n_hab);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nHlth: ");pi(st.n_hlth);ps(" Fl=");pi(st.t_fl);ps("\nBreed: ");pi(st.n_breed);ps(" Ts=");pi(st.t_ts);ps("\nMkt: ");pi(st.n_mkt);ps(" Dv=");pi(st.t_dv);ps("\n");}
void dugn_state(void){ps("[DUGN] Hab=");pi(st.n_hab);ps(" Feed=");pi(st.n_feed);ps(" Hlth=");pi(st.n_hlth);ps(" Breed=");pi(st.n_breed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dugong Admin Demo ===\n\n");dugn_init();
/* 1=bay 2=sanctuary 3=reef 4=seagrass 5=market */
ps("Dugong habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=250+(i*12),bw=300+(i*25),fm=40+(i*5),ti=(i%4)+1,dd=10+(i*5),ay=3+(i%10);dugn_habitat(lc,bl,bw,fm,ti,dd,ay);}
ps("\nDugong feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=260+(i*10),bw=320+(i*20),fm=45+(i*4),ti=(i%3)+1,dd=12+(i*4),ay=4+(i%8);dugn_feeding(lc,bl,bw,fm,ti,dd,ay);}
ps("\nDugong health...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=270+(i*8),bw=340+(i*15),fm=50+(i*3),ti=(i%5)+1,dd=15+(i*3),ay=3+(i%6);dugn_health(lc,bl,bw,fm,ti,dd,ay);}
ps("\nDugong breeding...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=245+(i*14),bw=280+(i*30),fm=38+(i*6),ti=(i%6)+1,dd=8+(i*6),ay=5+(i%7);dugn_breeding(lc,bl,bw,fm,ti,dd,ay);}
ps("\nDugong market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=280+(i*7),bw=360+(i*12),fm=55+(i*3),ti=(i%3)+3,dd=18+(i*4),ay=4+(i%5);dugn_market(lc,bl,bw,fm,ti,dd,ay);}
ps("\n");dugn_report();dugn_state();ps("\n=== Demo Complete ===\n");return 0;}
