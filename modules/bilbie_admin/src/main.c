/* bilbie_admin: Bilbie (Macrotis) greater bilby species (v1.0)
 * Bilbie forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, claw_len, tail_cm, bb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_len,tail_cm,bb_idx,age_yr,active;} bilb_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_tail,t_bb;} bilb_state_t;
static bilb_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static bilb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bilb_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cl,int tc,int bb,int ay){if(*cnt>=mx)return -1;bilb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_len=cl;x->tail_cm=tc;x->bb_idx=bb;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BILB] Bilbie ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cl=");pi(cl);ps(" tc=");pi(tc);ps(" bb=");pi(bb);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bilbie_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_tail=0;st.t_bb=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BILB] Bilbie initialized\n");return 0;}
int bilbie_forest(int lc,int bl,int bw,int cl,int tc,int bb,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,cl,tc,bb,ay);}
int bilbie_feeding(int lc,int bl,int bw,int cl,int tc,int bb,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cl,tc,bb,ay);}
int bilbie_breeding(int lc,int bl,int bw,int cl,int tc,int bb,int ay){return add(svb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cl,tc,bb,ay);}
int bilbie_health(int lc,int bl,int bw,int cl,int tc,int bb,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,cl,tc,bb,ay);}
int bilbie_market(int lc,int bl,int bw,int cl,int tc,int bb,int ay){return add(svm,&st.n_mkt,&st.t_bb,N-6,lc,bl,bw,cl,tc,bb,ay);}
void bilbie_report(void){ps("[BILB] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Bb=");pi(st.t_bb);ps("\n");}
void bilbie_state(void){ps("[BILB] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bilbie Admin Demo ===\n\n");bilbie_init();
ps("Bilbie forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*2),bw=1500+(i*100),cl=5+(i%3),tc=22+(i*2),bb=(i%8)+1,ay=(i%5)+1;bilbie_forest(lc,bl,bw,cl,tc,bb,ay);}
ps("\nBilbie feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=47+(i*2),bw=1600+(i*80),cl=5+(i%3),tc=23+(i*2),bb=(i%6)+1,ay=(i%4)+1;bilbie_feeding(lc,bl,bw,cl,tc,bb,ay);}
ps("\nBilbie breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=49+(i*2),bw=1700+(i*60),cl=6+(i%3),tc=24+(i*2),bb=(i%5)+1,ay=(i%3)+1;bilbie_breeding(lc,bl,bw,cl,tc,bb,ay);}
ps("\nBilbie health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=43+(i*3),bw=1400+(i*120),cl=4+(i%3),tc=21+(i*2),bb=(i%10)+1,ay=(i%5)+1;bilbie_health(lc,bl,bw,cl,tc,bb,ay);}
ps("\nBilbie market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=51+(i*2),bw=1800+(i*60),cl=6+(i%3),tc=25+(i*2),bb=(i%4)+1,ay=(i%3)+1;bilbie_market(lc,bl,bw,cl,tc,bb,ay);}
ps("\n");bilbie_report();bilbie_state();ps("\n=== Demo Complete ===\n");return 0;}
