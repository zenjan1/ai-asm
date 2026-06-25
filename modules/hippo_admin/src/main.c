/* hippo_admin: Hippo (Hippopotamus amphibius) common hippopotamus aquatic mammal (v1.0)
 * Hippo habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tusk_cm, bite_kgf, submerge_min, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tusk_cm,bt_kgf,sub_mn,age_yr,active;} hippo_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tusk,t_bt,t_sub;} hippo_state_t;
static hippo_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static hippo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hippo_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int bk,int sm,int ay){if(*cnt>=mx)return -1;hippo_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tusk_cm=tc;x->bt_kgf=bk;x->sub_mn=sm;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HIP] Hippo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" bk=");pi(bk);ps(" sm=");pi(sm);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hippo_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tusk=0;st.t_bt=0;st.t_sub=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HIP] Hippo initialized\n");return 0;}
int hippo_habitat(int lc,int bl,int bw,int tc,int bk,int sm,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,tc,bk,sm,ay);}
int hippo_feeding(int lc,int bl,int bw,int tc,int bk,int sm,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,bk,sm,ay);}
int hippo_breeding(int lc,int bl,int bw,int tc,int bk,int sm,int ay){return add(gbr,&st.n_breed,&st.t_tusk,N-4,lc,bl,bw,tc,bk,sm,ay);}
int hippo_health(int lc,int bl,int bw,int tc,int bk,int sm,int ay){return add(ghl,&st.n_health,&st.t_bt,N-6,lc,bl,bw,tc,bk,sm,ay);}
int hippo_market(int lc,int bl,int bw,int tc,int bk,int sm,int ay){return add(gmk,&st.n_mkt,&st.t_sub,N-6,lc,bl,bw,tc,bk,sm,ay);}
void hippo_report(void){ps("[HIP] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tusk=");pi(st.t_tusk);ps("\nHlth: ");pi(st.n_health);ps(" Bt=");pi(st.t_bt);ps("\nMkt: ");pi(st.n_mkt);ps(" Sub=");pi(st.t_sub);ps("\n");}
void hippo_state(void){ps("[HIP] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hippo Admin Demo ===\n\n");hippo_init();
/* 1=river 2=lake 3=wallow 4=savanna 5=reserve */
ps("Hippo habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=350+(i*15),bw=1500+(i*200),tc=30+(i*3),bk=800+(i*50),sm=3+(i%5),ay=(i%8)+1;hippo_habitat(lc,bl,bw,tc,bk,sm,ay);}
ps("\nHippo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=360+(i*12),bw=1600+(i*150),tc=32+(i*3),bk=850+(i*40),sm=4+(i%4),ay=(i%6)+1;hippo_feeding(lc,bl,bw,tc,bk,sm,ay);}
ps("\nHippo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=340+(i*18),bw=1400+(i*250),tc=28+(i*4),bk=750+(i*60),sm=3+(i%5),ay=(i%5)+2;hippo_breeding(lc,bl,bw,tc,bk,sm,ay);}
ps("\nHippo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=370+(i*10),bw=1700+(i*120),tc=35+(i*2),bk=900+(i*35),sm=5+(i%3),ay=(i%6)+1;hippo_health(lc,bl,bw,tc,bk,sm,ay);}
ps("\nHippo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=380+(i*8),bw=1800+(i*100),tc=38+(i*2),bk=950+(i*30),sm=6+(i%3),ay=(i%4)+2;hippo_market(lc,bl,bw,tc,bk,sm,ay);}
ps("\n");hippo_report();hippo_state();ps("\n=== Demo Complete ===\n");return 0;}
