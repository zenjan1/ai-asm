/* ibex_admin: Ibex (Capra ibex) Alpine ibex mountain goat (v1.0)
 * Ibex habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, climb_idx, hoof_grip, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,cl_idx,hf_gr,age_yr,active;} ibex_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_cl,t_hf;} ibex_state_t;
static ibex_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static ibex_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ibex_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ci,int hg,int ay){if(*cnt>=mx)return -1;ibex_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->cl_idx=ci;x->hf_gr=hg;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[IBEX] Ibex ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ci=");pi(ci);ps(" hg=");pi(hg);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ibex_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_cl=0;st.t_hf=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[IBEX] Ibex initialized\n");return 0;}
int ibex_habitat(int lc,int bl,int bw,int hc,int ci,int hg,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,hc,ci,hg,ay);}
int ibex_feeding(int lc,int bl,int bw,int hc,int ci,int hg,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ci,hg,ay);}
int ibex_breeding(int lc,int bl,int bw,int hc,int ci,int hg,int ay){return add(gbr,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,ci,hg,ay);}
int ibex_health(int lc,int bl,int bw,int hc,int ci,int hg,int ay){return add(ghl,&st.n_health,&st.t_cl,N-6,lc,bl,bw,hc,ci,hg,ay);}
int ibex_market(int lc,int bl,int bw,int hc,int ci,int hg,int ay){return add(gmk,&st.n_mkt,&st.t_hf,N-6,lc,bl,bw,hc,ci,hg,ay);}
void ibex_report(void){ps("[IBEX] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHlth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Hf=");pi(st.t_hf);ps("\n");}
void ibex_state(void){ps("[IBEX] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ibex Admin Demo ===\n\n");ibex_init();
/* 1=cliff 2=alpine 3=ridge 4=meadow 5=peak */
ps("Ibex habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=130+(i*5),bw=70+(i*5),hc=50+(i*4),ci=(i%5)+1,hg=(i%4)+1,ay=(i%6)+1;ibex_habitat(lc,bl,bw,hc,ci,hg,ay);}
ps("\nIbex feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=135+(i*4),bw=72+(i*4),hc=52+(i*3),ci=(i%4)+1,hg=(i%3)+1,ay=(i%5)+1;ibex_feeding(lc,bl,bw,hc,ci,hg,ay);}
ps("\nIbex breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=125+(i*6),bw=68+(i*6),hc=48+(i*5),ci=(i%3)+2,hg=(i%3)+1,ay=(i%4)+2;ibex_breeding(lc,bl,bw,hc,ci,hg,ay);}
ps("\nIbex health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=140+(i*4),bw=75+(i*3),hc=55+(i*3),ci=(i%5)+1,hg=(i%4)+1,ay=(i%5)+1;ibex_health(lc,bl,bw,hc,ci,hg,ay);}
ps("\nIbex market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=145+(i*3),bw=78+(i*3),hc=58+(i*2),ci=(i%4)+1,hg=(i%3)+1,ay=(i%3)+2;ibex_market(lc,bl,bw,hc,ci,hg,ay);}
ps("\n");ibex_report();ibex_state();ps("\n=== Demo Complete ===\n");return 0;}
