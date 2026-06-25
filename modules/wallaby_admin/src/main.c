/* wallaby_admin: Wallaby (Macropus rufogriseus) small macropod marsupial (v1.0)
 * Wallaby bush, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, jump_cm, hr_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,jp_cm,hr_idx,age_yr,active;} wall_t;
typedef struct{int n_bush,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_jp,t_hr;} wall_state_t;
static wall_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static wall_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wall_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int jc,int hi,int ay){if(*cnt>=mx)return -1;wall_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->jp_cm=jc;x->hr_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WALL] Wallaby ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" jc=");pi(jc);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int wallaby_init(void){if(init)return -1;st.n_bush=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_jp=0;st.t_hr=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[WALL] Wallaby initialized\n");return 0;}
int wallaby_bush(int lc,int bl,int bw,int ec,int jc,int hi,int ay){return add(svl,&st.n_bush,&st.t_ln,N,lc,bl,bw,ec,jc,hi,ay);}
int wallaby_feeding(int lc,int bl,int bw,int ec,int jc,int hi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,jc,hi,ay);}
int wallaby_breeding(int lc,int bl,int bw,int ec,int jc,int hi,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,jc,hi,ay);}
int wallaby_health(int lc,int bl,int bw,int ec,int jc,int hi,int ay){return add(svh,&st.n_health,&st.t_jp,N-6,lc,bl,bw,ec,jc,hi,ay);}
int wallaby_market(int lc,int bl,int bw,int ec,int jc,int hi,int ay){return add(svm,&st.n_mkt,&st.t_hr,N-6,lc,bl,bw,ec,jc,hi,ay);}
void wallaby_report(void){ps("[WALL] Bush: ");pi(st.n_bush);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Hr=");pi(st.t_hr);ps("\n");}
void wallaby_state(void){ps("[WALL] Bush=");pi(st.n_bush);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Wallaby Admin Demo ===\n\n");wallaby_init();
ps("Wallaby bush...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*4),bw=12+(i*2),ec=10+(i*1),jc=80+(i*6),hi=(i%8)+1,ay=(i%12)+1;wallaby_bush(lc,bl,bw,ec,jc,hi,ay);}
ps("\nWallaby feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=62+(i*3),bw=13+(i*2),ec=11+(i*1),jc=85+(i*5),hi=(i%6)+1,ay=(i%10)+1;wallaby_feeding(lc,bl,bw,ec,jc,hi,ay);}
ps("\nWallaby breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=65+(i*3),bw=14+(i*2),ec=12+(i*1),jc=90+(i*4),hi=(i%5)+1,ay=(i%8)+1;wallaby_breeding(lc,bl,bw,ec,jc,hi,ay);}
ps("\nWallaby health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=58+(i*5),bw=11+(i*3),ec=9+(i*2),jc=78+(i*8),hi=(i%10)+1,ay=(i%6)+1;wallaby_health(lc,bl,bw,ec,jc,hi,ay);}
ps("\nWallaby market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=68+(i*2),bw=15+(i*1),ec=13+(i*1),jc=95+(i*3),hi=(i%4)+1,ay=(i%6)+1;wallaby_market(lc,bl,bw,ec,jc,hi,ay);}
ps("\n");wallaby_report();wallaby_state();ps("\n=== Demo Complete ===\n");return 0;}
