/* cuscus_admin: Cuscus (Phalanger) arboreal marsupial (v1.0)
 * Cuscus canopy, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, grip_kg, cs_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,grip_kg,cs_idx,age_yr,active;} cscs_t;
typedef struct{int n_canopy,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_grip,t_cs;} cscs_state_t;
static cscs_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static cscs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cscs_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int gk,int ci,int ay){if(*cnt>=mx)return -1;cscs_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->grip_kg=gk;x->cs_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CSCS] Cuscus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" gk=");pi(gk);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int cuscus_init(void){if(init)return -1;st.n_canopy=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_grip=0;st.t_cs=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[CSCS] Cuscus initialized\n");return 0;}
int cuscus_canopy(int lc,int bl,int bw,int tc,int gk,int ci,int ay){return add(svl,&st.n_canopy,&st.t_ln,N,lc,bl,bw,tc,gk,ci,ay);}
int cuscus_feeding(int lc,int bl,int bw,int tc,int gk,int ci,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,gk,ci,ay);}
int cuscus_breeding(int lc,int bl,int bw,int tc,int gk,int ci,int ay){return add(svb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,gk,ci,ay);}
int cuscus_health(int lc,int bl,int bw,int tc,int gk,int ci,int ay){return add(svh,&st.n_health,&st.t_grip,N-6,lc,bl,bw,tc,gk,ci,ay);}
int cuscus_market(int lc,int bl,int bw,int tc,int gk,int ci,int ay){return add(svm,&st.n_mkt,&st.t_cs,N-6,lc,bl,bw,tc,gk,ci,ay);}
void cuscus_report(void){ps("[CSCS] Canopy: ");pi(st.n_canopy);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Grip=");pi(st.t_grip);ps("\nMkt: ");pi(st.n_mkt);ps(" Cs=");pi(st.t_cs);ps("\n");}
void cuscus_state(void){ps("[CSCS] Canopy=");pi(st.n_canopy);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cuscus Admin Demo ===\n\n");cuscus_init();
ps("Cuscus canopy...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*2),bw=3+(i*1),tc=30+(i*2),gk=2+(i%3),ci=(i%8)+1,ay=(i%6)+1;cuscus_canopy(lc,bl,bw,tc,gk,ci,ay);}
ps("\nCuscus feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*2),bw=3+(i*1),tc=32+(i*1),gk=2+(i%2),ci=(i%6)+1,ay=(i%5)+1;cuscus_feeding(lc,bl,bw,tc,gk,ci,ay);}
ps("\nCuscus breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=39+(i*1),bw=4+(i*1),tc=34+(i*1),gk=3+(i%2),ci=(i%5)+1,ay=(i%4)+1;cuscus_breeding(lc,bl,bw,tc,gk,ci,ay);}
ps("\nCuscus health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=33+(i*3),bw=3+(i*1),tc=28+(i*2),gk=1+(i%3),ci=(i%10)+1,ay=(i%5)+1;cuscus_health(lc,bl,bw,tc,gk,ci,ay);}
ps("\nCuscus market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=41+(i*1),bw=4+(i*1),tc=36+(i*1),gk=3+(i%2),ci=(i%4)+1,ay=(i%4)+1;cuscus_market(lc,bl,bw,tc,gk,ci,ay);}
ps("\n");cuscus_report();cuscus_state();ps("\n=== Demo Complete ===\n");return 0;}
