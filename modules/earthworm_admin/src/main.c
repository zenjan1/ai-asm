/* earthworm_admin: Earthworm (Lumbricus terrestris) soil invertebrate farming (v1.0)
 * Earthworm housing, feeding, breeding, harvest, market
 * Features: body_len_cm, body_wt_g, segment_ct, skin_idx, cast_wt_g, age_day
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,seg_ct,sk_idx,cst_wt,age_dy,active;} eart_t;
typedef struct{int n_house,n_feed,n_breed,n_harv,n_mkt,t_ln,t_wt,t_sg,t_sk,t_cs;} eart_state_t;
static eart_t ehs[N],efd[N-2],ebr[N-4],ehv[N-6],emk[N-6]; static eart_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eart_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int si,int cw,int ad){if(*cnt>=mx)return -1;eart_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->seg_ct=sc;x->sk_idx=si;x->cst_wt=cw;x->age_dy=ad;x->active=1;*sum+=bl;(*cnt)++;ps("[EART] Earthworm ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" si=");pi(si);ps(" cw=");pi(cw);ps(" ad=");pi(ad);ps("\n");return *cnt-1;}
int eart_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_sg=0;st.t_sk=0;st.t_cs=0;for(int i=0;i<N;i++)ehs[i].active=0;for(int i=0;i<N-2;i++)efd[i].active=0;for(int i=0;i<N-4;i++)ebr[i].active=0;for(int i=0;i<N-6;i++)ehv[i].active=0;for(int i=0;i<N-6;i++)emk[i].active=0;init=1;ps("[EART] Earthworm initialized\n");return 0;}
int eart_housing(int lc,int bl,int bw,int sc,int si,int cw,int ad){return add(ehs,&st.n_house,&st.t_ln,N,lc,bl,bw,sc,si,cw,ad);}
int eart_feeding(int lc,int bl,int bw,int sc,int si,int cw,int ad){return add(efd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,si,cw,ad);}
int eart_breeding(int lc,int bl,int bw,int sc,int si,int cw,int ad){return add(ebr,&st.n_breed,&st.t_sg,N-4,lc,bl,bw,sc,si,cw,ad);}
int eart_harvest(int lc,int bl,int bw,int sc,int si,int cw,int ad){return add(ehv,&st.n_harv,&st.t_sk,N-6,lc,bl,bw,sc,si,cw,ad);}
int eart_market(int lc,int bl,int bw,int sc,int si,int cw,int ad){return add(emk,&st.n_mkt,&st.t_cs,N-6,lc,bl,bw,sc,si,cw,ad);}
void eart_report(void){ps("[EART] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Sg=");pi(st.t_sg);ps("\nHarv: ");pi(st.n_harv);ps(" Sk=");pi(st.t_sk);ps("\nMkt: ");pi(st.n_mkt);ps(" Cs=");pi(st.t_cs);ps("\n");}
void eart_state(void){ps("[EART] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Earthworm Admin Demo ===\n\n");eart_init();
/* 1=bin 2=bed 3=compost 4=field 5=market */
ps("Earthworm housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*2),bw=1+(i%3),sc=50+(i*10),si=(i%5)+1,cw=2+(i*3),ad=5+(i*5);eart_housing(lc,bl,bw,sc,si,cw,ad);}
ps("\nEarthworm feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=12+(i%2),bw=2+(i%2),sc=55+(i*8),si=(i%4)+1,cw=3+(i*2),ad=8+(i*4);eart_feeding(lc,bl,bw,sc,si,cw,ad);}
ps("\nEarthworm breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=11+(i*2),bw=1+(i%3),sc=48+(i*12),si=(i%6)+1,cw=2+(i*4),ad=10+(i*3);eart_breeding(lc,bl,bw,sc,si,cw,ad);}
ps("\nEarthworm harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=13+(i%2),bw=2+(i%2),sc=60+(i*7),si=(i%5)+1,cw=4+(i*3),ad=15+(i*4);eart_harvest(lc,bl,bw,sc,si,cw,ad);}
ps("\nEarthworm market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=14+(i%2),bw=2+(i%2),sc=65+(i*6),si=(i%3)+4,cw=5+(i*2),ad=20+(i*3);eart_market(lc,bl,bw,sc,si,cw,ad);}
ps("\n");eart_report();eart_state();ps("\n=== Demo Complete ===\n");return 0;}
