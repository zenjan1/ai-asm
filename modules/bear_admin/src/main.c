/* bear_admin: Bear (Ursus arctos) brown bear large mammal (v1.0)
 * Bear forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_cm, walk_speed, fr_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,wk_sp,fr_idx,age_yr,active;} bear_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_wk,t_fr;} bear_state_t;
static bear_t arr_f[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static bear_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bear_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int ws,int fri,int ay){if(*cnt>=mx)return -1;bear_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->wk_sp=ws;x->fr_idx=fri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BEAR] Bear ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" ws=");pi(ws);ps(" fri=");pi(fri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bear_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_wk=0;st.t_fr=0;for(int i=0;i<N;i++)arr_f[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[BEAR] Bear initialized\n");return 0;}
int bear_forest(int lc,int bl,int bw,int cc,int ws,int fri,int ay){return add(arr_f,&st.n_forest,&st.t_ln,N,lc,bl,bw,cc,ws,fri,ay);}
int bear_feeding(int lc,int bl,int bw,int cc,int ws,int fri,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,ws,fri,ay);}
int bear_breeding(int lc,int bl,int bw,int cc,int ws,int fri,int ay){return add(arr_b,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,ws,fri,ay);}
int bear_health(int lc,int bl,int bw,int cc,int ws,int fri,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,cc,ws,fri,ay);}
int bear_market(int lc,int bl,int bw,int cc,int ws,int fri,int ay){return add(arr_m,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,cc,ws,fri,ay);}
void bear_report(void){ps("[BEAR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void bear_state(void){ps("[BEAR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bear Admin Demo ===\n\n");bear_init();
ps("Bear forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=180+(i*10),bw=200+(i*30),cc=10+(i*2),ws=4+(i*1),fri=(i%6)+1,ay=(i%20)+1;bear_forest(lc,bl,bw,cc,ws,fri,ay);}
ps("\nBear feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=190+(i*8),bw=210+(i*25),cc=11+(i*2),ws=5+(i*1),fri=(i%5)+1,ay=(i%18)+1;bear_feeding(lc,bl,bw,cc,ws,fri,ay);}
ps("\nBear breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=200+(i*6),bw=220+(i*20),cc=12+(i*1),ws=3+(i*2),fri=(i%4)+1,ay=(i%15)+1;bear_breeding(lc,bl,bw,cc,ws,fri,ay);}
ps("\nBear health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=170+(i*12),bw=190+(i*35),cc=9+(i*2),ws=3+(i*3),fri=(i%7)+1,ay=(i%12)+1;bear_health(lc,bl,bw,cc,ws,fri,ay);}
ps("\nBear market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=210+(i*5),bw=230+(i*15),cc=13+(i*1),ws=6+(i*1),fri=(i%3)+1,ay=(i%10)+1;bear_market(lc,bl,bw,cc,ws,fri,ay);}
ps("\n");bear_report();bear_state();ps("\n=== Demo Complete ===\n");return 0;}
