/* jackal_admin: Jackal (Canis aureus) golden jackal canid (v1.0)
 * Jackal habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, howl_vol, hunt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,hw_vl,hnt_idx,age_yr,active;} jackal_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_hw,t_hnt;} jackal_state_t;
static jackal_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static jackal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jackal_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int hv,int hi,int ay){if(*cnt>=mx)return -1;jackal_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->hw_vl=hv;x->hnt_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[JACK] Jackal ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" hv=");pi(hv);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int jackal_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_hw=0;st.t_hnt=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[JACK] Jackal initialized\n");return 0;}
int jackal_habitat(int lc,int bl,int bw,int ec,int hv,int hi,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,ec,hv,hi,ay);}
int jackal_feeding(int lc,int bl,int bw,int ec,int hv,int hi,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,hv,hi,ay);}
int jackal_breeding(int lc,int bl,int bw,int ec,int hv,int hi,int ay){return add(gbr,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,hv,hi,ay);}
int jackal_health(int lc,int bl,int bw,int ec,int hv,int hi,int ay){return add(ghl,&st.n_health,&st.t_hw,N-6,lc,bl,bw,ec,hv,hi,ay);}
int jackal_market(int lc,int bl,int bw,int ec,int hv,int hi,int ay){return add(gmk,&st.n_mkt,&st.t_hnt,N-6,lc,bl,bw,ec,hv,hi,ay);}
void jackal_report(void){ps("[JACK] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHlth: ");pi(st.n_health);ps(" Hw=");pi(st.t_hw);ps("\nMkt: ");pi(st.n_mkt);ps(" Hnt=");pi(st.t_hnt);ps("\n");}
void jackal_state(void){ps("[JACK] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Jackal Admin Demo ===\n\n");jackal_init();
/* 1=savanna 2=scrub 3=desert 4=forest 5=pack */
ps("Jackal habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*4),bw=10+(i*2),ec=8+(i%3),hv=40+(i*6),hi=(i%5)+1,ay=(i%5)+1;jackal_habitat(lc,bl,bw,ec,hv,hi,ay);}
ps("\nJackal feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=82+(i*3),bw=11+(i*2),ec=9+(i%3),hv=45+(i*5),hi=(i%4)+1,ay=(i%4)+1;jackal_feeding(lc,bl,bw,ec,hv,hi,ay);}
ps("\nJackal breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=78+(i*5),bw=9+(i*3),ec=7+(i%4),hv=35+(i*7),hi=(i%3)+2,ay=(i%3)+1;jackal_breeding(lc,bl,bw,ec,hv,hi,ay);}
ps("\nJackal health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=85+(i*3),bw=12+(i*2),ec=10+(i%2),hv=50+(i*4),hi=(i%5)+1,ay=(i%4)+1;jackal_health(lc,bl,bw,ec,hv,hi,ay);}
ps("\nJackal market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=88+(i*2),bw=13+(i*2),ec=11+(i%2),hv=55+(i*3),hi=(i%4)+1,ay=(i%3)+2;jackal_market(lc,bl,bw,ec,hv,hi,ay);}
ps("\n");jackal_report();jackal_state();ps("\n=== Demo Complete ===\n");return 0;}
