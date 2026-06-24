/* duck_admin: Duck (Anas platyrhynchos) domestic duck waterfowl husbandry (v1.0)
 * Duck housing, feeding, breeding, grooming, market
 * Features: body_len_cm, body_wt_kg, bill_cm, feather_idx, egg_wt_g, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,bill_cm,fthr_idx,egg_wt,age_yr,active;} duck_t;
typedef struct{int n_house,n_feed,n_breed,n_groom,n_mkt,t_ln,t_wt,t_bl,t_ft,t_eg;} duck_state_t;
static duck_t dhs[N],dfd[N-2],dbr[N-4],dgr[N-6],dmk[N-6]; static duck_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(duck_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bm,int fi,int ew,int ay){if(*cnt>=mx)return -1;duck_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->bill_cm=bm;x->fthr_idx=fi;x->egg_wt=ew;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DUCK] Duck ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bm=");pi(bm);ps(" fi=");pi(fi);ps(" ew=");pi(ew);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int duck_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_bl=0;st.t_ft=0;st.t_eg=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dbr[i].active=0;for(int i=0;i<N-6;i++)dgr[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DUCK] Duck initialized\n");return 0;}
int duck_housing(int lc,int bl,int bw,int bm,int fi,int ew,int ay){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,bm,fi,ew,ay);}
int duck_feeding(int lc,int bl,int bw,int bm,int fi,int ew,int ay){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bm,fi,ew,ay);}
int duck_breeding(int lc,int bl,int bw,int bm,int fi,int ew,int ay){return add(dbr,&st.n_breed,&st.t_bl,N-4,lc,bl,bw,bm,fi,ew,ay);}
int duck_grooming(int lc,int bl,int bw,int bm,int fi,int ew,int ay){return add(dgr,&st.n_groom,&st.t_ft,N-6,lc,bl,bw,bm,fi,ew,ay);}
int duck_market(int lc,int bl,int bw,int bm,int fi,int ew,int ay){return add(dmk,&st.n_mkt,&st.t_eg,N-6,lc,bl,bw,bm,fi,ew,ay);}
void duck_report(void){ps("[DUCK] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Bl=");pi(st.t_bl);ps("\nGroom: ");pi(st.n_groom);ps(" Ft=");pi(st.t_ft);ps("\nMkt: ");pi(st.n_mkt);ps(" Eg=");pi(st.t_eg);ps("\n");}
void duck_state(void){ps("[DUCK] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Duck Admin Demo ===\n\n");duck_init();
/* 1=pond 2=barn 3=free-range 4=pen 5=market */
ps("Duck housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*4),bw=2+(i%3),bm=5+(i%2),fi=(i%6)+1,ew=60+(i*5),ay=1+(i%6);duck_housing(lc,bl,bw,bm,fi,ew,ay);}
ps("\nDuck feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=50+(i*3),bw=3+(i%2),bm=6+(i%2),fi=(i%5)+1,ew=65+(i*4),ay=2+(i%5);duck_feeding(lc,bl,bw,bm,fi,ew,ay);}
ps("\nDuck breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=48+(i*4),bw=2+(i%3),bm=5+(i%3),fi=(i%4)+1,ew=70+(i*3),ay=1+(i%4);duck_breeding(lc,bl,bw,bm,fi,ew,ay);}
ps("\nDuck grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=42+(i*5),bw=2+(i%4),bm=4+(i%3),fi=(i%7)+1,ew=55+(i*6),ay=3+(i%7);duck_grooming(lc,bl,bw,bm,fi,ew,ay);}
ps("\nDuck market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=55+(i*3),bw=3+(i%2),bm=7+(i%2),fi=(i%3)+4,ew=75+(i*3),ay=2+(i%4);duck_market(lc,bl,bw,bm,fi,ew,ay);}
ps("\n");duck_report();duck_state();ps("\n=== Demo Complete ===\n");return 0;}
