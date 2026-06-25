/* suni_admin: Suni (Neotragus moschatus) smallest antelope species (v1.0)
 * Suni forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, ear_cm, sprint_speed, sn_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,sp_sp,sn_idx,age_yr,active;} suni_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_sp,t_sn;} suni_state_t;
static suni_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static suni_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(suni_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int ss,int si,int ay){if(*cnt>=mx)return -1;suni_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->sp_sp=ss;x->sn_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SUNI] Suni ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int suni_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_sp=0;st.t_sn=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[SUNI] Suni initialized\n");return 0;}
int suni_forest(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ec,ss,si,ay);}
int suni_feeding(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,ss,si,ay);}
int suni_breeding(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,ss,si,ay);}
int suni_health(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svh,&st.n_health,&st.t_sp,N-6,lc,bl,bw,ec,ss,si,ay);}
int suni_market(int lc,int bl,int bw,int ec,int ss,int si,int ay){return add(svm,&st.n_mkt,&st.t_sn,N-6,lc,bl,bw,ec,ss,si,ay);}
void suni_report(void){ps("[SUNI] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Sp=");pi(st.t_sp);ps("\nMkt: ");pi(st.n_mkt);ps(" Sn=");pi(st.t_sn);ps("\n");}
void suni_state(void){ps("[SUNI] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Suni Admin Demo ===\n\n");suni_init();
ps("Suni forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*2),bw=2500+(i*200),ec=4+(i*1),ss=12+(i*2),si=(i%8)+1,ay=(i%8)+1;suni_forest(lc,bl,bw,ec,ss,si,ay);}
ps("\nSuni feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=2600+(i*150),ec=5+(i*1),ss=13+(i*1),si=(i%6)+1,ay=(i%7)+1;suni_feeding(lc,bl,bw,ec,ss,si,ay);}
ps("\nSuni breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=44+(i*1),bw=2700+(i*120),ec=5+(i*1),ss=11+(i*2),si=(i%5)+1,ay=(i%6)+1;suni_breeding(lc,bl,bw,ec,ss,si,ay);}
ps("\nSuni health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*3),bw=2400+(i*300),ec=3+(i*1),ss=10+(i*3),si=(i%10)+1,ay=(i%5)+1;suni_health(lc,bl,bw,ec,ss,si,ay);}
ps("\nSuni market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=46+(i*1),bw=2800+(i*100),ec=6+(i*1),ss=14+(i*1),si=(i%4)+1,ay=(i%4)+1;suni_market(lc,bl,bw,ec,ss,si,ay);}
ps("\n");suni_report();suni_state();ps("\n=== Demo Complete ===\n");return 0;}
