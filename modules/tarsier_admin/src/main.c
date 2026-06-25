/* tarsier_admin: Tarsier (Tarsius syrichta) Philippine tarsier small primate (v1.0)
 * Tarsier forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, arm_cm, jump_cm, ey_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,arm_cm,jp_cm,ey_idx,age_yr,active;} tars_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_arm,t_jp,t_ey;} tars_state_t;
static tars_t arr_f[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static tars_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tars_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int jc,int eyi,int ay){if(*cnt>=mx)return -1;tars_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->arm_cm=ac;x->jp_cm=jc;x->ey_idx=eyi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TARS] Tarsier ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" jc=");pi(jc);ps(" eyi=");pi(eyi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int tarsier_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_arm=0;st.t_jp=0;st.t_ey=0;for(int i=0;i<N;i++)arr_f[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[TARS] Tarsier initialized\n");return 0;}
int tarsier_forest(int lc,int bl,int bw,int ac,int jc,int eyi,int ay){return add(arr_f,&st.n_forest,&st.t_ln,N,lc,bl,bw,ac,jc,eyi,ay);}
int tarsier_feeding(int lc,int bl,int bw,int ac,int jc,int eyi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,jc,eyi,ay);}
int tarsier_breeding(int lc,int bl,int bw,int ac,int jc,int eyi,int ay){return add(arr_b,&st.n_breed,&st.t_arm,N-4,lc,bl,bw,ac,jc,eyi,ay);}
int tarsier_health(int lc,int bl,int bw,int ac,int jc,int eyi,int ay){return add(arr_h,&st.n_health,&st.t_jp,N-6,lc,bl,bw,ac,jc,eyi,ay);}
int tarsier_market(int lc,int bl,int bw,int ac,int jc,int eyi,int ay){return add(arr_m,&st.n_mkt,&st.t_ey,N-6,lc,bl,bw,ac,jc,eyi,ay);}
void tarsier_report(void){ps("[TARS] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Arm=");pi(st.t_arm);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Ey=");pi(st.t_ey);ps("\n");}
void tarsier_state(void){ps("[TARS] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tarsier Admin Demo ===\n\n");tarsier_init();
ps("Tarsier forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*1),bw=100+(i*10),ac=5+(i*1),jc=40+(i*5),eyi=(i%6)+1,ay=(i%5)+1;tarsier_forest(lc,bl,bw,ac,jc,eyi,ay);}
ps("\nTarsier feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*1),bw=105+(i*8),ac=6+(i*1),jc=42+(i*4),eyi=(i%5)+1,ay=(i%4)+1;tarsier_feeding(lc,bl,bw,ac,jc,eyi,ay);}
ps("\nTarsier breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=110+(i*7),ac=6+(i*1),jc=45+(i*3),eyi=(i%4)+1,ay=(i%3)+1;tarsier_breeding(lc,bl,bw,ac,jc,eyi,ay);}
ps("\nTarsier health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*1),bw=95+(i*12),ac=4+(i*1),jc=38+(i*6),eyi=(i%7)+1,ay=(i%3)+1;tarsier_health(lc,bl,bw,ac,jc,eyi,ay);}
ps("\nTarsier market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=115+(i*6),ac=7+(i*1),jc=48+(i*3),eyi=(i%3)+1,ay=(i%2)+1;tarsier_market(lc,bl,bw,ac,jc,eyi,ay);}
ps("\n");tarsier_report();tarsier_state();ps("\n=== Demo Complete ===\n");return 0;}
