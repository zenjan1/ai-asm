/* pike_admin: Pike (Esox lucius) northern pike freshwater predatory fish (v1.0)
 * Pike lake, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, jaw_cm, swim_speed, fn_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,jaw_cm,sw_sp,fn_idx,age_yr,active;} pike_t;
typedef struct{int n_lake,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_jaw,t_sw,t_fn;} pike_state_t;
static pike_t arr_l[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static pike_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pike_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int jc,int ss,int fni,int ay){if(*cnt>=mx)return -1;pike_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->jaw_cm=jc;x->sw_sp=ss;x->fn_idx=fni;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PIKE] Pike ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" jc=");pi(jc);ps(" ss=");pi(ss);ps(" fni=");pi(fni);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int pike_init(void){if(init)return -1;st.n_lake=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_jaw=0;st.t_sw=0;st.t_fn=0;for(int i=0;i<N;i++)arr_l[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[PIKE] Pike initialized\n");return 0;}
int pike_lake(int lc,int bl,int bw,int jc,int ss,int fni,int ay){return add(arr_l,&st.n_lake,&st.t_ln,N,lc,bl,bw,jc,ss,fni,ay);}
int pike_feeding(int lc,int bl,int bw,int jc,int ss,int fni,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,jc,ss,fni,ay);}
int pike_breeding(int lc,int bl,int bw,int jc,int ss,int fni,int ay){return add(arr_b,&st.n_breed,&st.t_jaw,N-4,lc,bl,bw,jc,ss,fni,ay);}
int pike_health(int lc,int bl,int bw,int jc,int ss,int fni,int ay){return add(arr_h,&st.n_health,&st.t_sw,N-6,lc,bl,bw,jc,ss,fni,ay);}
int pike_market(int lc,int bl,int bw,int jc,int ss,int fni,int ay){return add(arr_m,&st.n_mkt,&st.t_fn,N-6,lc,bl,bw,jc,ss,fni,ay);}
void pike_report(void){ps("[PIKE] Lake: ");pi(st.n_lake);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Jaw=");pi(st.t_jaw);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Fn=");pi(st.t_fn);ps("\n");}
void pike_state(void){ps("[PIKE] Lake=");pi(st.n_lake);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Pike Admin Demo ===\n\n");pike_init();
ps("Pike lake...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*5),bw=3+(i*1),jc=8+(i*1),ss=20+(i*3),fni=(i%6)+1,ay=(i%10)+1;pike_lake(lc,bl,bw,jc,ss,fni,ay);}
ps("\nPike feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=65+(i*4),bw=4+(i*1),jc=9+(i*1),ss=22+(i*2),fni=(i%5)+1,ay=(i%8)+1;pike_feeding(lc,bl,bw,jc,ss,fni,ay);}
ps("\nPike breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=70+(i*3),bw=4+(i*1),jc=10+(i*1),ss=18+(i*4),fni=(i%4)+1,ay=(i%6)+1;pike_breeding(lc,bl,bw,jc,ss,fni,ay);}
ps("\nPike health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=55+(i*6),bw=2+(i*2),jc=7+(i*1),ss=15+(i*5),fni=(i%7)+1,ay=(i%5)+1;pike_health(lc,bl,bw,jc,ss,fni,ay);}
ps("\nPike market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=75+(i*3),bw=5+(i*1),jc=11+(i*1),ss=25+(i*3),fni=(i%3)+1,ay=(i%4)+1;pike_market(lc,bl,bw,jc,ss,fni,ay);}
ps("\n");pike_report();pike_state();ps("\n=== Demo Complete ===\n");return 0;}
