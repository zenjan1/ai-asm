/* gazelle_admin: Gazelle (Gazella gazella) slender antelope ungulate (v1.0)
 * Gazelle habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, speed_kmh, coat_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,spd_kh,ct_idx,age_yr,active;} gazelle_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_spd,t_ct;} gazelle_state_t;
static gazelle_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static gazelle_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gazelle_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int sk,int ci,int ay){if(*cnt>=mx)return -1;gazelle_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->spd_kh=sk;x->ct_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GAZ] Gazelle ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" sk=");pi(sk);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gazelle_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_spd=0;st.t_ct=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GAZ] Gazelle initialized\n");return 0;}
int gazelle_habitat(int lc,int bl,int bw,int hc,int sk,int ci,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,hc,sk,ci,ay);}
int gazelle_feeding(int lc,int bl,int bw,int hc,int sk,int ci,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,sk,ci,ay);}
int gazelle_breeding(int lc,int bl,int bw,int hc,int sk,int ci,int ay){return add(gbr,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,sk,ci,ay);}
int gazelle_health(int lc,int bl,int bw,int hc,int sk,int ci,int ay){return add(ghl,&st.n_health,&st.t_spd,N-6,lc,bl,bw,hc,sk,ci,ay);}
int gazelle_market(int lc,int bl,int bw,int hc,int sk,int ci,int ay){return add(gmk,&st.n_mkt,&st.t_ct,N-6,lc,bl,bw,hc,sk,ci,ay);}
void gazelle_report(void){ps("[GAZ] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHlth: ");pi(st.n_health);ps(" Spd=");pi(st.t_spd);ps("\nMkt: ");pi(st.n_mkt);ps(" Ct=");pi(st.t_ct);ps("\n");}
void gazelle_state(void){ps("[GAZ] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gazelle Admin Demo ===\n\n");gazelle_init();
/* 1=savanna 2=grassland 3=steppe 4=desert 5=reserve */
ps("Gazelle habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*5),bw=20+(i*2),hc=15+(i%8),sk=60+(i*4),ci=(i%5)+1,ay=(i%6)+1;gazelle_habitat(lc,bl,bw,hc,sk,ci,ay);}
ps("\nGazelle feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=85+(i*4),bw=22+(i*2),hc=18+(i%6),sk=65+(i*3),ci=(i%4)+1,ay=(i%5)+1;gazelle_feeding(lc,bl,bw,hc,sk,ci,ay);}
ps("\nGazelle breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=75+(i*6),bw=25+(i*2),hc=20+(i%5),sk=55+(i*5),ci=(i%3)+2,ay=(i%4)+2;gazelle_breeding(lc,bl,bw,hc,sk,ci,ay);}
ps("\nGazelle health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=90+(i*3),bw=18+(i*3),hc=12+(i%7),sk=70+(i*3),ci=(i%5)+1,ay=(i%6)+1;gazelle_health(lc,bl,bw,hc,sk,ci,ay);}
ps("\nGazelle market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=95+(i*4),bw=28+(i*2),hc=22+(i%4),sk=75+(i*3),ci=(i%4)+1,ay=(i%5)+1;gazelle_market(lc,bl,bw,hc,sk,ci,ay);}
ps("\n");gazelle_report();gazelle_state();ps("\n=== Demo Complete ===\n");return 0;}
