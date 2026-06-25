/* kudu_admin: Kudu (Tragelaphus strepsiceros) large spiral-horned antelope (v1.0)
 * Kudu woodland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, kn_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,rn_sp,kn_idx,age_yr,active;} kudu_t;
typedef struct{int n_wood,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_rn,t_kn;} kudu_state_t;
static kudu_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static kudu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kudu_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int ki,int ay){if(*cnt>=mx)return -1;kudu_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->rn_sp=rs;x->kn_idx=ki;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KUDU] Kudu ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" ki=");pi(ki);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int kudu_init(void){if(init)return -1;st.n_wood=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_rn=0;st.t_kn=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[KUDU] Kudu initialized\n");return 0;}
int kudu_woodland(int lc,int bl,int bw,int hc,int rs,int ki,int ay){return add(svl,&st.n_wood,&st.t_ln,N,lc,bl,bw,hc,rs,ki,ay);}
int kudu_feeding(int lc,int bl,int bw,int hc,int rs,int ki,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,ki,ay);}
int kudu_breeding(int lc,int bl,int bw,int hc,int rs,int ki,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,rs,ki,ay);}
int kudu_health(int lc,int bl,int bw,int hc,int rs,int ki,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,hc,rs,ki,ay);}
int kudu_market(int lc,int bl,int bw,int hc,int rs,int ki,int ay){return add(svm,&st.n_mkt,&st.t_kn,N-6,lc,bl,bw,hc,rs,ki,ay);}
void kudu_report(void){ps("[KUDU] Wood: ");pi(st.n_wood);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Kn=");pi(st.t_kn);ps("\n");}
void kudu_state(void){ps("[KUDU] Wood=");pi(st.n_wood);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Kudu Admin Demo ===\n\n");kudu_init();
ps("Kudu woodland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=200+(i*20),hc=80+(i*5),rs=25+(i*3),ki=(i%8)+1,ay=(i%15)+1;kudu_woodland(lc,bl,bw,hc,rs,ki,ay);}
ps("\nKudu feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=210+(i*15),hc=82+(i*4),rs=27+(i*2),ki=(i%6)+1,ay=(i%12)+1;kudu_feeding(lc,bl,bw,hc,rs,ki,ay);}
ps("\nKudu breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=220+(i*12),hc=85+(i*4),rs=24+(i*4),ki=(i%5)+1,ay=(i%10)+1;kudu_breeding(lc,bl,bw,hc,rs,ki,ay);}
ps("\nKudu health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=190+(i*25),hc=78+(i*6),rs=22+(i*5),ki=(i%10)+1,ay=(i%8)+1;kudu_health(lc,bl,bw,hc,rs,ki,ay);}
ps("\nKudu market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*5),bw=230+(i*10),hc=90+(i*3),rs=30+(i*2),ki=(i%4)+1,ay=(i%7)+1;kudu_market(lc,bl,bw,hc,rs,ki,ay);}
ps("\n");kudu_report();kudu_state();ps("\n=== Demo Complete ===\n");return 0;}
