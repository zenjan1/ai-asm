/* blesbok_admin: Blesbok (Damaliscus pygargus) white-fronted antelope (v1.0)
 * Blesbok grassland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, jump_cm, bl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,jp_cm,bl_idx,age_yr,active;} bles_t;
typedef struct{int n_grass,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_jp,t_bl;} bles_state_t;
static bles_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static bles_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bles_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int jc,int bi,int ay){if(*cnt>=mx)return -1;bles_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->jp_cm=jc;x->bl_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BLES] Blesbok ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" jc=");pi(jc);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int blesbok_init(void){if(init)return -1;st.n_grass=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_jp=0;st.t_bl=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BLES] Blesbok initialized\n");return 0;}
int blesbok_grassland(int lc,int bl,int bw,int hc,int jc,int bi,int ay){return add(svl,&st.n_grass,&st.t_ln,N,lc,bl,bw,hc,jc,bi,ay);}
int blesbok_feeding(int lc,int bl,int bw,int hc,int jc,int bi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,jc,bi,ay);}
int blesbok_breeding(int lc,int bl,int bw,int hc,int jc,int bi,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,jc,bi,ay);}
int blesbok_health(int lc,int bl,int bw,int hc,int jc,int bi,int ay){return add(svh,&st.n_health,&st.t_jp,N-6,lc,bl,bw,hc,jc,bi,ay);}
int blesbok_market(int lc,int bl,int bw,int hc,int jc,int bi,int ay){return add(svm,&st.n_mkt,&st.t_bl,N-6,lc,bl,bw,hc,jc,bi,ay);}
void blesbok_report(void){ps("[BLES] Grass: ");pi(st.n_grass);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Bl=");pi(st.t_bl);ps("\n");}
void blesbok_state(void){ps("[BLES] Grass=");pi(st.n_grass);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Blesbok Admin Demo ===\n\n");blesbok_init();
ps("Blesbok grassland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=130+(i*7),bw=60+(i*8),hc=45+(i*3),jc=120+(i*8),bi=(i%8)+1,ay=(i%12)+1;blesbok_grassland(lc,bl,bw,hc,jc,bi,ay);}
ps("\nBlesbok feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=135+(i*5),bw=63+(i*6),hc=47+(i*2),jc=125+(i*6),bi=(i%6)+1,ay=(i%10)+1;blesbok_feeding(lc,bl,bw,hc,jc,bi,ay);}
ps("\nBlesbok breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=140+(i*4),bw=66+(i*5),hc=49+(i*3),jc=130+(i*5),bi=(i%5)+1,ay=(i%8)+1;blesbok_breeding(lc,bl,bw,hc,jc,bi,ay);}
ps("\nBlesbok health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=128+(i*9),bw=58+(i*10),hc=43+(i*4),jc=118+(i*10),bi=(i%10)+1,ay=(i%7)+1;blesbok_health(lc,bl,bw,hc,jc,bi,ay);}
ps("\nBlesbok market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=145+(i*3),bw=68+(i*4),hc=51+(i*2),jc=135+(i*4),bi=(i%4)+1,ay=(i%6)+1;blesbok_market(lc,bl,bw,hc,jc,bi,ay);}
ps("\n");blesbok_report();blesbok_state();ps("\n=== Demo Complete ===\n");return 0;}
