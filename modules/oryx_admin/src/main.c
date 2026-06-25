/* oryx_admin: Oryx (Oryx gazella) gemsbok large desert antelope (v1.0)
 * Oryx desert, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, or_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,rn_sp,or_idx,age_yr,active;} oryx_t;
typedef struct{int n_desert,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_rn,t_or;} oryx_state_t;
static oryx_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static oryx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oryx_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int oi,int ay){if(*cnt>=mx)return -1;oryx_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->rn_sp=rs;x->or_idx=oi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ORYX] Oryx ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" oi=");pi(oi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int oryx_init(void){if(init)return -1;st.n_desert=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_rn=0;st.t_or=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ORYX] Oryx initialized\n");return 0;}
int oryx_desert(int lc,int bl,int bw,int hc,int rs,int oi,int ay){return add(svl,&st.n_desert,&st.t_ln,N,lc,bl,bw,hc,rs,oi,ay);}
int oryx_feeding(int lc,int bl,int bw,int hc,int rs,int oi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,oi,ay);}
int oryx_breeding(int lc,int bl,int bw,int hc,int rs,int oi,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,rs,oi,ay);}
int oryx_health(int lc,int bl,int bw,int hc,int rs,int oi,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,oi,ay);}
int oryx_market(int lc,int bl,int bw,int hc,int rs,int oi,int ay){return add(svm,&st.n_mkt,&st.t_or,N-6,lc,bl,bw,hc,rs,oi,ay);}
void oryx_report(void){ps("[ORYX] Desert: ");pi(st.n_desert);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Or=");pi(st.t_or);ps("\n");}
void oryx_state(void){ps("[ORYX] Desert=");pi(st.n_desert);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Oryx Admin Demo ===\n\n");oryx_init();
ps("Oryx desert...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=170+(i*8),bw=180+(i*15),hc=90+(i*5),rs=28+(i*3),oi=(i%8)+1,ay=(i%15)+1;oryx_desert(lc,bl,bw,hc,rs,oi,ay);}
ps("\nOryx feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=175+(i*6),bw=185+(i*12),hc=92+(i*4),rs=30+(i*2),oi=(i%6)+1,ay=(i%12)+1;oryx_feeding(lc,bl,bw,hc,rs,oi,ay);}
ps("\nOryx breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=180+(i*5),bw=190+(i*10),hc=95+(i*4),rs=27+(i*3),oi=(i%5)+1,ay=(i%10)+1;oryx_breeding(lc,bl,bw,hc,rs,oi,ay);}
ps("\nOryx health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=168+(i*10),bw=175+(i*18),hc=88+(i*6),rs=25+(i*4),oi=(i%10)+1,ay=(i%8)+1;oryx_health(lc,bl,bw,hc,rs,oi,ay);}
ps("\nOryx market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=185+(i*4),bw=195+(i*8),hc=98+(i*3),rs=32+(i*2),oi=(i%4)+1,ay=(i%7)+1;oryx_market(lc,bl,bw,hc,rs,oi,ay);}
ps("\n");oryx_report();oryx_state();ps("\n=== Demo Complete ===\n");return 0;}
