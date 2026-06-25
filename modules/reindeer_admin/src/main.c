/* reindeer_admin: Reindeer (Rangifer tarandus) reindeer Arctic deer (v1.0)
 * Reindeer tundra, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, antler_cm, run_speed, herd_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,antl_cm,run_sp,hr_idx,age_yr,active;} rind_t;
typedef struct{int n_tundra,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_antl,t_run,t_hr;} rind_state_t;
static rind_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static rind_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rind_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int rs,int hi,int ay){if(*cnt>=mx)return -1;rind_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->antl_cm=ac;x->run_sp=rs;x->hr_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[RIND] Reindeer ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" rs=");pi(rs);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rind_init(void){if(init)return -1;st.n_tundra=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_antl=0;st.t_run=0;st.t_hr=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[RIND] Reindeer initialized\n");return 0;}
int rind_tundra(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(apl,&st.n_tundra,&st.t_ln,N,lc,bl,bw,ac,rs,hi,ay);}
int rind_feeding(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,rs,hi,ay);}
int rind_breeding(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(abd,&st.n_breed,&st.t_antl,N-4,lc,bl,bw,ac,rs,hi,ay);}
int rind_health(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(ahd,&st.n_health,&st.t_run,N-6,lc,bl,bw,ac,rs,hi,ay);}
int rind_market(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(amd,&st.n_mkt,&st.t_hr,N-6,lc,bl,bw,ac,rs,hi,ay);}
void rind_report(void){ps("[RIND] Tundra: ");pi(st.n_tundra);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Antl=");pi(st.t_antl);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Hr=");pi(st.t_hr);ps("\n");}
void rind_state(void){ps("[RIND] Tundra=");pi(st.n_tundra);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Reindeer Admin Demo ===\n\n");rind_init();
ps("Reindeer tundra...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*10),bw=80+(i*10),ac=60+(i*5),rs=40+(i*5),hi=(i%8)+1,ay=2+(i%15);rind_tundra(lc,bl,bw,ac,rs,hi,ay);}
ps("\nReindeer feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=155+(i*10),bw=85+(i*10),ac=62+(i*5),rs=42+(i*5),hi=(i%7)+2,ay=3+(i%12);rind_feeding(lc,bl,bw,ac,rs,hi,ay);}
ps("\nReindeer breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=160+(i*10),bw=90+(i*10),ac=65+(i*5),rs=45+(i*5),hi=(i%6)+1,ay=4+(i%10);rind_breeding(lc,bl,bw,ac,rs,hi,ay);}
ps("\nReindeer health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=145+(i*12),bw=75+(i*12),ac=58+(i*6),rs=38+(i*6),hi=(i%5)+3,ay=5+(i%8);rind_health(lc,bl,bw,ac,rs,hi,ay);}
ps("\nReindeer market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=170+(i*10),bw=100+(i*10),ac=70+(i*5),rs=50+(i*5),hi=(i%4)+4,ay=6+(i%6);rind_market(lc,bl,bw,ac,rs,hi,ay);}
ps("\n");rind_report();rind_state();ps("\n=== Demo Complete ===\n");return 0;}
