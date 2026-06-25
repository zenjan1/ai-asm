/* jaguar_admin: Jaguar (Panthera onca) jaguar big cat (v1.0)
 * Jaguar habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, bite_kgf, spot_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,bt_kgf,sp_ct,age_yr,active;} jaguar_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_bt,t_sp;} jaguar_state_t;
static jaguar_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static jaguar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jaguar_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int bk,int sc,int ay){if(*cnt>=mx)return -1;jaguar_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->bt_kgf=bk;x->sp_ct=sc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[JAG] Jaguar ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" bk=");pi(bk);ps(" sc=");pi(sc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int jaguar_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_bt=0;st.t_sp=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[JAG] Jaguar initialized\n");return 0;}
int jaguar_habitat(int lc,int bl,int bw,int tc,int bk,int sc,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,tc,bk,sc,ay);}
int jaguar_feeding(int lc,int bl,int bw,int tc,int bk,int sc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,bk,sc,ay);}
int jaguar_breeding(int lc,int bl,int bw,int tc,int bk,int sc,int ay){return add(gbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,bk,sc,ay);}
int jaguar_health(int lc,int bl,int bw,int tc,int bk,int sc,int ay){return add(ghl,&st.n_health,&st.t_bt,N-6,lc,bl,bw,tc,bk,sc,ay);}
int jaguar_market(int lc,int bl,int bw,int tc,int bk,int sc,int ay){return add(gmk,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,tc,bk,sc,ay);}
void jaguar_report(void){ps("[JAG] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHlth: ");pi(st.n_health);ps(" Bt=");pi(st.t_bt);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void jaguar_state(void){ps("[JAG] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Jaguar Admin Demo ===\n\n");jaguar_init();
/* 1=jungle 2=rainforest 3=swamp 4=reserve 5=sanctuary */
ps("Jaguar habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=160+(i*6),bw=60+(i*5),tc=70+(i*3),bk=700+(i*40),sc=30+(i*4),ay=(i%6)+1;jaguar_habitat(lc,bl,bw,tc,bk,sc,ay);}
ps("\nJaguar feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=165+(i*5),bw=62+(i*4),tc=72+(i*3),bk=720+(i*35),sc=32+(i*3),ay=(i%5)+1;jaguar_feeding(lc,bl,bw,tc,bk,sc,ay);}
ps("\nJaguar breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=155+(i*7),bw=58+(i*6),tc=68+(i*4),bk=680+(i*45),sc=28+(i*5),ay=(i%4)+1;jaguar_breeding(lc,bl,bw,tc,bk,sc,ay);}
ps("\nJaguar health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=170+(i*4),bw=65+(i*4),tc=75+(i*2),bk=740+(i*30),sc=35+(i*3),ay=(i%5)+1;jaguar_health(lc,bl,bw,tc,bk,sc,ay);}
ps("\nJaguar market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=175+(i*3),bw=68+(i*3),tc=78+(i*2),bk=760+(i*25),sc=38+(i*2),ay=(i%3)+2;jaguar_market(lc,bl,bw,tc,bk,sc,ay);}
ps("\n");jaguar_report();jaguar_state();ps("\n=== Demo Complete ===\n");return 0;}
