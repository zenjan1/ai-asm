/* setifers_admin: Setifers (Setifer) tenrec-like mammals (v1.0)
 * Setifers forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, quill_cm, tail_cm, sf_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,quill_cm,tail_cm,sf_idx,age_yr,active;} stfr_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_quill,t_tail,t_sf;} stfr_state_t;
static stfr_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static stfr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(stfr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int qc,int tc,int sf,int ay){if(*cnt>=mx)return -1;stfr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->quill_cm=qc;x->tail_cm=tc;x->sf_idx=sf;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[STFR] Setifer ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" qc=");pi(qc);ps(" tc=");pi(tc);ps(" sf=");pi(sf);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int setifers_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_quill=0;st.t_tail=0;st.t_sf=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[STFR] Setifers initialized\n");return 0;}
int setifers_forest(int lc,int bl,int bw,int qc,int tc,int sf,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,qc,tc,sf,ay);}
int setifers_feeding(int lc,int bl,int bw,int qc,int tc,int sf,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,qc,tc,sf,ay);}
int setifers_breeding(int lc,int bl,int bw,int qc,int tc,int sf,int ay){return add(svb,&st.n_breed,&st.t_quill,N-4,lc,bl,bw,qc,tc,sf,ay);}
int setifers_health(int lc,int bl,int bw,int qc,int tc,int sf,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,qc,tc,sf,ay);}
int setifers_market(int lc,int bl,int bw,int qc,int tc,int sf,int ay){return add(svm,&st.n_mkt,&st.t_sf,N-6,lc,bl,bw,qc,tc,sf,ay);}
void setifers_report(void){ps("[STFR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Quill=");pi(st.t_quill);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Sf=");pi(st.t_sf);ps("\n");}
void setifers_state(void){ps("[STFR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Setifers Admin Demo ===\n\n");setifers_init();
ps("Setifers forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=15+(i*1),bw=50+(i*5),qc=2+(i%2),tc=5+(i*1),sf=(i%8)+1,ay=(i%5)+1;setifers_forest(lc,bl,bw,qc,tc,sf,ay);}
ps("\nSetifers feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=16+(i*1),bw=55+(i*4),qc=2+(i%2),tc=6+(i*1),sf=(i%6)+1,ay=(i%4)+1;setifers_feeding(lc,bl,bw,qc,tc,sf,ay);}
ps("\nSetifers breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=17+(i*1),bw=60+(i*3),qc=3+(i%2),tc=6+(i*1),sf=(i%5)+1,ay=(i%3)+1;setifers_breeding(lc,bl,bw,qc,tc,sf,ay);}
ps("\nSetifers health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i*2),bw=45+(i*6),qc=1+(i%2),tc=4+(i*1),sf=(i%10)+1,ay=(i%5)+1;setifers_health(lc,bl,bw,qc,tc,sf,ay);}
ps("\nSetifers market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=18+(i*1),bw=65+(i*2),qc=3+(i%2),tc=7+(i*1),sf=(i%4)+1,ay=(i%3)+1;setifers_market(lc,bl,bw,qc,tc,sf,ay);}
ps("\n");setifers_report();setifers_state();ps("\n=== Demo Complete ===\n");return 0;}
