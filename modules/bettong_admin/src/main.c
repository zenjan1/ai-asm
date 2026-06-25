/* bettong_admin: Bettong (Bettongia) rat-kangaroo nest-builder (v1.0)
 * Bettong burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, pouch_cm, claw_mm, bt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,pouch_cm,claw_mm,bt_idx,age_yr,active;} btng_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_pouch,t_claw,t_bt;} btng_state_t;
static btng_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static btng_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(btng_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int pc,int cm,int bt,int ay){if(*cnt>=mx)return -1;btng_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->pouch_cm=pc;x->claw_mm=cm;x->bt_idx=bt;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BTNG] Bettong ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" pc=");pi(pc);ps(" cm=");pi(cm);ps(" bt=");pi(bt);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int bettong_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_pouch=0;st.t_claw=0;st.t_bt=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[BTNG] Bettong initialized\n");return 0;}
int bettong_burrow(int lc,int bl,int bw,int pc,int cm,int bt,int ay){return add(svl,&st.n_burrow,&st.t_ln,N,lc,bl,bw,pc,cm,bt,ay);}
int bettong_feeding(int lc,int bl,int bw,int pc,int cm,int bt,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,pc,cm,bt,ay);}
int bettong_breeding(int lc,int bl,int bw,int pc,int cm,int bt,int ay){return add(svb,&st.n_breed,&st.t_pouch,N-4,lc,bl,bw,pc,cm,bt,ay);}
int bettong_health(int lc,int bl,int bw,int pc,int cm,int bt,int ay){return add(svh,&st.n_health,&st.t_claw,N-6,lc,bl,bw,pc,cm,bt,ay);}
int bettong_market(int lc,int bl,int bw,int pc,int cm,int bt,int ay){return add(svm,&st.n_mkt,&st.t_bt,N-6,lc,bl,bw,pc,cm,bt,ay);}
void bettong_report(void){ps("[BTNG] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Pouch=");pi(st.t_pouch);ps("\nHealth: ");pi(st.n_health);ps(" Claw=");pi(st.t_claw);ps("\nMkt: ");pi(st.n_mkt);ps(" Bt=");pi(st.t_bt);ps("\n");}
void bettong_state(void){ps("[BTNG] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bettong Admin Demo ===\n\n");bettong_init();
ps("Bettong burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*2),bw=1+(i*1),pc=5+(i%3),cm=4+(i%3),bt=(i%8)+1,ay=(i%5)+1;bettong_burrow(lc,bl,bw,pc,cm,bt,ay);}
ps("\nBettong feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=27+(i*1),bw=1+(i*1),pc=6+(i%2),cm=5+(i%2),bt=(i%6)+1,ay=(i%4)+1;bettong_feeding(lc,bl,bw,pc,cm,bt,ay);}
ps("\nBettong breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=29+(i*1),bw=2+(i*1),pc=6+(i%2),cm=5+(i%2),bt=(i%5)+1,ay=(i%4)+1;bettong_breeding(lc,bl,bw,pc,cm,bt,ay);}
ps("\nBettong health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=23+(i*3),bw=1+(i*1),pc=4+(i%3),cm=3+(i%3),bt=(i%10)+1,ay=(i%5)+1;bettong_health(lc,bl,bw,pc,cm,bt,ay);}
ps("\nBettong market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=31+(i*1),bw=2+(i*1),pc=7+(i%2),cm=6+(i%2),bt=(i%4)+1,ay=(i%3)+1;bettong_market(lc,bl,bw,pc,cm,bt,ay);}
ps("\n");bettong_report();bettong_state();ps("\n=== Demo Complete ===\n");return 0;}
