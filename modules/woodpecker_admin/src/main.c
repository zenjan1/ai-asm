/* woodpecker_admin: Woodpecker (Dryocopus martius) tree bird (v1.0)
 * Woodpecker woodland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, peck_speed, crest_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,pk_sp,cr_idx,age_yr,active;} wdpk_t;
typedef struct{int n_wdln,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_pk,t_cr;} wdpk_state_t;
static wdpk_t wpl[N],wfd[N-2],wbr[N-4],whl[N-6],wmk[N-6]; static wdpk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wdpk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int ps2,int ci,int ay){if(*cnt>=mx)return -1;wdpk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->pk_sp=ps2;x->cr_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WDPK] Woodpecker ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" ps=");pi(ps2);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int wdpk_init(void){if(init)return -1;st.n_wdln=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_pk=0;st.t_cr=0;for(int i=0;i<N;i++)wpl[i].active=0;for(int i=0;i<N-2;i++)wfd[i].active=0;for(int i=0;i<N-4;i++)wbr[i].active=0;for(int i=0;i<N-6;i++)whl[i].active=0;for(int i=0;i<N-6;i++)wmk[i].active=0;init=1;ps("[WDPK] Woodpecker initialized\n");return 0;}
int wdpk_woodland(int lc,int bl,int bw,int bc,int ps2,int ci,int ay){return add(wpl,&st.n_wdln,&st.t_ln,N,lc,bl,bw,bc,ps2,ci,ay);}
int wdpk_feeding(int lc,int bl,int bw,int bc,int ps2,int ci,int ay){return add(wfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,ps2,ci,ay);}
int wdpk_breeding(int lc,int bl,int bw,int bc,int ps2,int ci,int ay){return add(wbr,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,ps2,ci,ay);}
int wdpk_health(int lc,int bl,int bw,int bc,int ps2,int ci,int ay){return add(whl,&st.n_health,&st.t_pk,N-6,lc,bl,bw,bc,ps2,ci,ay);}
int wdpk_market(int lc,int bl,int bw,int bc,int ps2,int ci,int ay){return add(wmk,&st.n_mkt,&st.t_cr,N-6,lc,bl,bw,bc,ps2,ci,ay);}
void wdpk_report(void){ps("[WDPK] Woodland: ");pi(st.n_wdln);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Pk=");pi(st.t_pk);ps("\nMkt: ");pi(st.n_mkt);ps(" Cr=");pi(st.t_cr);ps("\n");}
void wdpk_state(void){ps("[WDPK] Wdln=");pi(st.n_wdln);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Woodpecker Admin Demo ===\n\n");wdpk_init();
ps("Woodpecker woodland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=200+(i*30),bc=8+(i%3),ps2=20+(i*5),cr=(i%6)+1,ay=(i%3)+1;wdpk_woodland(lc,bl,bw,bc,ps2,cr,ay);}
ps("\nWoodpecker feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=42+(i*2),bw=220+(i*25),bc=9+(i%2),ps2=22+(i*4),cr=(i%5)+1,ay=(i%3)+1;wdpk_feeding(lc,bl,bw,bc,ps2,cr,ay);}
ps("\nWoodpecker breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=38+(i*4),bw=180+(i*35),bc=7+(i%4),ps2=18+(i*6),cr=(i%4)+1,ay=(i%2)+1;wdpk_breeding(lc,bl,bw,bc,ps2,cr,ay);}
ps("\nWoodpecker health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=45+(i*2),bw=250+(i*20),bc=10+(i%2),ps2=25+(i*3),cr=(i%7)+1,ay=(i%4)+2;wdpk_health(lc,bl,bw,bc,ps2,cr,ay);}
ps("\nWoodpecker market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=48+(i),bw=280+(i*15),bc=11+(i%2),ps2=15+(i*4),cr=(i%3)+3,ay=(i%2)+2;wdpk_market(lc,bl,bw,bc,ps2,cr,ay);}
ps("\n");wdpk_report();wdpk_state();ps("\n=== Demo Complete ===\n");return 0;}
