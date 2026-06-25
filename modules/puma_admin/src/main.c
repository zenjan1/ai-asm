/* puma_admin: Puma (Puma concolor) puma mountain lion large cat (v1.0)
 * Puma mountain, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, run_speed, cl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,rn_sp,cl_idx,age_yr,active;} puma_t;
typedef struct{int n_mountain,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_rn,t_cl;} puma_state_t;
static puma_t arr_m[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_mk[N-6]; static puma_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(puma_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int ci,int ay){if(*cnt>=mx)return -1;puma_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->rn_sp=rs;x->cl_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PUMA] Puma ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int puma_init(void){if(init)return -1;st.n_mountain=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_rn=0;st.t_cl=0;for(int i=0;i<N;i++)arr_m[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_mk[i].active=0;init=1;ps("[PUMA] Puma initialized\n");return 0;}
int puma_mountain(int lc,int bl,int bw,int tc,int rs,int ci,int ay){return add(arr_m,&st.n_mountain,&st.t_ln,N,lc,bl,bw,tc,rs,ci,ay);}
int puma_feeding(int lc,int bl,int bw,int tc,int rs,int ci,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,ci,ay);}
int puma_breeding(int lc,int bl,int bw,int tc,int rs,int ci,int ay){return add(arr_b,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,ci,ay);}
int puma_health(int lc,int bl,int bw,int tc,int rs,int ci,int ay){return add(arr_h,&st.n_health,&st.t_rn,N-6,lc,bl,bw,tc,rs,ci,ay);}
int puma_market(int lc,int bl,int bw,int tc,int rs,int ci,int ay){return add(arr_mk,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,tc,rs,ci,ay);}
void puma_report(void){ps("[PUMA] Mountain: ");pi(st.n_mountain);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void puma_state(void){ps("[PUMA] Mountain=");pi(st.n_mountain);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Puma Admin Demo ===\n\n");puma_init();
ps("Puma mountain...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*8),bw=50+(i*8),tc=70+(i*5),rs=40+(i*5),ci=(i%7)+1,ay=(i%15)+1;puma_mountain(lc,bl,bw,tc,rs,ci,ay);}
ps("\nPuma feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=155+(i*6),bw=55+(i*6),tc=72+(i*4),rs=42+(i*4),ci=(i%6)+1,ay=(i%12)+1;puma_feeding(lc,bl,bw,tc,rs,ci,ay);}
ps("\nPuma breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=160+(i*5),bw=60+(i*5),tc=75+(i*3),rs=35+(i*6),ci=(i%5)+1,ay=(i%10)+1;puma_breeding(lc,bl,bw,tc,rs,ci,ay);}
ps("\nPuma health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=145+(i*10),bw=45+(i*10),tc=68+(i*6),rs=30+(i*8),ci=(i%8)+1,ay=(i%8)+1;puma_health(lc,bl,bw,tc,rs,ci,ay);}
ps("\nPuma market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=165+(i*4),bw=65+(i*4),tc=78+(i*3),rs=50+(i*4),ci=(i%4)+1,ay=(i%6)+1;puma_market(lc,bl,bw,tc,rs,ci,ay);}
ps("\n");puma_report();puma_state();ps("\n=== Demo Complete ===\n");return 0;}
