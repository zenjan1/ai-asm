/* gull_admin: Gull (Larus canus) common gull seabird (v1.0)
 * Gull habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, beak_cm, dive_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,bk_cm,dv_idx,age_yr,active;} gull_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_bk,t_dv;} gull_state_t;
static gull_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static gull_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gull_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int bc,int di,int ay){if(*cnt>=mx)return -1;gull_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->bk_cm=bc;x->dv_idx=di;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GULL] Gull ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" bc=");pi(bc);ps(" di=");pi(di);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gull_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_bk=0;st.t_dv=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GULL] Gull initialized\n");return 0;}
int gull_habitat(int lc,int bl,int bw,int ws,int bc,int di,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,ws,bc,di,ay);}
int gull_feeding(int lc,int bl,int bw,int ws,int bc,int di,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,bc,di,ay);}
int gull_breeding(int lc,int bl,int bw,int ws,int bc,int di,int ay){return add(gbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,bc,di,ay);}
int gull_health(int lc,int bl,int bw,int ws,int bc,int di,int ay){return add(ghl,&st.n_health,&st.t_bk,N-6,lc,bl,bw,ws,bc,di,ay);}
int gull_market(int lc,int bl,int bw,int ws,int bc,int di,int ay){return add(gmk,&st.n_mkt,&st.t_dv,N-6,lc,bl,bw,ws,bc,di,ay);}
void gull_report(void){ps("[GULL] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHlth: ");pi(st.n_health);ps(" Bk=");pi(st.t_bk);ps("\nMkt: ");pi(st.n_mkt);ps(" Dv=");pi(st.t_dv);ps("\n");}
void gull_state(void){ps("[GULL] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gull Admin Demo ===\n\n");gull_init();
/* 1=coast 2=cliff 3=beach 4=harbor 5=island */
ps("Gull habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=1+(i*2),ws=100+(i*5),bc=4+(i%3),dv=(i%5)+1,ay=(i%6)+1;gull_habitat(lc,bl,bw,ws,bc,dv,ay);}
ps("\nGull feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=42+(i*2),bw=1+(i*2),ws=105+(i*4),bc=5+(i%3),dv=(i%4)+1,ay=(i%5)+1;gull_feeding(lc,bl,bw,ws,bc,dv,ay);}
ps("\nGull breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=38+(i*4),bw=2+(i*2),ws=95+(i*6),bc=3+(i%4),dv=(i%3)+2,ay=(i%4)+1;gull_breeding(lc,bl,bw,ws,bc,dv,ay);}
ps("\nGull health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=45+(i*2),bw=2+(i*2),ws=110+(i*4),bc=6+(i%3),dv=(i%5)+1,ay=(i%5)+1;gull_health(lc,bl,bw,ws,bc,dv,ay);}
ps("\nGull market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=48+(i*2),bw=2+(i*2),ws=115+(i*3),bc=7+(i%2),dv=(i%4)+1,ay=(i%3)+2;gull_market(lc,bl,bw,ws,bc,dv,ay);}
ps("\n");gull_report();gull_state();ps("\n=== Demo Complete ===\n");return 0;}
