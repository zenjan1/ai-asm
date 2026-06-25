/* potoroos_admin: Potoroos (Potorous) rat-kangaroo marsupials (v1.0)
 * Potoroos scrub, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, foot_cm, jump_cm, pt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,foot_cm,jmp_cm,pt_idx,age_yr,active;} ptos_t;
typedef struct{int n_scrub,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_foot,t_jmp,t_pt;} ptos_state_t;
static ptos_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static ptos_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ptos_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int jc,int pt,int ay){if(*cnt>=mx)return -1;ptos_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->foot_cm=fc;x->jmp_cm=jc;x->pt_idx=pt;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PTOS] Potoroo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" jc=");pi(jc);ps(" pt=");pi(pt);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int potoroos_init(void){if(init)return -1;st.n_scrub=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_foot=0;st.t_jmp=0;st.t_pt=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[PTOS] Potoroos initialized\n");return 0;}
int potoroos_scrub(int lc,int bl,int bw,int fc,int jc,int pt,int ay){return add(svl,&st.n_scrub,&st.t_ln,N,lc,bl,bw,fc,jc,pt,ay);}
int potoroos_feeding(int lc,int bl,int bw,int fc,int jc,int pt,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,jc,pt,ay);}
int potoroos_breeding(int lc,int bl,int bw,int fc,int jc,int pt,int ay){return add(svb,&st.n_breed,&st.t_foot,N-4,lc,bl,bw,fc,jc,pt,ay);}
int potoroos_health(int lc,int bl,int bw,int fc,int jc,int pt,int ay){return add(svh,&st.n_health,&st.t_jmp,N-6,lc,bl,bw,fc,jc,pt,ay);}
int potoroos_market(int lc,int bl,int bw,int fc,int jc,int pt,int ay){return add(svm,&st.n_mkt,&st.t_pt,N-6,lc,bl,bw,fc,jc,pt,ay);}
void potoroos_report(void){ps("[PTOS] Scrub: ");pi(st.n_scrub);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Foot=");pi(st.t_foot);ps("\nHealth: ");pi(st.n_health);ps(" Jmp=");pi(st.t_jmp);ps("\nMkt: ");pi(st.n_mkt);ps(" Pt=");pi(st.t_pt);ps("\n");}
void potoroos_state(void){ps("[PTOS] Scrub=");pi(st.n_scrub);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Potoroos Admin Demo ===\n\n");potoroos_init();
ps("Potoroos scrub...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*2),bw=2+(i*1),fc=8+(i*1),jc=15+(i*2),pt=(i%8)+1,ay=(i%6)+1;potoroos_scrub(lc,bl,bw,fc,jc,pt,ay);}
ps("\nPotoroos feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*2),bw=2+(i*1),fc=9+(i%2),jc=16+(i*2),pt=(i%6)+1,ay=(i%5)+1;potoroos_feeding(lc,bl,bw,fc,jc,pt,ay);}
ps("\nPotoroos breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*1),bw=3+(i*1),fc=9+(i%2),jc=17+(i*1),pt=(i%5)+1,ay=(i%4)+1;potoroos_breeding(lc,bl,bw,fc,jc,pt,ay);}
ps("\nPotoroos health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*3),bw=2+(i*1),fc=7+(i*1),jc=14+(i*3),pt=(i%10)+1,ay=(i%5)+1;potoroos_health(lc,bl,bw,fc,jc,pt,ay);}
ps("\nPotoroos market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*1),bw=3+(i*1),fc=10+(i%2),jc=18+(i*1),pt=(i%4)+1,ay=(i%4)+1;potoroos_market(lc,bl,bw,fc,jc,pt,ay);}
ps("\n");potoroos_report();potoroos_state();ps("\n=== Demo Complete ===\n");return 0;}
