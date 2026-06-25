/* oyster_admin: Oyster (Crassostrea gigas) Pacific oyster marine bivalve (v1.0)
 * Oyster estuary, feeding, breeding, health, market
 * Features: shell_len_cm, body_wt_g, gill_cm, swim_speed, wt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,sh_ln,bdy_wt,gl_cm,sw_sp,wt_idx,age_yr,active;} oyst_t;
typedef struct{int n_estuary,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_gl,t_sw,t_wt2;} oyst_state_t;
static oyst_t arr_e[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static oyst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oyst_t*a,int*cnt,int*sum,int mx,int lc,int sl,int bw,int gc,int ss,int wi,int ay){if(*cnt>=mx)return -1;oyst_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->sh_ln=sl;x->bdy_wt=bw;x->gl_cm=gc;x->sw_sp=ss;x->wt_idx=wi;x->age_yr=ay;x->active=1;*sum+=sl;(*cnt)++;ps("[OYST] Oyster ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sl=");pi(sl);ps(" bw=");pi(bw);ps(" gc=");pi(gc);ps(" ss=");pi(ss);ps(" wi=");pi(wi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int oyster_init(void){if(init)return -1;st.n_estuary=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_gl=0;st.t_sw=0;st.t_wt2=0;for(int i=0;i<N;i++)arr_e[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[OYST] Oyster initialized\n");return 0;}
int oyster_estuary(int lc,int sl,int bw,int gc,int ss,int wi,int ay){return add(arr_e,&st.n_estuary,&st.t_ln,N,lc,sl,bw,gc,ss,wi,ay);}
int oyster_feeding(int lc,int sl,int bw,int gc,int ss,int wi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,sl,bw,gc,ss,wi,ay);}
int oyster_breeding(int lc,int sl,int bw,int gc,int ss,int wi,int ay){return add(arr_b,&st.n_breed,&st.t_gl,N-4,lc,sl,bw,gc,ss,wi,ay);}
int oyster_health(int lc,int sl,int bw,int gc,int ss,int wi,int ay){return add(arr_h,&st.n_health,&st.t_sw,N-6,lc,sl,bw,gc,ss,wi,ay);}
int oyster_market(int lc,int sl,int bw,int gc,int ss,int wi,int ay){return add(arr_m,&st.n_mkt,&st.t_wt2,N-6,lc,sl,bw,gc,ss,wi,ay);}
void oyster_report(void){ps("[OYST] Estuary: ");pi(st.n_estuary);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Gl=");pi(st.t_gl);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Wt2=");pi(st.t_wt2);ps("\n");}
void oyster_state(void){ps("[OYST] Estuary=");pi(st.n_estuary);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Oyster Admin Demo ===\n\n");oyster_init();
ps("Oyster estuary...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sl=8+(i*1),bw=50+(i*5),gc=3+(i*1),ss=1+(i*1),wi=(i%6)+1,ay=(i%5)+1;oyster_estuary(lc,sl,bw,gc,ss,wi,ay);}
ps("\nOyster feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sl=9+(i*1),bw=55+(i*4),gc=4+(i*1),ss=1+(i*1),wi=(i%5)+1,ay=(i%4)+1;oyster_feeding(lc,sl,bw,gc,ss,wi,ay);}
ps("\nOyster breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sl=10+(i*1),bw=60+(i*3),gc=5+(i*1),ss=2+(i*1),wi=(i%4)+1,ay=(i%3)+1;oyster_breeding(lc,sl,bw,gc,ss,wi,ay);}
ps("\nOyster health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sl=7+(i*2),bw=45+(i*6),gc=2+(i*1),ss=1+(i*2),wi=(i%7)+1,ay=(i%3)+1;oyster_health(lc,sl,bw,gc,ss,wi,ay);}
ps("\nOyster market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sl=11+(i*1),bw=65+(i*3),gc=6+(i*1),ss=2+(i*1),wi=(i%3)+1,ay=(i%2)+1;oyster_market(lc,sl,bw,gc,ss,wi,ay);}
ps("\n");oyster_report();oyster_state();ps("\n=== Demo Complete ===\n");return 0;}
