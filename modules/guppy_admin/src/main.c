/* guppy_admin: Guppy (Poecilia reticulata) common guppy tropical fish (v1.0)
 * Guppy tank, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_span_cm, color_idx, spot_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_sp,cl_idx,sp_ct,age_yr,active;} guppy_t;
typedef struct{int n_tank,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cl,t_sp;} guppy_state_t;
static guppy_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static guppy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(guppy_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ts,int ci,int sc,int ay){if(*cnt>=mx)return -1;guppy_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_sp=ts;x->cl_idx=ci;x->sp_ct=sc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GUP] Guppy ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ts=");pi(ts);ps(" ci=");pi(ci);ps(" sc=");pi(sc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int guppy_init(void){if(init)return -1;st.n_tank=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cl=0;st.t_sp=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GUP] Guppy initialized\n");return 0;}
int guppy_tank(int lc,int bl,int bw,int ts,int ci,int sc,int ay){return add(gpl,&st.n_tank,&st.t_ln,N,lc,bl,bw,ts,ci,sc,ay);}
int guppy_feeding(int lc,int bl,int bw,int ts,int ci,int sc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ts,ci,sc,ay);}
int guppy_breeding(int lc,int bl,int bw,int ts,int ci,int sc,int ay){return add(gbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,ts,ci,sc,ay);}
int guppy_health(int lc,int bl,int bw,int ts,int ci,int sc,int ay){return add(ghl,&st.n_health,&st.t_cl,N-6,lc,bl,bw,ts,ci,sc,ay);}
int guppy_market(int lc,int bl,int bw,int ts,int ci,int sc,int ay){return add(gmk,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,ts,ci,sc,ay);}
void guppy_report(void){ps("[GUP] Tank: ");pi(st.n_tank);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHlth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void guppy_state(void){ps("[GUP] Tank=");pi(st.n_tank);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Guppy Admin Demo ===\n\n");guppy_init();
/* 1=aquarium 2=pond 3=stream 4=river 5=tank */
ps("Guppy tank...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=3+(i),bw=1+(i),ts=2+(i%3),ci=(i%5)+1,sc=4+(i*2),ay=(i%2)+1;guppy_tank(lc,bl,bw,ts,ci,sc,ay);}
ps("\nGuppy feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=4+(i),bw=1+(i),ts=3+(i%3),ci=(i%4)+1,sc=5+(i*2),ay=(i%2)+1;guppy_feeding(lc,bl,bw,ts,ci,sc,ay);}
ps("\nGuppy breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=2+(i),bw=1+(i),ts=2+(i%4),ci=(i%3)+2,sc=3+(i*2),ay=(i%2)+1;guppy_breeding(lc,bl,bw,ts,ci,sc,ay);}
ps("\nGuppy health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=5+(i),bw=2+(i),ts=4+(i%2),ci=(i%5)+1,sc=6+(i*2),ay=(i%2)+1;guppy_health(lc,bl,bw,ts,ci,sc,ay);}
ps("\nGuppy market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=6+(i),bw=2+(i),ts=5+(i%2),ci=(i%4)+1,sc=7+(i*2),ay=(i%2)+1;guppy_market(lc,bl,bw,ts,ci,sc,ay);}
ps("\n");guppy_report();guppy_state();ps("\n=== Demo Complete ===\n");return 0;}
