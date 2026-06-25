/* chinchilla_admin: Chinchilla (Chinchilla lanigera) long-tailed chinchilla small rodent (v1.0)
 * Chinchilla burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, ear_cm, jump_cm, fr_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,jp_cm,fr_idx,age_yr,active;} chch_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_jp,t_fr;} chch_state_t;
static chch_t arr_b[N],arr_fd[N-2],arr_br[N-4],arr_h[N-6],arr_m[N-6]; static chch_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chch_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int jc,int fri,int ay){if(*cnt>=mx)return -1;chch_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->jp_cm=jc;x->fr_idx=fri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CHCH] Chinchilla ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" jc=");pi(jc);ps(" fri=");pi(fri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int chinchilla_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_jp=0;st.t_fr=0;for(int i=0;i<N;i++)arr_b[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_br[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[CHCH] Chinchilla initialized\n");return 0;}
int chinchilla_burrow(int lc,int bl,int bw,int ec,int jc,int fri,int ay){return add(arr_b,&st.n_burrow,&st.t_ln,N,lc,bl,bw,ec,jc,fri,ay);}
int chinchilla_feeding(int lc,int bl,int bw,int ec,int jc,int fri,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,jc,fri,ay);}
int chinchilla_breeding(int lc,int bl,int bw,int ec,int jc,int fri,int ay){return add(arr_br,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,jc,fri,ay);}
int chinchilla_health(int lc,int bl,int bw,int ec,int jc,int fri,int ay){return add(arr_h,&st.n_health,&st.t_jp,N-6,lc,bl,bw,ec,jc,fri,ay);}
int chinchilla_market(int lc,int bl,int bw,int ec,int jc,int fri,int ay){return add(arr_m,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,ec,jc,fri,ay);}
void chinchilla_report(void){ps("[CHCH] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void chinchilla_state(void){ps("[CHCH] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chinchilla Admin Demo ===\n\n");chinchilla_init();
ps("Chinchilla burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*2),bw=400+(i*30),ec=5+(i*1),jc=60+(i*5),fri=(i%6)+1,ay=(i%8)+1;chinchilla_burrow(lc,bl,bw,ec,jc,fri,ay);}
ps("\nChinchilla feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=27+(i*1),bw=420+(i*25),ec=6+(i*1),jc=65+(i*4),fri=(i%5)+1,ay=(i%7)+1;chinchilla_feeding(lc,bl,bw,ec,jc,fri,ay);}
ps("\nChinchilla breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=29+(i*1),bw=440+(i*20),ec=6+(i*1),jc=70+(i*3),fri=(i%4)+1,ay=(i%6)+1;chinchilla_breeding(lc,bl,bw,ec,jc,fri,ay);}
ps("\nChinchilla health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=23+(i*3),bw=380+(i*35),ec=4+(i*1),jc=55+(i*6),fri=(i%7)+1,ay=(i%5)+1;chinchilla_health(lc,bl,bw,ec,jc,fri,ay);}
ps("\nChinchilla market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=31+(i*1),bw=460+(i*15),ec=7+(i*1),jc=75+(i*3),fri=(i%3)+1,ay=(i%4)+1;chinchilla_market(lc,bl,bw,ec,jc,fri,ay);}
ps("\n");chinchilla_report();chinchilla_state();ps("\n=== Demo Complete ===\n");return 0;}
