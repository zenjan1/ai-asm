/* pangolin_admin: Pangolin (Manis javanica) Sunda pangolin scaly mammal (v1.0)
 * Pangolin forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, walk_speed, sc_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,wk_sp,sc_idx,age_yr,active;} pang_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_wk,t_sc;} pang_state_t;
static pang_t arr_f[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static pang_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pang_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ws,int sci,int ay){if(*cnt>=mx)return -1;pang_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->wk_sp=ws;x->sc_idx=sci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PANG] Pangolin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ws=");pi(ws);ps(" sci=");pi(sci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int pangolin_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_wk=0;st.t_sc=0;for(int i=0;i<N;i++)arr_f[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[PANG] Pangolin initialized\n");return 0;}
int pangolin_forest(int lc,int bl,int bw,int tc,int ws,int sci,int ay){return add(arr_f,&st.n_forest,&st.t_ln,N,lc,bl,bw,tc,ws,sci,ay);}
int pangolin_feeding(int lc,int bl,int bw,int tc,int ws,int sci,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ws,sci,ay);}
int pangolin_breeding(int lc,int bl,int bw,int tc,int ws,int sci,int ay){return add(arr_b,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,ws,sci,ay);}
int pangolin_health(int lc,int bl,int bw,int tc,int ws,int sci,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,tc,ws,sci,ay);}
int pangolin_market(int lc,int bl,int bw,int tc,int ws,int sci,int ay){return add(arr_m,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,tc,ws,sci,ay);}
void pangolin_report(void){ps("[PANG] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void pangolin_state(void){ps("[PANG] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Pangolin Admin Demo ===\n\n");pangolin_init();
ps("Pangolin forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=70+(i*5),bw=4+(i*1),tc=50+(i*3),ws=2+(i*1),sci=(i%6)+1,ay=(i%12)+1;pangolin_forest(lc,bl,bw,tc,ws,sci,ay);}
ps("\nPangolin feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=75+(i*4),bw=5+(i*1),tc=52+(i*2),ws=3+(i*1),sci=(i%5)+1,ay=(i%10)+1;pangolin_feeding(lc,bl,bw,tc,ws,sci,ay);}
ps("\nPangolin breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=80+(i*3),bw=5+(i*1),tc=55+(i*2),ws=2+(i*2),sci=(i%4)+1,ay=(i%8)+1;pangolin_breeding(lc,bl,bw,tc,ws,sci,ay);}
ps("\nPangolin health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=65+(i*6),bw=3+(i*2),tc=48+(i*4),ws=1+(i*3),sci=(i%7)+1,ay=(i%6)+1;pangolin_health(lc,bl,bw,tc,ws,sci,ay);}
ps("\nPangolin market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=85+(i*3),bw=6+(i*1),tc=58+(i*2),ws=4+(i*1),sci=(i%3)+1,ay=(i%5)+1;pangolin_market(lc,bl,bw,tc,ws,sci,ay);}
ps("\n");pangolin_report();pangolin_state();ps("\n=== Demo Complete ===\n");return 0;}
