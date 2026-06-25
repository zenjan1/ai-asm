/* monjons_admin: Monjons (Petrogale) small rock-wallabies (v1.0)
 * Monjons forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, foot_cm, tail_cm, mj_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,foot_cm,tail_cm,mj_idx,age_yr,active;} mjon_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_foot,t_tail,t_mj;} mjon_state_t;
static mjon_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static mjon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mjon_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int tc,int mj,int ay){if(*cnt>=mx)return -1;mjon_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->foot_cm=fc;x->tail_cm=tc;x->mj_idx=mj;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MJON] Monjon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" tc=");pi(tc);ps(" mj=");pi(mj);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int monjons_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_foot=0;st.t_tail=0;st.t_mj=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[MJON] Monjons initialized\n");return 0;}
int monjons_forest(int lc,int bl,int bw,int fc,int tc,int mj,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,fc,tc,mj,ay);}
int monjons_feeding(int lc,int bl,int bw,int fc,int tc,int mj,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,tc,mj,ay);}
int monjons_breeding(int lc,int bl,int bw,int fc,int tc,int mj,int ay){return add(svb,&st.n_breed,&st.t_foot,N-4,lc,bl,bw,fc,tc,mj,ay);}
int monjons_health(int lc,int bl,int bw,int fc,int tc,int mj,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,fc,tc,mj,ay);}
int monjons_market(int lc,int bl,int bw,int fc,int tc,int mj,int ay){return add(svm,&st.n_mkt,&st.t_mj,N-6,lc,bl,bw,fc,tc,mj,ay);}
void monjons_report(void){ps("[MJON] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Foot=");pi(st.t_foot);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Mj=");pi(st.t_mj);ps("\n");}
void monjons_state(void){ps("[MJON] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Monjons Admin Demo ===\n\n");monjons_init();
ps("Monjons forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*2),bw=200+(i*15),fc=6+(i%3),tc=25+(i*2),mj=(i%8)+1,ay=(i%5)+1;monjons_forest(lc,bl,bw,fc,tc,mj,ay);}
ps("\nMonjons feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*1),bw=210+(i*12),fc=7+(i%2),tc=26+(i%1),mj=(i%6)+1,ay=(i%4)+1;monjons_feeding(lc,bl,bw,fc,tc,mj,ay);}
ps("\nMonjons breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*1),bw=220+(i*10),fc=7+(i%2),tc=28+(i%1),mj=(i%5)+1,ay=(i%3)+1;monjons_breeding(lc,bl,bw,fc,tc,mj,ay);}
ps("\nMonjons health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*3),bw=190+(i*20),fc=5+(i%3),tc=24+(i*2),mj=(i%10)+1,ay=(i%5)+1;monjons_health(lc,bl,bw,fc,tc,mj,ay);}
ps("\nMonjons market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*1),bw=240+(i*8),fc=8+(i%2),tc=29+(i%1),mj=(i%4)+1,ay=(i%3)+1;monjons_market(lc,bl,bw,fc,tc,mj,ay);}
ps("\n");monjons_report();monjons_state();ps("\n=== Demo Complete ===\n");return 0;}
