/* woylie_admin: Woylie (Bettongia penicillata) brush-tailed bettong (v1.0)
 * Woylie burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, hind_cm, tail_str, wy_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hind_cm,tail_str,wy_idx,age_yr,active;} wylr_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hind,t_tail,t_wy;} wylr_state_t;
static wylr_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static wylr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wylr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ts,int wy,int ay){if(*cnt>=mx)return -1;wylr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hind_cm=hc;x->tail_str=ts;x->wy_idx=wy;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WYLR] Woylie ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ts=");pi(ts);ps(" wy=");pi(wy);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int woylie_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hind=0;st.t_tail=0;st.t_wy=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[WYLR] Woylie initialized\n");return 0;}
int woylie_burrow(int lc,int bl,int bw,int hc,int ts,int wy,int ay){return add(svl,&st.n_burrow,&st.t_ln,N,lc,bl,bw,hc,ts,wy,ay);}
int woylie_feeding(int lc,int bl,int bw,int hc,int ts,int wy,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ts,wy,ay);}
int woylie_breeding(int lc,int bl,int bw,int hc,int ts,int wy,int ay){return add(svb,&st.n_breed,&st.t_hind,N-4,lc,bl,bw,hc,ts,wy,ay);}
int woylie_health(int lc,int bl,int bw,int hc,int ts,int wy,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,hc,ts,wy,ay);}
int woylie_market(int lc,int bl,int bw,int hc,int ts,int wy,int ay){return add(svm,&st.n_mkt,&st.t_wy,N-6,lc,bl,bw,hc,ts,wy,ay);}
void woylie_report(void){ps("[WYLR] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hind=");pi(st.t_hind);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Wy=");pi(st.t_wy);ps("\n");}
void woylie_state(void){ps("[WYLR] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Woylie Admin Demo ===\n\n");woylie_init();
ps("Woylie burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*2),bw=1+(i*1),hc=10+(i*1),ts=5+(i%3),wy=(i%8)+1,ay=(i%5)+1;woylie_burrow(lc,bl,bw,hc,ts,wy,ay);}
ps("\nWoylie feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=27+(i*1),bw=1+(i*1),hc=11+(i%2),ts=6+(i%2),wy=(i%6)+1,ay=(i%4)+1;woylie_feeding(lc,bl,bw,hc,ts,wy,ay);}
ps("\nWoylie breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=29+(i*1),bw=2+(i*1),hc=12+(i%2),ts=6+(i%2),wy=(i%5)+1,ay=(i%4)+1;woylie_breeding(lc,bl,bw,hc,ts,wy,ay);}
ps("\nWoylie health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=23+(i*3),bw=1+(i*1),hc=9+(i%3),ts=4+(i%3),wy=(i%10)+1,ay=(i%5)+1;woylie_health(lc,bl,bw,hc,ts,wy,ay);}
ps("\nWoylie market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=31+(i*1),bw=2+(i*1),hc=13+(i%2),ts=7+(i%2),wy=(i%4)+1,ay=(i%3)+1;woylie_market(lc,bl,bw,hc,ts,wy,ay);}
ps("\n");woylie_report();woylie_state();ps("\n=== Demo Complete ===\n");return 0;}
