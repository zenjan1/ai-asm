/* hornet_admin: Hornet (Vespa crabro) European hornet large insect (v1.0)
 * Hornet nest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_span_cm, sting_idx, nest_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,st_idx,n_ct,age_yr,active;} hornet_t;
typedef struct{int n_nest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_st,t_n;} hornet_state_t;
static hornet_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static hornet_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hornet_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int si,int nc,int ay){if(*cnt>=mx)return -1;hornet_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->st_idx=si;x->n_ct=nc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HOR] Hornet ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" si=");pi(si);ps(" nc=");pi(nc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hornet_init(void){if(init)return -1;st.n_nest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_st=0;st.t_n=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HOR] Hornet initialized\n");return 0;}
int hornet_nest(int lc,int bl,int bw,int ws,int si,int nc,int ay){return add(gpl,&st.n_nest,&st.t_ln,N,lc,bl,bw,ws,si,nc,ay);}
int hornet_feeding(int lc,int bl,int bw,int ws,int si,int nc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,si,nc,ay);}
int hornet_breeding(int lc,int bl,int bw,int ws,int si,int nc,int ay){return add(gbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,si,nc,ay);}
int hornet_health(int lc,int bl,int bw,int ws,int si,int nc,int ay){return add(ghl,&st.n_health,&st.t_st,N-6,lc,bl,bw,ws,si,nc,ay);}
int hornet_market(int lc,int bl,int bw,int ws,int si,int nc,int ay){return add(gmk,&st.n_mkt,&st.t_n,N-6,lc,bl,bw,ws,si,nc,ay);}
void hornet_report(void){ps("[HOR] Nest: ");pi(st.n_nest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHlth: ");pi(st.n_health);ps(" St=");pi(st.t_st);ps("\nMkt: ");pi(st.n_mkt);ps(" N=");pi(st.t_n);ps("\n");}
void hornet_state(void){ps("[HOR] Nest=");pi(st.n_nest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hornet Admin Demo ===\n\n");hornet_init();
/* 1=tree 2=eaves 3=log 4=attic 5=garden */
ps("Hornet nest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=3+(i%3),bw=1+(i),ws=5+(i%4),si=(i%5)+1,nc=10+(i*8),ay=(i%2)+1;hornet_nest(lc,bl,bw,ws,si,nc,ay);}
ps("\nHornet feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=3+(i%3),bw=1+(i),ws=5+(i%4),si=(i%4)+1,nc=12+(i*6),ay=(i%2)+1;hornet_feeding(lc,bl,bw,ws,si,nc,ay);}
ps("\nHornet breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=2+(i%4),bw=1+(i),ws=4+(i%5),si=(i%3)+2,nc=8+(i*10),ay=(i%2)+1;hornet_breeding(lc,bl,bw,ws,si,nc,ay);}
ps("\nHornet health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=4+(i%3),bw=2+(i),ws=6+(i%3),si=(i%5)+1,nc=15+(i*5),ay=(i%2)+1;hornet_health(lc,bl,bw,ws,si,nc,ay);}
ps("\nHornet market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=4+(i%2),bw=2+(i),ws=7+(i%3),si=(i%4)+1,nc=18+(i*4),ay=(i%2)+1;hornet_market(lc,bl,bw,ws,si,nc,ay);}
ps("\n");hornet_report();hornet_state();ps("\n=== Demo Complete ===\n");return 0;}
