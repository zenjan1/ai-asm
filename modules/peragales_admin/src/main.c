/* peragales_admin: Peragales (Perameles) striped bandicoots (v1.0)
 * Peragales forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, whisker_cm, tail_cm, pg_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,whisker_cm,tail_cm,pg_idx,age_yr,active;} perg_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_whisker,t_tail,t_pg;} perg_state_t;
static perg_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static perg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(perg_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int tc,int pg,int ay){if(*cnt>=mx)return -1;perg_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->whisker_cm=wc;x->tail_cm=tc;x->pg_idx=pg;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PERG] Peragale ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" tc=");pi(tc);ps(" pg=");pi(pg);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int peragales_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_whisker=0;st.t_tail=0;st.t_pg=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PERG] Peragales initialized\n");return 0;}
int peragales_forest(int lc,int bl,int bw,int wc,int tc,int pg,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,wc,tc,pg,ay);}
int peragales_feeding(int lc,int bl,int bw,int wc,int tc,int pg,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,tc,pg,ay);}
int peragales_breeding(int lc,int bl,int bw,int wc,int tc,int pg,int ay){return add(svb,&st.n_breed,&st.t_whisker,N-4,lc,bl,bw,wc,tc,pg,ay);}
int peragales_health(int lc,int bl,int bw,int wc,int tc,int pg,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,wc,tc,pg,ay);}
int peragales_market(int lc,int bl,int bw,int wc,int tc,int pg,int ay){return add(svm,&st.n_mkt,&st.t_pg,N-6,lc,bl,bw,wc,tc,pg,ay);}
void peragales_report(void){ps("[PERG] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Whisker=");pi(st.t_whisker);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Pg=");pi(st.t_pg);ps("\n");}
void peragales_state(void){ps("[PERG] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Peragales Admin Demo ===\n\n");peragales_init();
ps("Peragales forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*2),bw=1+(i*1),wc=3+(i%3),tc=15+(i*2),pg=(i%8)+1,ay=(i%5)+1;peragales_forest(lc,bl,bw,wc,tc,pg,ay);}
ps("\nPeragales feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*1),bw=2+(i*1),wc=4+(i%2),tc=16+(i%1),pg=(i%6)+1,ay=(i%4)+1;peragales_feeding(lc,bl,bw,wc,tc,pg,ay);}
ps("\nPeragales breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=40+(i*1),bw=2+(i*1),wc=4+(i%2),tc=18+(i%1),pg=(i%5)+1,ay=(i%3)+1;peragales_breeding(lc,bl,bw,wc,tc,pg,ay);}
ps("\nPeragales health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=33+(i*3),bw=1+(i*1),wc=2+(i%3),tc=14+(i*2),pg=(i%10)+1,ay=(i%5)+1;peragales_health(lc,bl,bw,wc,tc,pg,ay);}
ps("\nPeragales market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=42+(i*1),bw=3+(i*1),wc=5+(i%2),tc=19+(i%1),pg=(i%4)+1,ay=(i%3)+1;peragales_market(lc,bl,bw,wc,tc,pg,ay);}
ps("\n");peragales_report();peragales_state();ps("\n=== Demo Complete ===\n");return 0;}
