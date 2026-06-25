/* oribi_admin: Oribi (Ourebia ourebi) small graceful antelope (v1.0)
 * Oribi grassland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, jump_cm, or_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,jp_cm,or_idx,age_yr,active;} orib_t;
typedef struct{int n_grass,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_jp,t_or;} orib_state_t;
static orib_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static orib_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(orib_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int jc,int oi,int ay){if(*cnt>=mx)return -1;orib_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->jp_cm=jc;x->or_idx=oi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ORIB] Oribi ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" jc=");pi(jc);ps(" oi=");pi(oi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int oribi_init(void){if(init)return -1;st.n_grass=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_jp=0;st.t_or=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ORIB] Oribi initialized\n");return 0;}
int oribi_grassland(int lc,int bl,int bw,int ec,int jc,int oi,int ay){return add(svl,&st.n_grass,&st.t_ln,N,lc,bl,bw,ec,jc,oi,ay);}
int oribi_feeding(int lc,int bl,int bw,int ec,int jc,int oi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,jc,oi,ay);}
int oribi_breeding(int lc,int bl,int bw,int ec,int jc,int oi,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,jc,oi,ay);}
int oribi_health(int lc,int bl,int bw,int ec,int jc,int oi,int ay){return add(svh,&st.n_health,&st.t_jp,N-6,lc,bl,bw,ec,jc,oi,ay);}
int oribi_market(int lc,int bl,int bw,int ec,int jc,int oi,int ay){return add(svm,&st.n_mkt,&st.t_or,N-6,lc,bl,bw,ec,jc,oi,ay);}
void oribi_report(void){ps("[ORIB] Grass: ");pi(st.n_grass);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Or=");pi(st.t_or);ps("\n");}
void oribi_state(void){ps("[ORIB] Grass=");pi(st.n_grass);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Oribi Admin Demo ===\n\n");oribi_init();
ps("Oribi grassland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*4),bw=15+(i*2),ec=7+(i*1),jc=100+(i*8),oi=(i%8)+1,ay=(i%10)+1;oribi_grassland(lc,bl,bw,ec,jc,oi,ay);}
ps("\nOribi feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=82+(i*3),bw=16+(i*1),ec=8+(i*1),jc=105+(i*6),oi=(i%6)+1,ay=(i%8)+1;oribi_feeding(lc,bl,bw,ec,jc,oi,ay);}
ps("\nOribi breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=85+(i*3),bw=17+(i*1),ec=9+(i*1),jc=110+(i*5),oi=(i%5)+1,ay=(i%7)+1;oribi_breeding(lc,bl,bw,ec,jc,oi,ay);}
ps("\nOribi health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=78+(i*5),bw=14+(i*3),ec=6+(i*1),jc=98+(i*10),oi=(i%10)+1,ay=(i%6)+1;oribi_health(lc,bl,bw,ec,jc,oi,ay);}
ps("\nOribi market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=88+(i*2),bw=18+(i*1),ec=10+(i*1),jc=115+(i*4),oi=(i%4)+1,ay=(i%5)+1;oribi_market(lc,bl,bw,ec,jc,oi,ay);}
ps("\n");oribi_report();oribi_state();ps("\n=== Demo Complete ===\n");return 0;}
