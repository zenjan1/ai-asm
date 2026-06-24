/* dingo_admin: Dingo (Canis dingo) wild canid husbandry management (v1.0)
 * Dingo housing, feeding, training, grooming, market
 * Features: body_len_cm, body_wt_kg, ear_ht_cm, coat_idx, hunt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_ht,coat_idx,hnt_idx,age_yr,active;} ding_t;
typedef struct{int n_house,n_feed,n_train,n_groom,n_mkt,t_ln,t_wt,t_ear,t_co,t_hn;} ding_state_t;
static ding_t dhs[N],dfd[N-2],dtr[N-4],dgr[N-6],dmk[N-6]; static ding_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ding_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int eh,int ci,int hi,int ay){if(*cnt>=mx)return -1;ding_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_ht=eh;x->coat_idx=ci;x->hnt_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DING] Dingo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" eh=");pi(eh);ps(" ci=");pi(ci);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ding_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_train=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_co=0;st.t_hn=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dtr[i].active=0;for(int i=0;i<N-6;i++)dgr[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DING] Dingo initialized\n");return 0;}
int ding_housing(int lc,int bl,int bw,int eh,int ci,int hi,int ay){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,eh,ci,hi,ay);}
int ding_feeding(int lc,int bl,int bw,int eh,int ci,int hi,int ay){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,eh,ci,hi,ay);}
int ding_training(int lc,int bl,int bw,int eh,int ci,int hi,int ay){return add(dtr,&st.n_train,&st.t_ear,N-4,lc,bl,bw,eh,ci,hi,ay);}
int ding_grooming(int lc,int bl,int bw,int eh,int ci,int hi,int ay){return add(dgr,&st.n_groom,&st.t_co,N-6,lc,bl,bw,eh,ci,hi,ay);}
int ding_market(int lc,int bl,int bw,int eh,int ci,int hi,int ay){return add(dmk,&st.n_mkt,&st.t_hn,N-6,lc,bl,bw,eh,ci,hi,ay);}
void ding_report(void){ps("[DING] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nTrain: ");pi(st.n_train);ps(" Ear=");pi(st.t_ear);ps("\nGroom: ");pi(st.n_groom);ps(" Co=");pi(st.t_co);ps("\nMkt: ");pi(st.n_mkt);ps(" Hn=");pi(st.t_hn);ps("\n");}
void ding_state(void){ps("[DING] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Train=");pi(st.n_train);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dingo Admin Demo ===\n\n");ding_init();
/* 1=enclosure 2=sanctuary 3=reserve 4=wild 5=market */
ps("Dingo housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*6),bw=18+(i*3),eh=8+(i*2),ci=(i%4)+1,hi=(i%8)+1,ay=1+(i%8);ding_housing(lc,bl,bw,eh,ci,hi,ay);}
ps("\nDingo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=95+(i*5),bw=20+(i*3),eh=9+(i*2),ci=(i%3)+1,hi=(i%7)+1,ay=2+(i%6);ding_feeding(lc,bl,bw,eh,ci,hi,ay);}
ps("\nDingo training...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=100+(i*4),bw=22+(i*2),eh=10+(i*2),ci=(i%5)+1,hi=(i%6)+1,ay=2+(i%5);ding_training(lc,bl,bw,eh,ci,hi,ay);}
ps("\nDingo grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=85+(i*7),bw=16+(i*4),eh=7+(i*3),ci=(i%6)+1,hi=(i%9)+1,ay=3+(i%7);ding_grooming(lc,bl,bw,eh,ci,hi,ay);}
ps("\nDingo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=105+(i*4),bw=24+(i*2),eh=11+(i*2),ci=(i%3)+3,hi=(i%5)+4,ay=3+(i%4);ding_market(lc,bl,bw,eh,ci,hi,ay);}
ps("\n");ding_report();ding_state();ps("\n=== Demo Complete ===\n");return 0;}
