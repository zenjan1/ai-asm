/* hare_admin: Hare (Lepus europaeus) European hare lagomorph (v1.0)
 * Hare habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, leap_m, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,lp_m,f_idx,age_yr,active;} hare_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_lp,t_f;} hare_state_t;
static hare_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static hare_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hare_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int lm,int fi,int ay){if(*cnt>=mx)return -1;hare_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->lp_m=lm;x->f_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HARE] Hare ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" lm=");pi(lm);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hare_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_lp=0;st.t_f=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HARE] Hare initialized\n");return 0;}
int hare_habitat(int lc,int bl,int bw,int ec,int lm,int fi,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,ec,lm,fi,ay);}
int hare_feeding(int lc,int bl,int bw,int ec,int lm,int fi,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,lm,fi,ay);}
int hare_breeding(int lc,int bl,int bw,int ec,int lm,int fi,int ay){return add(gbr,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,lm,fi,ay);}
int hare_health(int lc,int bl,int bw,int ec,int lm,int fi,int ay){return add(ghl,&st.n_health,&st.t_lp,N-6,lc,bl,bw,ec,lm,fi,ay);}
int hare_market(int lc,int bl,int bw,int ec,int lm,int fi,int ay){return add(gmk,&st.n_mkt,&st.t_f,N-6,lc,bl,bw,ec,lm,fi,ay);}
void hare_report(void){ps("[HARE] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHlth: ");pi(st.n_health);ps(" Lp=");pi(st.t_lp);ps("\nMkt: ");pi(st.n_mkt);ps(" F=");pi(st.t_f);ps("\n");}
void hare_state(void){ps("[HARE] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hare Admin Demo ===\n\n");hare_init();
/* 1=field 2=meadow 3=hedgerow 4=forest 5=warren */
ps("Hare habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*4),bw=3+(i*2),ec=8+(i%4),lm=2+(i%3),fi=(i%5)+1,ay=(i%5)+1;hare_habitat(lc,bl,bw,ec,lm,fi,ay);}
ps("\nHare feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=52+(i*3),bw=3+(i*2),ec=9+(i%3),lm=3+(i%2),fi=(i%4)+1,ay=(i%4)+1;hare_feeding(lc,bl,bw,ec,lm,fi,ay);}
ps("\nHare breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=48+(i*5),bw=4+(i*2),ec=7+(i%5),lm=2+(i%3),fi=(i%3)+2,ay=(i%3)+1;hare_breeding(lc,bl,bw,ec,lm,fi,ay);}
ps("\nHare health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=55+(i*3),bw=5+(i*2),ec=10+(i%3),lm=4+(i%2),fi=(i%5)+1,ay=(i%4)+1;hare_health(lc,bl,bw,ec,lm,fi,ay);}
ps("\nHare market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=58+(i*2),bw=5+(i*2),ec=11+(i%2),lm=5+(i%2),fi=(i%4)+1,ay=(i%3)+1;hare_market(lc,bl,bw,ec,lm,fi,ay);}
ps("\n");hare_report();hare_state();ps("\n=== Demo Complete ===\n");return 0;}
