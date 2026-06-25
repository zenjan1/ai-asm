/* guinea_admin: Guinea Pig (Cavia porcellus) domestic guinea pig rodent (v1.0)
 * Guinea pig housing, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, ear_cm, whisker_idx, squeak_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,wh_idx,sq_vl,age_yr,active;} guinea_t;
typedef struct{int n_hous,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_wh,t_sq;} guinea_state_t;
static guinea_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static guinea_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(guinea_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int wi,int sv,int ay){if(*cnt>=mx)return -1;guinea_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->wh_idx=wi;x->sq_vl=sv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GUI] Guinea ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" wi=");pi(wi);ps(" sv=");pi(sv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int guinea_init(void){if(init)return -1;st.n_hous=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_wh=0;st.t_sq=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GUI] Guinea initialized\n");return 0;}
int guinea_housing(int lc,int bl,int bw,int ec,int wi,int sv,int ay){return add(gpl,&st.n_hous,&st.t_ln,N,lc,bl,bw,ec,wi,sv,ay);}
int guinea_feeding(int lc,int bl,int bw,int ec,int wi,int sv,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,wi,sv,ay);}
int guinea_breeding(int lc,int bl,int bw,int ec,int wi,int sv,int ay){return add(gbr,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,wi,sv,ay);}
int guinea_health(int lc,int bl,int bw,int ec,int wi,int sv,int ay){return add(ghl,&st.n_health,&st.t_wh,N-6,lc,bl,bw,ec,wi,sv,ay);}
int guinea_market(int lc,int bl,int bw,int ec,int wi,int sv,int ay){return add(gmk,&st.n_mkt,&st.t_sq,N-6,lc,bl,bw,ec,wi,sv,ay);}
void guinea_report(void){ps("[GUI] Hous: ");pi(st.n_hous);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHlth: ");pi(st.n_health);ps(" Wh=");pi(st.t_wh);ps("\nMkt: ");pi(st.n_mkt);ps(" Sq=");pi(st.t_sq);ps("\n");}
void guinea_state(void){ps("[GUI] Hous=");pi(st.n_hous);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Guinea Pig Admin Demo ===\n\n");guinea_init();
/* 1=cage 2=hutch 3=pen 4=barn 5=home */
ps("Guinea housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*2),bw=400+(i*30),ec=3+(i%3),wi=(i%5)+1,sv=40+(i*6),ay=(i%4)+1;guinea_housing(lc,bl,bw,ec,wi,sv,ay);}
ps("\nGuinea feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=22+(i*2),bw=420+(i*25),ec=4+(i%3),wi=(i%4)+1,sv=45+(i*5),ay=(i%3)+1;guinea_feeding(lc,bl,bw,ec,wi,sv,ay);}
ps("\nGuinea breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=18+(i*3),bw=380+(i*35),ec=2+(i%4),wi=(i%3)+2,sv=35+(i*7),ay=(i%3)+1;guinea_breeding(lc,bl,bw,ec,wi,sv,ay);}
ps("\nGuinea health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=25+(i*2),bw=450+(i*20),ec=5+(i%2),wi=(i%5)+1,sv=50+(i*4),ay=(i%4)+1;guinea_health(lc,bl,bw,ec,wi,sv,ay);}
ps("\nGuinea market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=28+(i*2),bw=500+(i*15),ec=6+(i%2),wi=(i%4)+1,sv=55+(i*3),ay=(i%3)+1;guinea_market(lc,bl,bw,ec,wi,sv,ay);}
ps("\n");guinea_report();guinea_state();ps("\n=== Demo Complete ===\n");return 0;}
