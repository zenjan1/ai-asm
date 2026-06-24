/* dalmatian_admin: Dalmatian (Canis familiaris) spotted breed dog (v1.0)
 * Dalmatian housing, feeding, training, grooming, market
 * Features: body_len_cm, body_wt_kg, spot_count, coat_idx, run_speed, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,spot_ct,coat_idx,run_spd,age_yr,active;} dalm_t;
typedef struct{int n_house,n_feed,n_train,n_groom,n_mkt,t_ln,t_wt,t_sp,t_ct,t_ag;} dalm_state_t;
static dalm_t dhs[N],dfd[N-2],dtr[N-4],dgr[N-6],dmk[N-6]; static dalm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dalm_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int ci,int rs,int ay){if(*cnt>=mx)return -1;dalm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->spot_ct=sc;x->coat_idx=ci;x->run_spd=rs;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DALM] Dalmatian ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" ci=");pi(ci);ps(" rs=");pi(rs);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dalm_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_train=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_sp=0;st.t_ct=0;st.t_ag=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dtr[i].active=0;for(int i=0;i<N-6;i++)dgr[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DALM] Dalmatian initialized\n");return 0;}
int dalm_housing(int lc,int bl,int bw,int sc,int ci,int rs,int ay){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,sc,ci,rs,ay);}
int dalm_feeding(int lc,int bl,int bw,int sc,int ci,int rs,int ay){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,ci,rs,ay);}
int dalm_training(int lc,int bl,int bw,int sc,int ci,int rs,int ay){return add(dtr,&st.n_train,&st.t_sp,N-4,lc,bl,bw,sc,ci,rs,ay);}
int dalm_grooming(int lc,int bl,int bw,int sc,int ci,int rs,int ay){return add(dgr,&st.n_groom,&st.t_ct,N-6,lc,bl,bw,sc,ci,rs,ay);}
int dalm_market(int lc,int bl,int bw,int sc,int ci,int rs,int ay){return add(dmk,&st.n_mkt,&st.t_ag,N-6,lc,bl,bw,sc,ci,rs,ay);}
void dalm_report(void){ps("[DALM] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nTrain: ");pi(st.n_train);ps(" Sp=");pi(st.t_sp);ps("\nGroom: ");pi(st.n_groom);ps(" Ct=");pi(st.t_ct);ps("\nMkt: ");pi(st.n_mkt);ps(" Ag=");pi(st.t_ag);ps("\n");}
void dalm_state(void){ps("[DALM] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Train=");pi(st.n_train);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dalmatian Admin Demo ===\n\n");dalm_init();
/* 1=kennel 2=home 3=show 4=farm 5=market */
ps("Dalmatian housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*4),bw=20+(i*3),sc=30+(i*5),ci=(i%4)+1,rs=40+(i*3),ay=1+(i%10);dalm_housing(lc,bl,bw,sc,ci,rs,ay);}
ps("\nDalmatian feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=62+(i*3),bw=22+(i*3),sc=35+(i*4),ci=(i%3)+1,rs=42+(i*2),ay=2+(i%8);dalm_feeding(lc,bl,bw,sc,ci,rs,ay);}
ps("\nDalmatian training...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=65+(i*3),bw=24+(i*2),sc=40+(i*3),ci=(i%4)+1,rs=45+(i*2),ay=1+(i%6);dalm_training(lc,bl,bw,sc,ci,rs,ay);}
ps("\nDalmatian grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=58+(i*5),bw=18+(i*4),sc=25+(i*6),ci=(i%5)+1,rs=38+(i*4),ay=3+(i%7);dalm_grooming(lc,bl,bw,sc,ci,rs,ay);}
ps("\nDalmatian market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=68+(i*3),bw=26+(i*2),sc=45+(i*3),ci=(i%3)+3,rs=48+(i*2),ay=2+(i%5);dalm_market(lc,bl,bw,sc,ci,rs,ay);}
ps("\n");dalm_report();dalm_state();ps("\n=== Demo Complete ===\n");return 0;}
