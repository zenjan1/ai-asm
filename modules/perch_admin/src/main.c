/* perch_admin: Perch (Perca fluviatilis) European perch freshwater fish (v1.0)
 * Perch river, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fin_cm, swim_speed, sc_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,sc_idx,age_yr,active;} perch_t;
typedef struct{int n_river,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_sc;} perch_state_t;
static perch_t arr_r[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static perch_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(perch_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int sci,int ay){if(*cnt>=mx)return -1;perch_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->sc_idx=sci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PERCH] Perch ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" sci=");pi(sci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int perch_init(void){if(init)return -1;st.n_river=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_sc=0;for(int i=0;i<N;i++)arr_r[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[PERCH] Perch initialized\n");return 0;}
int perch_river(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_r,&st.n_river,&st.t_ln,N,lc,bl,bw,fc,ss,sci,ay);}
int perch_feeding(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,sci,ay);}
int perch_breeding(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_b,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,sci,ay);}
int perch_health(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_h,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,sci,ay);}
int perch_market(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_m,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fc,ss,sci,ay);}
void perch_report(void){ps("[PERCH] River: ");pi(st.n_river);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void perch_state(void){ps("[PERCH] River=");pi(st.n_river);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Perch Admin Demo ===\n\n");perch_init();
ps("Perch river...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*2),bw=100+(i*15),fc=5+(i*1),ss=10+(i*2),sci=(i%6)+1,ay=(i%6)+1;perch_river(lc,bl,bw,fc,ss,sci,ay);}
ps("\nPerch feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=22+(i*1),bw=110+(i*12),fc=6+(i*1),ss=12+(i*1),sci=(i%5)+1,ay=(i%5)+1;perch_feeding(lc,bl,bw,fc,ss,sci,ay);}
ps("\nPerch breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=24+(i*1),bw=120+(i*10),fc=6+(i*1),ss=8+(i*3),sci=(i%4)+1,ay=(i%4)+1;perch_breeding(lc,bl,bw,fc,ss,sci,ay);}
ps("\nPerch health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=18+(i*3),bw=90+(i*18),fc=4+(i*1),ss=7+(i*3),sci=(i%7)+1,ay=(i%3)+1;perch_health(lc,bl,bw,fc,ss,sci,ay);}
ps("\nPerch market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=26+(i*1),bw=130+(i*8),fc=7+(i*1),ss=15+(i*2),sci=(i%3)+1,ay=(i%3)+1;perch_market(lc,bl,bw,fc,ss,sci,ay);}
ps("\n");perch_report();perch_state();ps("\n=== Demo Complete ===\n");return 0;}
