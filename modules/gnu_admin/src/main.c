/* gnu_admin: Gnu (Connochaetes taurinus) blue wildebeest large antelope (v1.0)
 * Gnu plains, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, gn_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,rn_sp,gn_idx,age_yr,active;} gnu_t;
typedef struct{int n_plains,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_rn,t_gn;} gnu_state_t;
static gnu_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static gnu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gnu_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int gi,int ay){if(*cnt>=mx)return -1;gnu_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->rn_sp=rs;x->gn_idx=gi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GNU] Gnu ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" gi=");pi(gi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gnu_init(void){if(init)return -1;st.n_plains=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_rn=0;st.t_gn=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[GNU] Gnu initialized\n");return 0;}
int gnu_plains(int lc,int bl,int bw,int hc,int rs,int gi,int ay){return add(svl,&st.n_plains,&st.t_ln,N,lc,bl,bw,hc,rs,gi,ay);}
int gnu_feeding(int lc,int bl,int bw,int hc,int rs,int gi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,gi,ay);}
int gnu_breeding(int lc,int bl,int bw,int hc,int rs,int gi,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,rs,gi,ay);}
int gnu_health(int lc,int bl,int bw,int hc,int rs,int gi,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,gi,ay);}
int gnu_market(int lc,int bl,int bw,int hc,int rs,int gi,int ay){return add(svm,&st.n_mkt,&st.t_gn,N-6,lc,bl,bw,hc,rs,gi,ay);}
void gnu_report(void){ps("[GNU] Plains: ");pi(st.n_plains);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Gn=");pi(st.t_gn);ps("\n");}
void gnu_state(void){ps("[GNU] Plains=");pi(st.n_plains);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gnu Admin Demo ===\n\n");gnu_init();
ps("Gnu plains...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=180+(i*10),bw=180+(i*20),hc=75+(i*5),rs=30+(i*3),gi=(i%8)+1,ay=(i%15)+1;gnu_plains(lc,bl,bw,hc,rs,gi,ay);}
ps("\nGnu feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=185+(i*8),bw=185+(i*18),hc=78+(i*4),rs=32+(i*2),gi=(i%6)+1,ay=(i%12)+1;gnu_feeding(lc,bl,bw,hc,rs,gi,ay);}
ps("\nGnu breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=190+(i*6),bw=190+(i*15),hc=80+(i*4),rs=28+(i*3),gi=(i%5)+1,ay=(i%10)+1;gnu_breeding(lc,bl,bw,hc,rs,gi,ay);}
ps("\nGnu health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=175+(i*12),bw=175+(i*25),hc=72+(i*6),rs=26+(i*4),gi=(i%10)+1,ay=(i%8)+1;gnu_health(lc,bl,bw,hc,rs,gi,ay);}
ps("\nGnu market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=195+(i*5),bw=195+(i*12),hc=83+(i*3),rs=34+(i*2),gi=(i%4)+1,ay=(i%7)+1;gnu_market(lc,bl,bw,hc,rs,gi,ay);}
ps("\n");gnu_report();gnu_state();ps("\n=== Demo Complete ===\n");return 0;}
