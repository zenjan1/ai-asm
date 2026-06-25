/* dikdik_admin: Dikdik (Madoqua kirkii) small hornless antelope (v1.0)
 * Dikdik thicket, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, run_speed, nk_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,rn_sp,nk_idx,age_yr,active;} dikd_t;
typedef struct{int n_thick,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_rn,t_nk;} dikd_state_t;
static dikd_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static dikd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dikd_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int rs,int ni,int ay){if(*cnt>=mx)return -1;dikd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->rn_sp=rs;x->nk_idx=ni;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DIKD] Dikdik ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" rs=");pi(rs);ps(" ni=");pi(ni);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dikdik_init(void){if(init)return -1;st.n_thick=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_rn=0;st.t_nk=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[DIKD] Dikdik initialized\n");return 0;}
int dikdik_thicket(int lc,int bl,int bw,int ec,int rs,int ni,int ay){return add(svl,&st.n_thick,&st.t_ln,N,lc,bl,bw,ec,rs,ni,ay);}
int dikdik_feeding(int lc,int bl,int bw,int ec,int rs,int ni,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,rs,ni,ay);}
int dikdik_breeding(int lc,int bl,int bw,int ec,int rs,int ni,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,rs,ni,ay);}
int dikdik_health(int lc,int bl,int bw,int ec,int rs,int ni,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,ec,rs,ni,ay);}
int dikdik_market(int lc,int bl,int bw,int ec,int rs,int ni,int ay){return add(svm,&st.n_mkt,&st.t_nk,N-6,lc,bl,bw,ec,rs,ni,ay);}
void dikdik_report(void){ps("[DIKD] Thick: ");pi(st.n_thick);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Nk=");pi(st.t_nk);ps("\n");}
void dikdik_state(void){ps("[DIKD] Thick=");pi(st.n_thick);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dikdik Admin Demo ===\n\n");dikdik_init();
ps("Dikdik thicket...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*3),bw=5+(i*1),ec=8+(i*1),rs=25+(i*3),ni=(i%8)+1,ay=(i%10)+1;dikdik_thicket(lc,bl,bw,ec,rs,ni,ay);}
ps("\nDikdik feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=52+(i*2),bw=5+(i*1),ec=9+(i*1),rs=27+(i*2),ni=(i%6)+1,ay=(i%8)+1;dikdik_feeding(lc,bl,bw,ec,rs,ni,ay);}
ps("\nDikdik breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=54+(i*2),bw=6+(i*1),ec=10+(i*1),rs=24+(i*3),ni=(i%5)+1,ay=(i%7)+1;dikdik_breeding(lc,bl,bw,ec,rs,ni,ay);}
ps("\nDikdik health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=48+(i*4),bw=4+(i*2),ec=7+(i*2),rs=22+(i*4),ni=(i%10)+1,ay=(i%6)+1;dikdik_health(lc,bl,bw,ec,rs,ni,ay);}
ps("\nDikdik market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=56+(i*1),bw=6+(i*1),ec=11+(i*1),rs=30+(i*2),ni=(i%4)+1,ay=(i%5)+1;dikdik_market(lc,bl,bw,ec,rs,ni,ay);}
ps("\n");dikdik_report();dikdik_state();ps("\n=== Demo Complete ===\n");return 0;}
