/* fowl_admin: Fowl (Gallus gallus domesticus) domestic poultry bird husbandry (v1.0)
 * Fowl housing, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, comb_cm, feather_idx, egg_wt_g, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,comb_cm,fthr_idx,egg_wt,age_yr,active;} fowl_t;
typedef struct{int n_house,n_feed,n_breed,n_hlth,n_mkt,t_ln,t_wt,t_cm,t_ft,t_eg;} fowl_state_t;
static fowl_t fhs[N],ffd[N-2],fbr[N-4],fhl[N-6],fmk[N-6]; static fowl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fowl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cm,int fi,int ew,int ay){if(*cnt>=mx)return -1;fowl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->comb_cm=cm;x->fthr_idx=fi;x->egg_wt=ew;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FOWL] Fowl ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cm=");pi(cm);ps(" fi=");pi(fi);ps(" ew=");pi(ew);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int fowl_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_hlth=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_cm=0;st.t_ft=0;st.t_eg=0;for(int i=0;i<N;i++)fhs[i].active=0;for(int i=0;i<N-2;i++)ffd[i].active=0;for(int i=0;i<N-4;i++)fbr[i].active=0;for(int i=0;i<N-6;i++)fhl[i].active=0;for(int i=0;i<N-6;i++)fmk[i].active=0;init=1;ps("[FOWL] Fowl initialized\n");return 0;}
int fowl_housing(int lc,int bl,int bw,int cm,int fi,int ew,int ay){return add(fhs,&st.n_house,&st.t_ln,N,lc,bl,bw,cm,fi,ew,ay);}
int fowl_feeding(int lc,int bl,int bw,int cm,int fi,int ew,int ay){return add(ffd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cm,fi,ew,ay);}
int fowl_breeding(int lc,int bl,int bw,int cm,int fi,int ew,int ay){return add(fbr,&st.n_breed,&st.t_cm,N-4,lc,bl,bw,cm,fi,ew,ay);}
int fowl_health(int lc,int bl,int bw,int cm,int fi,int ew,int ay){return add(fhl,&st.n_hlth,&st.t_ft,N-6,lc,bl,bw,cm,fi,ew,ay);}
int fowl_market(int lc,int bl,int bw,int cm,int fi,int ew,int ay){return add(fmk,&st.n_mkt,&st.t_eg,N-6,lc,bl,bw,cm,fi,ew,ay);}
void fowl_report(void){ps("[FOWL] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Cm=");pi(st.t_cm);ps("\nHlth: ");pi(st.n_hlth);ps(" Ft=");pi(st.t_ft);ps("\nMkt: ");pi(st.n_mkt);ps(" Eg=");pi(st.t_eg);ps("\n");}
void fowl_state(void){ps("[FOWL] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_hlth);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Fowl Admin Demo ===\n\n");fowl_init();
/* 1=coop 2=barn 3=free-range 4=pen 5=market */
ps("Fowl housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*3),bw=2+(i%3),cm=4+(i%2),fi=(i%6)+1,ew=50+(i*5),ay=1+(i%5);fowl_housing(lc,bl,bw,cm,fi,ew,ay);}
ps("\nFowl feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=38+(i*2),bw=2+(i%2),cm=5+(i%2),fi=(i%5)+1,ew=55+(i*4),ay=1+(i%4);fowl_feeding(lc,bl,bw,cm,fi,ew,ay);}
ps("\nFowl breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=33+(i*4),bw=2+(i%3),cm=3+(i%3),fi=(i%4)+1,ew=60+(i*3),ay=1+(i%3);fowl_breeding(lc,bl,bw,cm,fi,ew,ay);}
ps("\nFowl health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=40+(i*3),bw=3+(i%2),cm=4+(i%2),fi=(i%7)+1,ew=45+(i*6),ay=2+(i%5);fowl_health(lc,bl,bw,cm,fi,ew,ay);}
ps("\nFowl market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*2),bw=3+(i%2),cm=6+(i%2),fi=(i%3)+4,ew=65+(i*3),ay=2+(i%3);fowl_market(lc,bl,bw,cm,fi,ew,ay);}
ps("\n");fowl_report();fowl_state();ps("\n=== Demo Complete ===\n");return 0;}
