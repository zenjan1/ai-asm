/* crane_admin: Crane (Grus grus) wading bird husbandry (v1.0)
 * Crane housing, feeding, breeding, grooming, market
 * Features: body_ht_cm, body_wt_g, wing_span_cm, neck_len_cm, egg_count, feather_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ht,bdy_wt,wing_sp,neck_ln,eg_ct,fthr_idx,active;} crnb_t;
typedef struct{int n_house,n_feed,n_breed,n_groom,n_mkt,t_ht,t_wt,t_wing,t_neck,t_egg;} crnb_state_t;
static crnb_t crnbh[N],crnbf[N-2],crnbr[N-4],crnbg[N-6],crnbm[N-6]; static crnb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crnb_t*a,int*cnt,int*sum,int mx,int lc,int bh,int bw,int ws,int nl,int ec,int fi){if(*cnt>=mx)return -1;crnb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ht=bh;x->bdy_wt=bw;x->wing_sp=ws;x->neck_ln=nl;x->eg_ct=ec;x->fthr_idx=fi;x->active=1;*sum+=bh;(*cnt)++;ps("[CRNB] Crane ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bh=");pi(bh);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" nl=");pi(nl);ps(" ec=");pi(ec);ps(" fi=");pi(fi);ps("\n");return *cnt-1;}
int crnb_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_groom=0;st.n_mkt=0;st.t_ht=0;st.t_wt=0;st.t_wing=0;st.t_neck=0;st.t_egg=0;for(int i=0;i<N;i++)crnbh[i].active=0;for(int i=0;i<N-2;i++)crnbf[i].active=0;for(int i=0;i<N-4;i++)crnbr[i].active=0;for(int i=0;i<N-6;i++)crnbg[i].active=0;for(int i=0;i<N-6;i++)crnbm[i].active=0;init=1;ps("[CRNB] Crane initialized\n");return 0;}
int crnb_housing(int lc,int bh,int bw,int ws,int nl,int ec,int fi){return add(crnbh,&st.n_house,&st.t_ht,N,lc,bh,bw,ws,nl,ec,fi);}
int crnb_feeding(int lc,int bh,int bw,int ws,int nl,int ec,int fi){return add(crnbf,&st.n_feed,&st.t_wt,N-2,lc,bh,bw,ws,nl,ec,fi);}
int crnb_breeding(int lc,int bh,int bw,int ws,int nl,int ec,int fi){return add(crnbr,&st.n_breed,&st.t_wing,N-4,lc,bh,bw,ws,nl,ec,fi);}
int crnb_grooming(int lc,int bh,int bw,int ws,int nl,int ec,int fi){return add(crnbg,&st.n_groom,&st.t_neck,N-6,lc,bh,bw,ws,nl,ec,fi);}
int crnb_market(int lc,int bh,int bw,int ws,int nl,int ec,int fi){return add(crnbm,&st.n_mkt,&st.t_egg,N-6,lc,bh,bw,ws,nl,ec,fi);}
void crnb_report(void){ps("[CRNB] House: ");pi(st.n_house);ps(" Ht=");pi(st.t_ht);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nGroom: ");pi(st.n_groom);ps(" Neck=");pi(st.t_neck);ps("\nMkt: ");pi(st.n_mkt);ps(" Egg=");pi(st.t_egg);ps("\n");}
void crnb_state(void){ps("[CRNB] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Crane Admin Demo ===\n\n");crnb_init();
/* 1=wetland 2=lakeside 3=aviary 4=sanctuary 5=market */
ps("Crane housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bh=100+(i*8),bw=3000+(i*300),ws=180+(i*10),nl=40+(i*5),ec=(i%5),fi=(i%6)+1;crnb_housing(lc,bh,bw,ws,nl,ec,fi);}
ps("\nCrane feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bh=105+(i*7),bw=3200+(i*250),ws=185+(i*8),nl=42+(i*4),ec=(i%4),fi=(i%5)+1;crnb_feeding(lc,bh,bw,ws,nl,ec,fi);}
ps("\nCrane breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bh=110+(i*6),bw=3400+(i*200),ws=190+(i*7),nl=44+(i*4),ec=1+(i%3),fi=(i%4)+1;crnb_breeding(lc,bh,bw,ws,nl,ec,fi);}
ps("\nCrane grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bh=95+(i*10),bw=2800+(i*400),ws=175+(i*12),nl=38+(i*6),ec=(i%6),fi=(i%7)+1;crnb_grooming(lc,bh,bw,ws,nl,ec,fi);}
ps("\nCrane market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bh=115+(i*5),bw=3600+(i*150),ws=195+(i*6),nl=46+(i*3),ec=2+(i%2),fi=(i%3)+4;crnb_market(lc,bh,bw,ws,nl,ec,fi);}
ps("\n");crnb_report();crnb_state();ps("\n=== Demo Complete ===\n");return 0;}
