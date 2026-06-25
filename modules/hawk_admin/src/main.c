/* hawk_admin: Hawk (Buteo buteo) common buzzard hawk raptor (v1.0)
 * Hawk habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, beak_cm, dive_kmh, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,bk_cm,dv_kh,age_yr,active;} hawk_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_bk,t_dv;} hawk_state_t;
static hawk_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static hawk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hawk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int bc,int dk,int ay){if(*cnt>=mx)return -1;hawk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->bk_cm=bc;x->dv_kh=dk;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HAWK] Hawk ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" bc=");pi(bc);ps(" dk=");pi(dk);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hawk_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_bk=0;st.t_dv=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HAWK] Hawk initialized\n");return 0;}
int hawk_habitat(int lc,int bl,int bw,int ws,int bc,int dk,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,ws,bc,dk,ay);}
int hawk_feeding(int lc,int bl,int bw,int ws,int bc,int dk,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,bc,dk,ay);}
int hawk_breeding(int lc,int bl,int bw,int ws,int bc,int dk,int ay){return add(gbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,bc,dk,ay);}
int hawk_health(int lc,int bl,int bw,int ws,int bc,int dk,int ay){return add(ghl,&st.n_health,&st.t_bk,N-6,lc,bl,bw,ws,bc,dk,ay);}
int hawk_market(int lc,int bl,int bw,int ws,int bc,int dk,int ay){return add(gmk,&st.n_mkt,&st.t_dv,N-6,lc,bl,bw,ws,bc,dk,ay);}
void hawk_report(void){ps("[HAWK] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHlth: ");pi(st.n_health);ps(" Bk=");pi(st.t_bk);ps("\nMkt: ");pi(st.n_mkt);ps(" Dv=");pi(st.t_dv);ps("\n");}
void hawk_state(void){ps("[HAWK] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hawk Admin Demo ===\n\n");hawk_init();
/* 1=field 2=woodland 3=cliff 4=meadow 5=aerie */
ps("Hawk habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*3),bw=1+(i*2),ws=110+(i*5),bc=3+(i%3),dv=80+(i*10),ay=(i%5)+1;hawk_habitat(lc,bl,bw,ws,bc,dv,ay);}
ps("\nHawk feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=52+(i*2),bw=1+(i*2),ws=115+(i*4),bc=4+(i%3),dv=85+(i*8),ay=(i%4)+1;hawk_feeding(lc,bl,bw,ws,bc,dv,ay);}
ps("\nHawk breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=48+(i*4),bw=2+(i*2),ws=105+(i*6),bc=2+(i%4),dv=75+(i*12),ay=(i%3)+1;hawk_breeding(lc,bl,bw,ws,bc,dv,ay);}
ps("\nHawk health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=55+(i*3),bw=2+(i*2),ws=120+(i*4),bc=5+(i%2),dv=90+(i*7),ay=(i%4)+1;hawk_health(lc,bl,bw,ws,bc,dv,ay);}
ps("\nHawk market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=58+(i*2),bw=2+(i*2),ws=125+(i*3),bc=6+(i%2),dv=95+(i*5),ay=(i%3)+1;hawk_market(lc,bl,bw,ws,bc,dv,ay);}
ps("\n");hawk_report();hawk_state();ps("\n=== Demo Complete ===\n");return 0;}
