/* koala_admin: Koala (Phascolarctos cinereus) koala arboreal marsupial (v1.0)
 * Koala habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, sleep_hr, leaf_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,sl_hr,lf_ct,age_yr,active;} koala_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_sl,t_lf;} koala_state_t;
static koala_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static koala_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(koala_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int sh,int lfc,int ay){if(*cnt>=mx)return -1;koala_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->sl_hr=sh;x->lf_ct=lfc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KOAL] Koala ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" sh=");pi(sh);ps(" lfc=");pi(lfc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int koala_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_sl=0;st.t_lf=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[KOAL] Koala initialized\n");return 0;}
int koala_habitat(int lc,int bl,int bw,int ec,int sh,int lfc,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,ec,sh,lfc,ay);}
int koala_feeding(int lc,int bl,int bw,int ec,int sh,int lfc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,sh,lfc,ay);}
int koala_breeding(int lc,int bl,int bw,int ec,int sh,int lfc,int ay){return add(gbr,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,sh,lfc,ay);}
int koala_health(int lc,int bl,int bw,int ec,int sh,int lfc,int ay){return add(ghl,&st.n_health,&st.t_sl,N-6,lc,bl,bw,ec,sh,lfc,ay);}
int koala_market(int lc,int bl,int bw,int ec,int sh,int lfc,int ay){return add(gmk,&st.n_mkt,&st.t_lf,N-6,lc,bl,bw,ec,sh,lfc,ay);}
void koala_report(void){ps("[KOAL] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHlth: ");pi(st.n_health);ps(" Sl=");pi(st.t_sl);ps("\nMkt: ");pi(st.n_mkt);ps(" Lf=");pi(st.t_lf);ps("\n");}
void koala_state(void){ps("[KOAL] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Koala Admin Demo ===\n\n");koala_init();
/* 1=eucalyptus 2=forest 3=sanctuary 4=zoo 5=park */
ps("Koala habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*3),bw=8+(i*2),ec=5+(i%4),sh=14+(i%6),lf=200+(i*30),ay=(i%6)+1;koala_habitat(lc,bl,bw,ec,sh,lf,ay);}
ps("\nKoala feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=62+(i*3),bw=8+(i*2),ec=6+(i%3),sh=15+(i%5),lf=220+(i*25),ay=(i%5)+1;koala_feeding(lc,bl,bw,ec,sh,lf,ay);}
ps("\nKoala breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=58+(i*4),bw=7+(i*2),ec=4+(i%4),sh=13+(i%6),lf=180+(i*35),ay=(i%4)+1;koala_breeding(lc,bl,bw,ec,sh,lf,ay);}
ps("\nKoala health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=65+(i*2),bw=9+(i*2),ec=7+(i%3),sh=16+(i%4),lf=240+(i*20),ay=(i%5)+1;koala_health(lc,bl,bw,ec,sh,lf,ay);}
ps("\nKoala market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=68+(i*2),bw=10+(i*2),ec=8+(i%2),sh=17+(i%3),lf=260+(i*15),ay=(i%3)+2;koala_market(lc,bl,bw,ec,sh,lf,ay);}
ps("\n");koala_report();koala_state();ps("\n=== Demo Complete ===\n");return 0;}
