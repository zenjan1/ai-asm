/* prawn_admin: Prawn (Penaeus monodon) giant tiger prawn marine crustacean (v1.0)
 * Prawn ocean, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, swim_speed, sh_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,sw_sp,sh_idx,age_yr,active;} praw_t;
typedef struct{int n_ocean,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_sw,t_sh;} praw_state_t;
static praw_t arr_o[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static praw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(praw_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int shi,int ay){if(*cnt>=mx)return -1;praw_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->sw_sp=ss;x->sh_idx=shi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PRAW] Prawn ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" shi=");pi(shi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int prawn_init(void){if(init)return -1;st.n_ocean=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_sw=0;st.t_sh=0;for(int i=0;i<N;i++)arr_o[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[PRAW] Prawn initialized\n");return 0;}
int prawn_ocean(int lc,int bl,int bw,int tc,int ss,int shi,int ay){return add(arr_o,&st.n_ocean,&st.t_ln,N,lc,bl,bw,tc,ss,shi,ay);}
int prawn_feeding(int lc,int bl,int bw,int tc,int ss,int shi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,shi,ay);}
int prawn_breeding(int lc,int bl,int bw,int tc,int ss,int shi,int ay){return add(arr_b,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,ss,shi,ay);}
int prawn_health(int lc,int bl,int bw,int tc,int ss,int shi,int ay){return add(arr_h,&st.n_health,&st.t_sw,N-6,lc,bl,bw,tc,ss,shi,ay);}
int prawn_market(int lc,int bl,int bw,int tc,int ss,int shi,int ay){return add(arr_m,&st.n_mkt,&st.t_sh,N-6,lc,bl,bw,tc,ss,shi,ay);}
void prawn_report(void){ps("[PRAW] Ocean: ");pi(st.n_ocean);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sh=");pi(st.t_sh);ps("\n");}
void prawn_state(void){ps("[PRAW] Ocean=");pi(st.n_ocean);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Prawn Admin Demo ===\n\n");prawn_init();
ps("Prawn ocean...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=15+(i*1),bw=20+(i*3),tc=8+(i*1),ss=5+(i*1),shi=(i%6)+1,ay=(i%3)+1;prawn_ocean(lc,bl,bw,tc,ss,shi,ay);}
ps("\nPrawn feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=16+(i*1),bw=22+(i*2),tc=9+(i*1),ss=6+(i*1),shi=(i%5)+1,ay=(i%2)+1;prawn_feeding(lc,bl,bw,tc,ss,shi,ay);}
ps("\nPrawn breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=17+(i*1),bw=24+(i*2),tc=10+(i*1),ss=4+(i*2),shi=(i%4)+1,ay=(i%2)+1;prawn_breeding(lc,bl,bw,tc,ss,shi,ay);}
ps("\nPrawn health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i*2),bw=18+(i*4),tc=7+(i*1),ss=3+(i*2),shi=(i%7)+1,ay=(i%2)+1;prawn_health(lc,bl,bw,tc,ss,shi,ay);}
ps("\nPrawn market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=18+(i*1),bw=26+(i*2),tc=11+(i*1),ss=7+(i*1),shi=(i%3)+1,ay=(i%1)+1;prawn_market(lc,bl,bw,tc,ss,shi,ay);}
ps("\n");prawn_report();prawn_state();ps("\n=== Demo Complete ===\n");return 0;}
