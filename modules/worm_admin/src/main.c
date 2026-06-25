/* worm_admin: Worm (Lumbricus terrestris) earthworm (v1.0)
 * Worm soil, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, segment_ct, burrow_speed, ring_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,seg_ct,br_sp,rg_idx,age_yr,active;} worm_t;
typedef struct{int n_soil,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_seg,t_br,t_rg;} worm_state_t;
static worm_t wpl[N],wfd[N-2],wbr[N-4],whl[N-6],wmk[N-6]; static worm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(worm_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int bs,int ri,int ay){if(*cnt>=mx)return -1;worm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->seg_ct=sc;x->br_sp=bs;x->rg_idx=ri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WORM] Worm ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" bs=");pi(bs);ps(" ri=");pi(ri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int worm_init(void){if(init)return -1;st.n_soil=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_seg=0;st.t_br=0;st.t_rg=0;for(int i=0;i<N;i++)wpl[i].active=0;for(int i=0;i<N-2;i++)wfd[i].active=0;for(int i=0;i<N-4;i++)wbr[i].active=0;for(int i=0;i<N-6;i++)whl[i].active=0;for(int i=0;i<N-6;i++)wmk[i].active=0;init=1;ps("[WORM] Worm initialized\n");return 0;}
int worm_soil(int lc,int bl,int bw,int sc,int bs,int ri,int ay){return add(wpl,&st.n_soil,&st.t_ln,N,lc,bl,bw,sc,bs,ri,ay);}
int worm_feeding(int lc,int bl,int bw,int sc,int bs,int ri,int ay){return add(wfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,bs,ri,ay);}
int worm_breeding(int lc,int bl,int bw,int sc,int bs,int ri,int ay){return add(wbr,&st.n_breed,&st.t_seg,N-4,lc,bl,bw,sc,bs,ri,ay);}
int worm_health(int lc,int bl,int bw,int sc,int bs,int ri,int ay){return add(whl,&st.n_health,&st.t_br,N-6,lc,bl,bw,sc,bs,ri,ay);}
int worm_market(int lc,int bl,int bw,int sc,int bs,int ri,int ay){return add(wmk,&st.n_mkt,&st.t_rg,N-6,lc,bl,bw,sc,bs,ri,ay);}
void worm_report(void){ps("[WORM] Soil: ");pi(st.n_soil);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Seg=");pi(st.t_seg);ps("\nHealth: ");pi(st.n_health);ps(" Br=");pi(st.t_br);ps("\nMkt: ");pi(st.n_mkt);ps(" Rg=");pi(st.t_rg);ps("\n");}
void worm_state(void){ps("[WORM] Soil=");pi(st.n_soil);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Worm Admin Demo ===\n\n");worm_init();
ps("Worm soil...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*2),bw=2+(i*2),sc=50+(i*10),bs=0+(i%3),rg=(i%6)+1,ay=(i%2)+1;worm_soil(lc,bl,bw,sc,bs,rg,ay);}
ps("\nWorm feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=12+(i*2),bw=3+(i),sc=55+(i*8),bs=1+(i%2),rg=(i%5)+1,ay=(i%2)+1;worm_feeding(lc,bl,bw,sc,bs,rg,ay);}
ps("\nWorm breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=8+(i*3),bw=1+(i*2),sc=45+(i*12),bs=0+(i%3),rg=(i%4)+1,ay=(i%2)+1;worm_breeding(lc,bl,bw,sc,bs,rg,ay);}
ps("\nWorm health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i),bw=4+(i),sc=60+(i*6),bs=0+(i%2),rg=(i%7)+1,ay=(i%3)+2;worm_health(lc,bl,bw,sc,bs,rg,ay);}
ps("\nWorm market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=16+(i%3),bw=5+(i%2),sc=65+(i*5),bs=0+(i%3),rg=(i%3)+3,ay=(i%2)+2;worm_market(lc,bl,bw,sc,bs,rg,ay);}
ps("\n");worm_report();worm_state();ps("\n=== Demo Complete ===\n");return 0;}
