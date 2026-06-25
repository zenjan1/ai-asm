/* onager_admin: Onager (Equus hemionus) Central Asian wild ass (v1.0)
 * Onager desert, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, gallop_speed, on_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,gl_sp,on_idx,age_yr,active;} onag_t;
typedef struct{int n_desert,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_gl,t_on;} onag_state_t;
static onag_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static onag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(onag_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int gs,int oi,int ay){if(*cnt>=mx)return -1;onag_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->gl_sp=gs;x->on_idx=oi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ONAG] Onager ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" gs=");pi(gs);ps(" oi=");pi(oi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int onager_init(void){if(init)return -1;st.n_desert=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_gl=0;st.t_on=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ONAG] Onager initialized\n");return 0;}
int onager_desert(int lc,int bl,int bw,int ec,int gs,int oi,int ay){return add(svl,&st.n_desert,&st.t_ln,N,lc,bl,bw,ec,gs,oi,ay);}
int onager_feeding(int lc,int bl,int bw,int ec,int gs,int oi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,gs,oi,ay);}
int onager_breeding(int lc,int bl,int bw,int ec,int gs,int oi,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,gs,oi,ay);}
int onager_health(int lc,int bl,int bw,int ec,int gs,int oi,int ay){return add(svh,&st.n_health,&st.t_gl,N-6,lc,bl,bw,ec,gs,oi,ay);}
int onager_market(int lc,int bl,int bw,int ec,int gs,int oi,int ay){return add(svm,&st.n_mkt,&st.t_on,N-6,lc,bl,bw,ec,gs,oi,ay);}
void onager_report(void){ps("[ONAG] Desert: ");pi(st.n_desert);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Gl=");pi(st.t_gl);ps("\nMkt: ");pi(st.n_mkt);ps(" On=");pi(st.t_on);ps("\n");}
void onager_state(void){ps("[ONAG] Desert=");pi(st.n_desert);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Onager Admin Demo ===\n\n");onager_init();
ps("Onager desert...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*9),bw=220+(i*18),ec=12+(i*1),gs=30+(i*3),oi=(i%8)+1,ay=(i%18)+1;onager_desert(lc,bl,bw,ec,gs,oi,ay);}
ps("\nOnager feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=205+(i*7),bw=230+(i*14),ec=13+(i*1),gs=32+(i*2),oi=(i%6)+1,ay=(i%15)+1;onager_feeding(lc,bl,bw,ec,gs,oi,ay);}
ps("\nOnager breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=210+(i*5),bw=240+(i*11),ec=14+(i*1),gs=28+(i*3),oi=(i%5)+1,ay=(i%12)+1;onager_breeding(lc,bl,bw,ec,gs,oi,ay);}
ps("\nOnager health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*11),bw=210+(i*22),ec=11+(i*2),gs=26+(i*4),oi=(i%10)+1,ay=(i%10)+1;onager_health(lc,bl,bw,ec,gs,oi,ay);}
ps("\nOnager market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=218+(i*4),bw=250+(i*9),ec=15+(i*1),gs=34+(i*2),oi=(i%4)+1,ay=(i%8)+1;onager_market(lc,bl,bw,ec,gs,oi,ay);}
ps("\n");onager_report();onager_state();ps("\n=== Demo Complete ===\n");return 0;}
