/* chipmunk_admin: Chipmunk (Tamias striatus) eastern chipmunk small squirrel (v1.0)
 * Chipmunk burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, run_speed, ch_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,rn_sp,ch_idx,age_yr,active;} chip_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_rn,t_ch;} chip_state_t;
static chip_t arr_b[N],arr_fd[N-2],arr_br[N-4],arr_h[N-6],arr_m[N-6]; static chip_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chip_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int chi,int ay){if(*cnt>=mx)return -1;chip_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->rn_sp=rs;x->ch_idx=chi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CHIP] Chipmunk ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" chi=");pi(chi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int chipmunk_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_rn=0;st.t_ch=0;for(int i=0;i<N;i++)arr_b[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_br[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[CHIP] Chipmunk initialized\n");return 0;}
int chipmunk_burrow(int lc,int bl,int bw,int tc,int rs,int chi,int ay){return add(arr_b,&st.n_burrow,&st.t_ln,N,lc,bl,bw,tc,rs,chi,ay);}
int chipmunk_feeding(int lc,int bl,int bw,int tc,int rs,int chi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,chi,ay);}
int chipmunk_breeding(int lc,int bl,int bw,int tc,int rs,int chi,int ay){return add(arr_br,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,chi,ay);}
int chipmunk_health(int lc,int bl,int bw,int tc,int rs,int chi,int ay){return add(arr_h,&st.n_health,&st.t_rn,N-6,lc,bl,bw,tc,rs,chi,ay);}
int chipmunk_market(int lc,int bl,int bw,int tc,int rs,int chi,int ay){return add(arr_m,&st.n_mkt,&st.t_ch,N-6,lc,bl,bw,tc,rs,chi,ay);}
void chipmunk_report(void){ps("[CHIP] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Ch=");pi(st.t_ch);ps("\n");}
void chipmunk_state(void){ps("[CHIP] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chipmunk Admin Demo ===\n\n");chipmunk_init();
ps("Chipmunk burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=12+(i*1),bw=80+(i*8),tc=8+(i*1),rs=5+(i*1),chi=(i%6)+1,ay=(i%4)+1;chipmunk_burrow(lc,bl,bw,tc,rs,chi,ay);}
ps("\nChipmunk feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=13+(i*1),bw=85+(i*6),tc=9+(i*1),rs=6+(i*1),chi=(i%5)+1,ay=(i%3)+1;chipmunk_feeding(lc,bl,bw,tc,rs,chi,ay);}
ps("\nChipmunk breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=14+(i*1),bw=90+(i*5),tc=9+(i*1),rs=4+(i*2),chi=(i%4)+1,ay=(i%3)+1;chipmunk_breeding(lc,bl,bw,tc,rs,chi,ay);}
ps("\nChipmunk health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=11+(i*1),bw=75+(i*10),tc=7+(i*1),rs=3+(i*2),chi=(i%7)+1,ay=(i%2)+1;chipmunk_health(lc,bl,bw,tc,rs,chi,ay);}
ps("\nChipmunk market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=15+(i*1),bw=95+(i*4),tc=10+(i*1),rs=8+(i*1),chi=(i%3)+1,ay=(i%2)+1;chipmunk_market(lc,bl,bw,tc,rs,chi,ay);}
ps("\n");chipmunk_report();chipmunk_state();ps("\n=== Demo Complete ===\n");return 0;}
