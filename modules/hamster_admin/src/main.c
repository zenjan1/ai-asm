/* hamster_admin: Hamster (Mesocricetus auratus) golden hamster rodent (v1.0)
 * Hamster housing, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, cheek_pouch_idx, wheel_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,chk_idx,wh_ct,age_yr,active;} hamster_t;
typedef struct{int n_hous,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_chk,t_wh;} hamster_state_t;
static hamster_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static hamster_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hamster_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ci,int wc,int ay){if(*cnt>=mx)return -1;hamster_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->chk_idx=ci;x->wh_ct=wc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HAM] Hamster ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ci=");pi(ci);ps(" wc=");pi(wc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hamster_init(void){if(init)return -1;st.n_hous=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_chk=0;st.t_wh=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HAM] Hamster initialized\n");return 0;}
int hamster_housing(int lc,int bl,int bw,int tc,int ci,int wc,int ay){return add(gpl,&st.n_hous,&st.t_ln,N,lc,bl,bw,tc,ci,wc,ay);}
int hamster_feeding(int lc,int bl,int bw,int tc,int ci,int wc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ci,wc,ay);}
int hamster_breeding(int lc,int bl,int bw,int tc,int ci,int wc,int ay){return add(gbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,ci,wc,ay);}
int hamster_health(int lc,int bl,int bw,int tc,int ci,int wc,int ay){return add(ghl,&st.n_health,&st.t_chk,N-6,lc,bl,bw,tc,ci,wc,ay);}
int hamster_market(int lc,int bl,int bw,int tc,int ci,int wc,int ay){return add(gmk,&st.n_mkt,&st.t_wh,N-6,lc,bl,bw,tc,ci,wc,ay);}
void hamster_report(void){ps("[HAM] Hous: ");pi(st.n_hous);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHlth: ");pi(st.n_health);ps(" Chk=");pi(st.t_chk);ps("\nMkt: ");pi(st.n_mkt);ps(" Wh=");pi(st.t_wh);ps("\n");}
void hamster_state(void){ps("[HAM] Hous=");pi(st.n_hous);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hamster Admin Demo ===\n\n");hamster_init();
/* 1=cage 2=wheel 3=tube 4=bedding 5=home */
ps("Hamster housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*2),bw=80+(i*10),tc=2+(i%3),ci=(i%5)+1,wc=50+(i*20),ay=(i%3)+1;hamster_housing(lc,bl,bw,tc,ci,wc,ay);}
ps("\nHamster feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=11+(i*2),bw=85+(i*8),tc=2+(i%3),ci=(i%4)+1,wc=55+(i*15),ay=(i%3)+1;hamster_feeding(lc,bl,bw,tc,ci,wc,ay);}
ps("\nHamster breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=9+(i*3),bw=75+(i*12),tc=1+(i%4),ci=(i%3)+2,wc=45+(i*18),ay=(i%3)+1;hamster_breeding(lc,bl,bw,tc,ci,wc,ay);}
ps("\nHamster health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=12+(i*2),bw=90+(i*7),tc=3+(i%2),ci=(i%5)+1,wc=60+(i*12),ay=(i%4)+1;hamster_health(lc,bl,bw,tc,ci,wc,ay);}
ps("\nHamster market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=14+(i*2),bw=95+(i*6),tc=3+(i%2),ci=(i%4)+1,wc=65+(i*10),ay=(i%3)+1;hamster_market(lc,bl,bw,tc,ci,wc,ay);}
ps("\n");hamster_report();hamster_state();ps("\n=== Demo Complete ===\n");return 0;}
