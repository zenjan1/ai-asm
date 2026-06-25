/* leopard_admin: Leopard (Panthera pardus) leopard big cat (v1.0)
 * Leopard habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, sprint_kmh, climb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,sp_kh,cl_idx,age_yr,active;} leopard_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_sp,t_cl;} leopard_state_t;
static leopard_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static leopard_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(leopard_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int sk,int ci,int ay){if(*cnt>=mx)return -1;leopard_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->sp_kh=sk;x->cl_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[LEOP] Leopard ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" sk=");pi(sk);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int leopard_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_sp=0;st.t_cl=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[LEOP] Leopard initialized\n");return 0;}
int leopard_habitat(int lc,int bl,int bw,int tc,int sk,int ci,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,tc,sk,ci,ay);}
int leopard_feeding(int lc,int bl,int bw,int tc,int sk,int ci,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,sk,ci,ay);}
int leopard_breeding(int lc,int bl,int bw,int tc,int sk,int ci,int ay){return add(gbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,sk,ci,ay);}
int leopard_health(int lc,int bl,int bw,int tc,int sk,int ci,int ay){return add(ghl,&st.n_health,&st.t_sp,N-6,lc,bl,bw,tc,sk,ci,ay);}
int leopard_market(int lc,int bl,int bw,int tc,int sk,int ci,int ay){return add(gmk,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,tc,sk,ci,ay);}
void leopard_report(void){ps("[LEOP] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHlth: ");pi(st.n_health);ps(" Sp=");pi(st.t_sp);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void leopard_state(void){ps("[LEOP] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Leopard Admin Demo ===\n\n");leopard_init();
/* 1=savanna 2=forest 3=mountain 4=jungle 5=reserve */
ps("Leopard habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*5),bw=40+(i*4),tc=70+(i*3),sk=50+(i*4),ci=(i%5)+1,ay=(i%6)+1;leopard_habitat(lc,bl,bw,tc,sk,ci,ay);}
ps("\nLeopard feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=125+(i*4),bw=42+(i*3),tc=72+(i*3),sk=55+(i*3),ci=(i%4)+1,ay=(i%5)+1;leopard_feeding(lc,bl,bw,tc,sk,ci,ay);}
ps("\nLeopard breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=115+(i*6),bw=38+(i*5),tc=68+(i*4),sk=45+(i*5),ci=(i%3)+2,ay=(i%4)+1;leopard_breeding(lc,bl,bw,tc,sk,ci,ay);}
ps("\nLeopard health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=130+(i*4),bw=45+(i*3),tc=75+(i*2),sk=60+(i*3),ci=(i%5)+1,ay=(i%5)+1;leopard_health(lc,bl,bw,tc,sk,ci,ay);}
ps("\nLeopard market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=135+(i*3),bw=48+(i*3),tc=78+(i*2),sk=65+(i*2),ci=(i%4)+1,ay=(i%3)+2;leopard_market(lc,bl,bw,tc,sk,ci,ay);}
ps("\n");leopard_report();leopard_state();ps("\n=== Demo Complete ===\n");return 0;}
