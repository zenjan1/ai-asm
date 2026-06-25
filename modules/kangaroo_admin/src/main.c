/* kangaroo_admin: Kangaroo (Macropus rufus) red kangaroo marsupial (v1.0)
 * Kangaroo habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, leap_m, tail_cm, pouch_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,lp_m,tail_cm,p_idx,age_yr,active;} kangaroo_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_lp,t_tail,t_p;} kangaroo_state_t;
static kangaroo_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static kangaroo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kangaroo_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lm,int tc,int pi2,int ay){if(*cnt>=mx)return -1;kangaroo_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->lp_m=lm;x->tail_cm=tc;x->p_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KANG] Kangaroo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lm=");pi(lm);ps(" tc=");pi(tc);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int kangaroo_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_lp=0;st.t_tail=0;st.t_p=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[KANG] Kangaroo initialized\n");return 0;}
int kangaroo_habitat(int lc,int bl,int bw,int lm,int tc,int pi2,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,lm,tc,pi2,ay);}
int kangaroo_feeding(int lc,int bl,int bw,int lm,int tc,int pi2,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lm,tc,pi2,ay);}
int kangaroo_breeding(int lc,int bl,int bw,int lm,int tc,int pi2,int ay){return add(gbr,&st.n_breed,&st.t_lp,N-4,lc,bl,bw,lm,tc,pi2,ay);}
int kangaroo_health(int lc,int bl,int bw,int lm,int tc,int pi2,int ay){return add(ghl,&st.n_health,&st.t_tail,N-6,lc,bl,bw,lm,tc,pi2,ay);}
int kangaroo_market(int lc,int bl,int bw,int lm,int tc,int pi2,int ay){return add(gmk,&st.n_mkt,&st.t_p,N-6,lc,bl,bw,lm,tc,pi2,ay);}
void kangaroo_report(void){ps("[KANG] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Lp=");pi(st.t_lp);ps("\nHlth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" P=");pi(st.t_p);ps("\n");}
void kangaroo_state(void){ps("[KANG] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Kangaroo Admin Demo ===\n\n");kangaroo_init();
/* 1=outback 2=bush 3=plain 4=sanctuary 5=zoo */
ps("Kangaroo habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=130+(i*6),bw=30+(i*5),lm=3+(i%5),tc=80+(i*4),pi2=(i%5)+1,ay=(i%6)+1;kangaroo_habitat(lc,bl,bw,lm,tc,pi2,ay);}
ps("\nKangaroo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=135+(i*5),bw=32+(i*4),lm=4+(i%4),tc=82+(i*3),pi2=(i%4)+1,ay=(i%5)+1;kangaroo_feeding(lc,bl,bw,lm,tc,pi2,ay);}
ps("\nKangaroo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=125+(i*7),bw=28+(i*6),lm=3+(i%5),tc=78+(i*5),pi2=(i%3)+2,ay=(i%4)+1;kangaroo_breeding(lc,bl,bw,lm,tc,pi2,ay);}
ps("\nKangaroo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=140+(i*4),bw=35+(i*3),lm=5+(i%3),tc=85+(i*3),pi2=(i%5)+1,ay=(i%5)+1;kangaroo_health(lc,bl,bw,lm,tc,pi2,ay);}
ps("\nKangaroo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=145+(i*3),bw=38+(i*3),lm=6+(i%3),tc=88+(i*2),pi2=(i%4)+1,ay=(i%3)+2;kangaroo_market(lc,bl,bw,lm,tc,pi2,ay);}
ps("\n");kangaroo_report();kangaroo_state();ps("\n=== Demo Complete ===\n");return 0;}
