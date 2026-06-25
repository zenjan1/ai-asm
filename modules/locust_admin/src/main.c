/* locust_admin: Locust (Locusta migratoria) migratory locust insect (v1.0)
 * Locust field, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_span_cm, swarm_idx, chirp_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,sw_idx,cr_vl,age_yr,active;} locust_t;
typedef struct{int n_field,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_sw,t_cr;} locust_state_t;
static locust_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static locust_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(locust_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int si,int cv,int ay){if(*cnt>=mx)return -1;locust_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->sw_idx=si;x->cr_vl=cv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[LOC] Locust ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" si=");pi(si);ps(" cv=");pi(cv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int locust_init(void){if(init)return -1;st.n_field=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_sw=0;st.t_cr=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[LOC] Locust initialized\n");return 0;}
int locust_field(int lc,int bl,int bw,int ws,int si,int cv,int ay){return add(gpl,&st.n_field,&st.t_ln,N,lc,bl,bw,ws,si,cv,ay);}
int locust_feeding(int lc,int bl,int bw,int ws,int si,int cv,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,si,cv,ay);}
int locust_breeding(int lc,int bl,int bw,int ws,int si,int cv,int ay){return add(gbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,si,cv,ay);}
int locust_health(int lc,int bl,int bw,int ws,int si,int cv,int ay){return add(ghl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,ws,si,cv,ay);}
int locust_market(int lc,int bl,int bw,int ws,int si,int cv,int ay){return add(gmk,&st.n_mkt,&st.t_cr,N-6,lc,bl,bw,ws,si,cv,ay);}
void locust_report(void){ps("[LOC] Field: ");pi(st.n_field);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHlth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Cr=");pi(st.t_cr);ps("\n");}
void locust_state(void){ps("[LOC] Field=");pi(st.n_field);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Locust Admin Demo ===\n\n");locust_init();
/* 1=field 2=desert 3=grassland 4=crop 5=swarm */
ps("Locust field...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=5+(i),bw=2+(i),ws=6+(i%3),si=(i%5)+1,cv=20+(i*5),ay=(i%2)+1;locust_field(lc,bl,bw,ws,si,cv,ay);}
ps("\nLocust feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=5+(i),bw=2+(i),ws=7+(i%3),si=(i%4)+1,cv=25+(i*4),ay=(i%2)+1;locust_feeding(lc,bl,bw,ws,si,cv,ay);}
ps("\nLocust breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=4+(i*2),bw=1+(i),ws=5+(i%4),si=(i%3)+2,cv=18+(i*6),ay=(i%2)+1;locust_breeding(lc,bl,bw,ws,si,cv,ay);}
ps("\nLocust health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=6+(i),bw=3+(i),ws=8+(i%2),si=(i%5)+1,cv=30+(i*3),ay=(i%2)+1;locust_health(lc,bl,bw,ws,si,cv,ay);}
ps("\nLocust market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=7+(i),bw=3+(i),ws=9+(i%2),si=(i%4)+1,cv=35+(i*3),ay=(i%2)+1;locust_market(lc,bl,bw,ws,si,cv,ay);}
ps("\n");locust_report();locust_state();ps("\n=== Demo Complete ===\n");return 0;}
