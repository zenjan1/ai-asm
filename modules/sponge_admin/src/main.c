/* sponge_admin: Sponge (Spongia officinalis) common sea sponge marine organism (v1.0)
 * Sponge reef, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, pore_idx, swim_speed, wt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,pr_idx,sw_sp,wt_idx,age_yr,active;} spng_t;
typedef struct{int n_reef,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_pr,t_sw,t_wt2;} spng_state_t;
static spng_t arr_r[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static spng_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spng_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int pri,int ss,int wi,int ay){if(*cnt>=mx)return -1;spng_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->pr_idx=pri;x->sw_sp=ss;x->wt_idx=wi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SPNG] Sponge ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" pri=");pi(pri);ps(" ss=");pi(ss);ps(" wi=");pi(wi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int sponge_init(void){if(init)return -1;st.n_reef=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_pr=0;st.t_sw=0;st.t_wt2=0;for(int i=0;i<N;i++)arr_r[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[SPNG] Sponge initialized\n");return 0;}
int sponge_reef(int lc,int bl,int bw,int pri,int ss,int wi,int ay){return add(arr_r,&st.n_reef,&st.t_ln,N,lc,bl,bw,pri,ss,wi,ay);}
int sponge_feeding(int lc,int bl,int bw,int pri,int ss,int wi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,pri,ss,wi,ay);}
int sponge_breeding(int lc,int bl,int bw,int pri,int ss,int wi,int ay){return add(arr_b,&st.n_breed,&st.t_pr,N-4,lc,bl,bw,pri,ss,wi,ay);}
int sponge_health(int lc,int bl,int bw,int pri,int ss,int wi,int ay){return add(arr_h,&st.n_health,&st.t_sw,N-6,lc,bl,bw,pri,ss,wi,ay);}
int sponge_market(int lc,int bl,int bw,int pri,int ss,int wi,int ay){return add(arr_m,&st.n_mkt,&st.t_wt2,N-6,lc,bl,bw,pri,ss,wi,ay);}
void sponge_report(void){ps("[SPNG] Reef: ");pi(st.n_reef);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Pr=");pi(st.t_pr);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Wt2=");pi(st.t_wt2);ps("\n");}
void sponge_state(void){ps("[SPNG] Reef=");pi(st.n_reef);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Sponge Admin Demo ===\n\n");sponge_init();
ps("Sponge reef...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*2),bw=100+(i*15),pri=(i%6)+1,ss=0+(i%2),wi=(i%5)+1,ay=(i%5)+1;sponge_reef(lc,bl,bw,pri,ss,wi,ay);}
ps("\nSponge feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=12+(i*2),bw=110+(i*12),pri=(i%5)+1,ss=0+(i%2),wi=(i%4)+1,ay=(i%4)+1;sponge_feeding(lc,bl,bw,pri,ss,wi,ay);}
ps("\nSponge breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=14+(i*1),bw=120+(i*10),pri=(i%4)+1,ss=0+(i%2),wi=(i%3)+1,ay=(i%3)+1;sponge_breeding(lc,bl,bw,pri,ss,wi,ay);}
ps("\nSponge health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=8+(i*3),bw=90+(i*18),pri=(i%7)+1,ss=0+(i%2),wi=(i%6)+1,ay=(i%2)+1;sponge_health(lc,bl,bw,pri,ss,wi,ay);}
ps("\nSponge market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=16+(i*1),bw=130+(i*8),pri=(i%3)+1,ss=0+(i%2),wi=(i%3)+1,ay=(i%2)+1;sponge_market(lc,bl,bw,pri,ss,wi,ay);}
ps("\n");sponge_report();sponge_state();ps("\n=== Demo Complete ===\n");return 0;}
