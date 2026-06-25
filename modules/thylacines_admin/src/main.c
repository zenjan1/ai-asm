/* thylacines_admin: Thylacines (Thylacinus) extinct marsupial wolves (v1.0)
 * Thylacines forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, skull_cm, tail_cm, th_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,skull_cm,tail_cm,th_idx,age_yr,active;} thyl_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_skull,t_tail,t_th;} thyl_state_t;
static thyl_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static thyl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(thyl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int tc,int th,int ay){if(*cnt>=mx)return -1;thyl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->skull_cm=sc;x->tail_cm=tc;x->th_idx=th;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[THYL] Thylacine ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" tc=");pi(tc);ps(" th=");pi(th);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int thylacines_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_skull=0;st.t_tail=0;st.t_th=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[THYL] Thylacines initialized\n");return 0;}
int thylacines_forest(int lc,int bl,int bw,int sc,int tc,int th,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,sc,tc,th,ay);}
int thylacines_feeding(int lc,int bl,int bw,int sc,int tc,int th,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,tc,th,ay);}
int thylacines_breeding(int lc,int bl,int bw,int sc,int tc,int th,int ay){return add(svb,&st.n_breed,&st.t_skull,N-4,lc,bl,bw,sc,tc,th,ay);}
int thylacines_health(int lc,int bl,int bw,int sc,int tc,int th,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,sc,tc,th,ay);}
int thylacines_market(int lc,int bl,int bw,int sc,int tc,int th,int ay){return add(svm,&st.n_mkt,&st.t_th,N-6,lc,bl,bw,sc,tc,th,ay);}
void thylacines_report(void){ps("[THYL] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Skull=");pi(st.t_skull);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Th=");pi(st.t_th);ps("\n");}
void thylacines_state(void){ps("[THYL] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Thylacines Admin Demo ===\n\n");thylacines_init();
ps("Thylacines forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*3),bw=20+(i*2),sc=25+(i%3),tc=50+(i*2),th=(i%8)+1,ay=(i%5)+1;thylacines_forest(lc,bl,bw,sc,tc,th,ay);}
ps("\nThylacines feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=102+(i*2),bw=21+(i*1),sc=26+(i%2),tc=52+(i*1),th=(i%6)+1,ay=(i%4)+1;thylacines_feeding(lc,bl,bw,sc,tc,th,ay);}
ps("\nThylacines breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=105+(i*2),bw=23+(i*1),sc=27+(i%2),tc=55+(i*1),th=(i%5)+1,ay=(i%3)+1;thylacines_breeding(lc,bl,bw,sc,tc,th,ay);}
ps("\nThylacines health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*4),bw=19+(i*2),sc=24+(i%3),tc=48+(i*2),th=(i%10)+1,ay=(i%5)+1;thylacines_health(lc,bl,bw,sc,tc,th,ay);}
ps("\nThylacines market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=108+(i*2),bw=25+(i*1),sc=28+(i%2),tc=58+(i*1),th=(i%4)+1,ay=(i%3)+1;thylacines_market(lc,bl,bw,sc,tc,th,ay);}
ps("\n");thylacines_report();thylacines_state();ps("\n=== Demo Complete ===\n");return 0;}
