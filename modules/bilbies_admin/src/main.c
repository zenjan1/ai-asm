/* bilbies_admin: Bilbies (Macrotis) rabbit-eared bandicoot marsupials (v1.0)
 * Bilbies desert, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, dig_speed, bl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,dg_sp,bl_idx,age_yr,active;} blbs_t;
typedef struct{int n_desert,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_dg,t_bl;} blbs_state_t;
static blbs_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static blbs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blbs_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int ds,int bi,int ay){if(*cnt>=mx)return -1;blbs_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->dg_sp=ds;x->bl_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BLBS] Bilby ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" ds=");pi(ds);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bilbies_init(void){if(init)return -1;st.n_desert=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_dg=0;st.t_bl=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BLBS] Bilbies initialized\n");return 0;}
int bilbies_desert(int lc,int bl,int bw,int ec,int ds,int bi,int ay){return add(svl,&st.n_desert,&st.t_ln,N,lc,bl,bw,ec,ds,bi,ay);}
int bilbies_feeding(int lc,int bl,int bw,int ec,int ds,int bi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,ds,bi,ay);}
int bilbies_breeding(int lc,int bl,int bw,int ec,int ds,int bi,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,ds,bi,ay);}
int bilbies_health(int lc,int bl,int bw,int ec,int ds,int bi,int ay){return add(svh,&st.n_health,&st.t_dg,N-6,lc,bl,bw,ec,ds,bi,ay);}
int bilbies_market(int lc,int bl,int bw,int ec,int ds,int bi,int ay){return add(svm,&st.n_mkt,&st.t_bl,N-6,lc,bl,bw,ec,ds,bi,ay);}
void bilbies_report(void){ps("[BLBS] Desert: ");pi(st.n_desert);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Dg=");pi(st.t_dg);ps("\nMkt: ");pi(st.n_mkt);ps(" Bl=");pi(st.t_bl);ps("\n");}
void bilbies_state(void){ps("[BLBS] Desert=");pi(st.n_desert);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bilbies Admin Demo ===\n\n");bilbies_init();
ps("Bilbies desert...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=1+(i*1),ec=8+(i*1),ds=5+(i*1),bi=(i%8)+1,ay=(i%8)+1;bilbies_desert(lc,bl,bw,ec,ds,bi,ay);}
ps("\nBilbies feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=1+(i*1),ec=9+(i*1),ds=6+(i*1),bi=(i%6)+1,ay=(i%7)+1;bilbies_feeding(lc,bl,bw,ec,ds,bi,ay);}
ps("\nBilbies breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=44+(i*2),bw=2+(i*1),ec=9+(i*1),ds=4+(i*1),bi=(i%5)+1,ay=(i%6)+1;bilbies_breeding(lc,bl,bw,ec,ds,bi,ay);}
ps("\nBilbies health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=1+(i*1),ec=7+(i*1),ds=3+(i*2),bi=(i%10)+1,ay=(i%5)+1;bilbies_health(lc,bl,bw,ec,ds,bi,ay);}
ps("\nBilbies market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=46+(i*1),bw=2+(i*1),ec=10+(i*1),ds=7+(i*1),bi=(i%4)+1,ay=(i%4)+1;bilbies_market(lc,bl,bw,ec,ds,bi,ay);}
ps("\n");bilbies_report();bilbies_state();ps("\n=== Demo Complete ===\n");return 0;}
