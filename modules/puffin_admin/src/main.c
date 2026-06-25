/* puffin_admin: Puffin (Fratercula arctica) Atlantic puffin seabird (v1.0)
 * Puffin cliff, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, fly_speed, bk_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,fl_sp,bk_idx,age_yr,active;} puff_t;
typedef struct{int n_cliff,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_fl,t_bk;} puff_state_t;
static puff_t arr_c[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static puff_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(puff_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int fs,int bki,int ay){if(*cnt>=mx)return -1;puff_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->fl_sp=fs;x->bk_idx=bki;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PUFF] Puffin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" fs=");pi(fs);ps(" bki=");pi(bki);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int puffin_init(void){if(init)return -1;st.n_cliff=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_fl=0;st.t_bk=0;for(int i=0;i<N;i++)arr_c[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[PUFF] Puffin initialized\n");return 0;}
int puffin_cliff(int lc,int bl,int bw,int bc,int fs,int bki,int ay){return add(arr_c,&st.n_cliff,&st.t_ln,N,lc,bl,bw,bc,fs,bki,ay);}
int puffin_feeding(int lc,int bl,int bw,int bc,int fs,int bki,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,fs,bki,ay);}
int puffin_breeding(int lc,int bl,int bw,int bc,int fs,int bki,int ay){return add(arr_b,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,fs,bki,ay);}
int puffin_health(int lc,int bl,int bw,int bc,int fs,int bki,int ay){return add(arr_h,&st.n_health,&st.t_fl,N-6,lc,bl,bw,bc,fs,bki,ay);}
int puffin_market(int lc,int bl,int bw,int bc,int fs,int bki,int ay){return add(arr_m,&st.n_mkt,&st.t_bk,N-6,lc,bl,bw,bc,fs,bki,ay);}
void puffin_report(void){ps("[PUFF] Cliff: ");pi(st.n_cliff);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Bk=");pi(st.t_bk);ps("\n");}
void puffin_state(void){ps("[PUFF] Cliff=");pi(st.n_cliff);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Puffin Admin Demo ===\n\n");puffin_init();
ps("Puffin cliff...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*2),bw=400+(i*30),bc=4+(i*1),fs=45+(i*4),bki=(i%6)+1,ay=(i%8)+1;puffin_cliff(lc,bl,bw,bc,fs,bki,ay);}
ps("\nPuffin feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=27+(i*1),bw=420+(i*25),bc=5+(i*1),fs=47+(i*3),bki=(i%5)+1,ay=(i%7)+1;puffin_feeding(lc,bl,bw,bc,fs,bki,ay);}
ps("\nPuffin breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=29+(i*1),bw=440+(i*20),bc=5+(i*1),fs=50+(i*3),bki=(i%4)+1,ay=(i%6)+1;puffin_breeding(lc,bl,bw,bc,fs,bki,ay);}
ps("\nPuffin health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=23+(i*3),bw=380+(i*35),bc=3+(i*1),fs=43+(i*5),bki=(i%7)+1,ay=(i%5)+1;puffin_health(lc,bl,bw,bc,fs,bki,ay);}
ps("\nPuffin market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=31+(i*1),bw=460+(i*15),bc=6+(i*1),fs=53+(i*3),bki=(i%3)+1,ay=(i%4)+1;puffin_market(lc,bl,bw,bc,fs,bki,ay);}
ps("\n");puffin_report();puffin_state();ps("\n=== Demo Complete ===\n");return 0;}
