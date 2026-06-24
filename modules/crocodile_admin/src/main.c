/* crocodile_admin: Crocodile (Crocodylus niloticus) reptile farming (v1.0)
 * Crocodile housing, feeding, breeding, veterinary, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, tooth_count, hide_quality, health_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,tooth_ct,hide_ql,hlth_idx,active;} crco_t;
typedef struct{int n_house,n_feed,n_breed,n_vet,n_mkt,t_ln,t_wt,t_tail,t_tooth,t_hide;} crco_state_t;
static crco_t crcohs[N],crcofd[N-2],crcobr[N-4],crcovt[N-6],crcomk[N-6]; static crco_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crco_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int tc,int hq,int hi){if(*cnt>=mx)return -1;crco_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->tooth_ct=tc;x->hide_ql=hq;x->hlth_idx=hi;x->active=1;*sum+=bl;(*cnt)++;ps("[CRCO] Crocodile ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" tc=");pi(tc);ps(" hq=");pi(hq);ps(" hi=");pi(hi);ps("\n");return *cnt-1;}
int crco_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_vet=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_tooth=0;st.t_hide=0;for(int i=0;i<N;i++)crcohs[i].active=0;for(int i=0;i<N-2;i++)crcofd[i].active=0;for(int i=0;i<N-4;i++)crcobr[i].active=0;for(int i=0;i<N-6;i++)crcovt[i].active=0;for(int i=0;i<N-6;i++)crcomk[i].active=0;init=1;ps("[CRCO] Crocodile initialized\n");return 0;}
int crco_housing(int lc,int bl,int bw,int tl,int tc,int hq,int hi){return add(crcohs,&st.n_house,&st.t_ln,N,lc,bl,bw,tl,tc,hq,hi);}
int crco_feeding(int lc,int bl,int bw,int tl,int tc,int hq,int hi){return add(crcofd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,tc,hq,hi);}
int crco_breeding(int lc,int bl,int bw,int tl,int tc,int hq,int hi){return add(crcobr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,tc,hq,hi);}
int crco_veterinary(int lc,int bl,int bw,int tl,int tc,int hq,int hi){return add(crcovt,&st.n_vet,&st.t_tooth,N-6,lc,bl,bw,tl,tc,hq,hi);}
int crco_market(int lc,int bl,int bw,int tl,int tc,int hq,int hi){return add(crcomk,&st.n_mkt,&st.t_hide,N-6,lc,bl,bw,tl,tc,hq,hi);}
void crco_report(void){ps("[CRCO] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nVet: ");pi(st.n_vet);ps(" Tooth=");pi(st.t_tooth);ps("\nMkt: ");pi(st.n_mkt);ps(" Hide=");pi(st.t_hide);ps("\n");}
void crco_state(void){ps("[CRCO] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Vet=");pi(st.n_vet);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Crocodile Admin Demo ===\n\n");crco_init();
/* 1=pond 2=farm 3=sanctuary 4=wild 5=market */
ps("Crocodile housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*20),bw=100+(i*30),tl=80+(i*10),tc=60+(i*4),hq=(i%8)+1,hi=7+(i%4);crco_housing(lc,bl,bw,tl,tc,hq,hi);}
ps("\nCrocodile feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*18),bw=110+(i*25),tl=85+(i*9),tc=62+(i*3),hq=(i%7)+1,hi=7+(i%3);crco_feeding(lc,bl,bw,tl,tc,hq,hi);}
ps("\nCrocodile breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*15),bw=120+(i*20),tl=90+(i*8),tc=64+(i*3),hq=(i%6)+1,hi=8+(i%3);crco_breeding(lc,bl,bw,tl,tc,hq,hi);}
ps("\nCrocodile veterinary...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=190+(i*25),bw=90+(i*35),tl=75+(i*12),tc=58+(i*5),hq=(i%9)+1,hi=6+(i%5);crco_veterinary(lc,bl,bw,tl,tc,hq,hi);}
ps("\nCrocodile market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*12),bw=130+(i*18),tl=95+(i*7),tc=66+(i*2),hq=(i%4)+5,hi=9+(i%2);crco_market(lc,bl,bw,tl,tc,hq,hi);}
ps("\n");crco_report();crco_state();ps("\n=== Demo Complete ===\n");return 0;}
