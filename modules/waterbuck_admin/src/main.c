/* waterbuck_admin: Waterbuck (Kobus ellipsiprymnus) large aquatic antelope (v1.0)
 * Waterbuck wetland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, swim_speed, wb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,sw_sp,wb_idx,age_yr,active;} wtrb_t;
typedef struct{int n_wet,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_sw,t_wb;} wtrb_state_t;
static wtrb_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static wtrb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wtrb_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ss,int wi,int ay){if(*cnt>=mx)return -1;wtrb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->sw_sp=ss;x->wb_idx=wi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[WTRB] Waterbuck ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ss=");pi(ss);ps(" wi=");pi(wi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int waterbuck_init(void){if(init)return -1;st.n_wet=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_sw=0;st.t_wb=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[WTRB] Waterbuck initialized\n");return 0;}
int waterbuck_wetland(int lc,int bl,int bw,int hc,int ss,int wi,int ay){return add(svl,&st.n_wet,&st.t_ln,N,lc,bl,bw,hc,ss,wi,ay);}
int waterbuck_feeding(int lc,int bl,int bw,int hc,int ss,int wi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ss,wi,ay);}
int waterbuck_breeding(int lc,int bl,int bw,int hc,int ss,int wi,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,ss,wi,ay);}
int waterbuck_health(int lc,int bl,int bw,int hc,int ss,int wi,int ay){return add(svh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,hc,ss,wi,ay);}
int waterbuck_market(int lc,int bl,int bw,int hc,int ss,int wi,int ay){return add(svm,&st.n_mkt,&st.t_wb,N-6,lc,bl,bw,hc,ss,wi,ay);}
void waterbuck_report(void){ps("[WTRB] Wet: ");pi(st.n_wet);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Wb=");pi(st.t_wb);ps("\n");}
void waterbuck_state(void){ps("[WTRB] Wet=");pi(st.n_wet);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Waterbuck Admin Demo ===\n\n");waterbuck_init();
ps("Waterbuck wetland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=180+(i*8),bw=180+(i*15),hc=70+(i*4),ss=10+(i*2),wi=(i%8)+1,ay=(i%14)+1;waterbuck_wetland(lc,bl,bw,hc,ss,wi,ay);}
ps("\nWaterbuck feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=185+(i*6),bw=185+(i*12),hc=72+(i*3),ss=11+(i*1),wi=(i%6)+1,ay=(i%12)+1;waterbuck_feeding(lc,bl,bw,hc,ss,wi,ay);}
ps("\nWaterbuck breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=190+(i*5),bw=190+(i*10),hc=75+(i*3),ss=9+(i*2),wi=(i%5)+1,ay=(i%10)+1;waterbuck_breeding(lc,bl,bw,hc,ss,wi,ay);}
ps("\nWaterbuck health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=175+(i*10),bw=175+(i*18),hc=68+(i*5),ss=8+(i*3),wi=(i%10)+1,ay=(i%8)+1;waterbuck_health(lc,bl,bw,hc,ss,wi,ay);}
ps("\nWaterbuck market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=195+(i*4),bw=195+(i*8),hc=78+(i*2),ss=12+(i*1),wi=(i%4)+1,ay=(i%7)+1;waterbuck_market(lc,bl,bw,hc,ss,wi,ay);}
ps("\n");waterbuck_report();waterbuck_state();ps("\n=== Demo Complete ===\n");return 0;}
