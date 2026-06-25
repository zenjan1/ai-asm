/* osprey_admin: Osprey (Pandion haliaetus) osprey fish hawk (v1.0)
 * Osprey nest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, dive_speed, talon_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,dv_sp,tl_idx,age_yr,active;} osp_t;
typedef struct{int n_nest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_dv,t_tl;} osp_state_t;
static osp_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static osp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(osp_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int ds,int ti,int ay){if(*cnt>=mx)return -1;osp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->dv_sp=ds;x->tl_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[OSPR] Osprey ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" ds=");pi(ds);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int osp_init(void){if(init)return -1;st.n_nest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_dv=0;st.t_tl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[OSPR] Osprey initialized\n");return 0;}
int osp_nest(int lc,int bl,int bw,int ws,int ds,int ti,int ay){return add(apl,&st.n_nest,&st.t_ln,N,lc,bl,bw,ws,ds,ti,ay);}
int osp_feeding(int lc,int bl,int bw,int ws,int ds,int ti,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,ds,ti,ay);}
int osp_breeding(int lc,int bl,int bw,int ws,int ds,int ti,int ay){return add(abd,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,ds,ti,ay);}
int osp_health(int lc,int bl,int bw,int ws,int ds,int ti,int ay){return add(ahd,&st.n_health,&st.t_dv,N-6,lc,bl,bw,ws,ds,ti,ay);}
int osp_market(int lc,int bl,int bw,int ws,int ds,int ti,int ay){return add(amd,&st.n_mkt,&st.t_tl,N-6,lc,bl,bw,ws,ds,ti,ay);}
void osp_report(void){ps("[OSPR] Nest: ");pi(st.n_nest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Dv=");pi(st.t_dv);ps("\nMkt: ");pi(st.n_mkt);ps(" Tl=");pi(st.t_tl);ps("\n");}
void osp_state(void){ps("[OSPR] Nest=");pi(st.n_nest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Osprey Admin Demo ===\n\n");osp_init();
ps("Osprey nest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*3),bw=1200+(i*80),ws=150+(i*10),ds=80+(i*5),ti=(i%7)+1,ay=1+(i%10);osp_nest(lc,bl,bw,ws,ds,ti,ay);}
ps("\nOsprey feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=52+(i*3),bw=1250+(i*80),ws=155+(i*10),ds=82+(i*5),ti=(i%6)+2,ay=2+(i%8);osp_feeding(lc,bl,bw,ws,ds,ti,ay);}
ps("\nOsprey breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=55+(i*3),bw=1300+(i*80),ws=160+(i*10),ds=85+(i*5),ti=(i%5)+1,ay=3+(i%6);osp_breeding(lc,bl,bw,ws,ds,ti,ay);}
ps("\nOsprey health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=48+(i*4),bw=1150+(i*100),ws=145+(i*12),ds=78+(i*6),ti=(i%4)+3,ay=4+(i%5);osp_health(lc,bl,bw,ws,ds,ti,ay);}
ps("\nOsprey market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=60+(i*3),bw=1400+(i*80),ws=170+(i*10),ds=90+(i*5),ti=(i%3)+4,ay=5+(i%4);osp_market(lc,bl,bw,ws,ds,ti,ay);}
ps("\n");osp_report();osp_state();ps("\n=== Demo Complete ===\n");return 0;}
