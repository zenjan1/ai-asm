/* pig_admin: Pig (Sus scrofa domesticus) domestic pig farm animal (v1.0)
 * Pig farm, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, snout_cm, walk_speed, br_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,snt_cm,wk_sp,br_idx,age_yr,active;} pig_t;
typedef struct{int n_farm,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_snt,t_wk,t_br;} pig_state_t;
static pig_t arr_f[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static pig_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pig_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int ws,int bi,int ay){if(*cnt>=mx)return -1;pig_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->snt_cm=sc;x->wk_sp=ws;x->br_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PIG] Pig ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" ws=");pi(ws);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int pig_init(void){if(init)return -1;st.n_farm=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_snt=0;st.t_wk=0;st.t_br=0;for(int i=0;i<N;i++)arr_f[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[PIG] Pig initialized\n");return 0;}
int pig_farm(int lc,int bl,int bw,int sc,int ws,int bi,int ay){return add(arr_f,&st.n_farm,&st.t_ln,N,lc,bl,bw,sc,ws,bi,ay);}
int pig_feeding(int lc,int bl,int bw,int sc,int ws,int bi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,ws,bi,ay);}
int pig_breeding(int lc,int bl,int bw,int sc,int ws,int bi,int ay){return add(arr_b,&st.n_breed,&st.t_snt,N-4,lc,bl,bw,sc,ws,bi,ay);}
int pig_health(int lc,int bl,int bw,int sc,int ws,int bi,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,sc,ws,bi,ay);}
int pig_market(int lc,int bl,int bw,int sc,int ws,int bi,int ay){return add(arr_m,&st.n_mkt,&st.t_br,N-6,lc,bl,bw,sc,ws,bi,ay);}
void pig_report(void){ps("[PIG] Farm: ");pi(st.n_farm);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Snt=");pi(st.t_snt);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Br=");pi(st.t_br);ps("\n");}
void pig_state(void){ps("[PIG] Farm=");pi(st.n_farm);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Pig Admin Demo ===\n\n");pig_init();
ps("Pig farm...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*8),bw=80+(i*15),sc=15+(i*2),ws=3+(i*1),bi=(i%6)+1,ay=(i%10)+1;pig_farm(lc,bl,bw,sc,ws,bi,ay);}
ps("\nPig feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=125+(i*6),bw=85+(i*12),sc=16+(i*1),ws=4+(i*1),bi=(i%5)+1,ay=(i%8)+1;pig_feeding(lc,bl,bw,sc,ws,bi,ay);}
ps("\nPig breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=130+(i*5),bw=90+(i*10),sc=17+(i*1),ws=2+(i*2),bi=(i%4)+1,ay=(i%6)+1;pig_breeding(lc,bl,bw,sc,ws,bi,ay);}
ps("\nPig health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=115+(i*9),bw=75+(i*18),sc=14+(i*2),ws=2+(i*3),bi=(i%7)+1,ay=(i%5)+1;pig_health(lc,bl,bw,sc,ws,bi,ay);}
ps("\nPig market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=135+(i*4),bw=95+(i*8),sc=18+(i*1),ws=5+(i*1),bi=(i%3)+1,ay=(i%4)+1;pig_market(lc,bl,bw,sc,ws,bi,ay);}
ps("\n");pig_report();pig_state();ps("\n=== Demo Complete ===\n");return 0;}
