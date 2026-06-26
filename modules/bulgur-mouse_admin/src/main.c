/* bulgur_mouse_admin: Bulgur-mouse vegetable rodents (v1.0)
 * Bulgur-mouse forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, grind_ct, tail_cm, blgmx_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,grind_ct,tail_cm,blgmx_idx,age_yr,active;} blgmx_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_gc,t_tail,t_di;} blgmx_state_t;
static blgmx_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static blgmx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blgmx_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int gc,int tc,int bli,int ay){if(*cnt>=mx)return -1;blgmx_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->grind_ct=gc;x->tail_cm=tc;x->blgmx_idx=bli;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BLGMX] Bulgur ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" gc=");pi(gc);ps(" tc=");pi(tc);ps(" bli=");pi(bli);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bulgur_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_gc=0;st.t_tail=0;st.t_di=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BLGMX] Bulgur initialized\n");return 0;}
int bulgur_forest(int lc,int bl,int bw,int gc,int tc,int bli,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,gc,tc,bli,ay);}
int bulgur_feeding(int lc,int bl,int bw,int gc,int tc,int bli,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,gc,tc,bli,ay);}
int bulgur_breeding(int lc,int bl,int bw,int gc,int tc,int bli,int ay){return add(svb,&st.n_breed,&st.t_gc,N-4,lc,bl,bw,gc,tc,bli,ay);}
int bulgur_health(int lc,int bl,int bw,int gc,int tc,int bli,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,gc,tc,bli,ay);}
int bulgur_market(int lc,int bl,int bw,int gc,int tc,int bli,int ay){return add(svm,&st.n_mkt,&st.t_di,N-6,lc,bl,bw,gc,tc,bli,ay);}
void bulgur_report(void){ps("[BLGMX] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Gc=");pi(st.t_gc);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Di=");pi(st.t_di);ps("\n");}
void bulgur_state(void){ps("[BLGMX] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bulgur Admin Demo ===\n\n");bulgur_init();
ps("Bulgur forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*1),bw=18+(i*3),gc=3+(i%7),tc=8+(i*1),bli=(i%7)+1,ay=(i%5)+1;bulgur_forest(lc,bl,bw,gc,tc,bli,ay);}
ps("\nBulgur feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*1),bw=20+(i*3),gc=3+(i%7),tc=8+(i*1),bli=(i%6)+1,ay=(i%4)+1;bulgur_feeding(lc,bl,bw,gc,tc,bli,ay);}
ps("\nBulgur breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=22+(i*3),gc=4+(i%7),tc=9+(i*1),bli=(i%5)+1,ay=(i%3)+1;bulgur_breeding(lc,bl,bw,gc,tc,bli,ay);}
ps("\nBulgur health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*1),bw=16+(i*3),gc=2+(i%7),tc=7+(i*1),bli=(i%9)+1,ay=(i%5)+1;bulgur_health(lc,bl,bw,gc,tc,bli,ay);}
ps("\nBulgur market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=24+(i*3),gc=5+(i%7),tc=9+(i*1),bli=(i%4)+1,ay=(i%3)+1;bulgur_market(lc,bl,bw,gc,tc,bli,ay);}
ps("\n");bulgur_report();bulgur_state();ps("\n=== Demo Complete ===\n");return 0;}
