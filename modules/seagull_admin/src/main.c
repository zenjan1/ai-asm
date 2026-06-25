/* seagull_admin: Seagull (Larus argentatus) herring gull coastal seabird (v1.0)
 * Seagull coast, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wingspan_cm, fly_speed, bk_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wn_sp,fl_sp,bk_idx,age_yr,active;} sgul_t;
typedef struct{int n_coast,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wn,t_fl,t_bk;} sgul_state_t;
static sgul_t arr_c[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static sgul_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sgul_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int fs,int bki,int ay){if(*cnt>=mx)return -1;sgul_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wn_sp=ws;x->fl_sp=fs;x->bk_idx=bki;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SGUL] Seagull ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" fs=");pi(fs);ps(" bki=");pi(bki);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int seagull_init(void){if(init)return -1;st.n_coast=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wn=0;st.t_fl=0;st.t_bk=0;for(int i=0;i<N;i++)arr_c[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[SGUL] Seagull initialized\n");return 0;}
int seagull_coast(int lc,int bl,int bw,int ws,int fs,int bki,int ay){return add(arr_c,&st.n_coast,&st.t_ln,N,lc,bl,bw,ws,fs,bki,ay);}
int seagull_feeding(int lc,int bl,int bw,int ws,int fs,int bki,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,fs,bki,ay);}
int seagull_breeding(int lc,int bl,int bw,int ws,int fs,int bki,int ay){return add(arr_b,&st.n_breed,&st.t_wn,N-4,lc,bl,bw,ws,fs,bki,ay);}
int seagull_health(int lc,int bl,int bw,int ws,int fs,int bki,int ay){return add(arr_h,&st.n_health,&st.t_fl,N-6,lc,bl,bw,ws,fs,bki,ay);}
int seagull_market(int lc,int bl,int bw,int ws,int fs,int bki,int ay){return add(arr_m,&st.n_mkt,&st.t_bk,N-6,lc,bl,bw,ws,fs,bki,ay);}
void seagull_report(void){ps("[SGUL] Coast: ");pi(st.n_coast);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wn=");pi(st.t_wn);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Bk=");pi(st.t_bk);ps("\n");}
void seagull_state(void){ps("[SGUL] Coast=");pi(st.n_coast);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Seagull Admin Demo ===\n\n");seagull_init();
ps("Seagull coast...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=55+(i*3),bw=500+(i*40),ws=140+(i*8),fs=40+(i*4),bki=(i%6)+1,ay=(i%10)+1;seagull_coast(lc,bl,bw,ws,fs,bki,ay);}
ps("\nSeagull feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=57+(i*2),bw=520+(i*30),ws=145+(i*6),fs=42+(i*3),bki=(i%5)+1,ay=(i%8)+1;seagull_feeding(lc,bl,bw,ws,fs,bki,ay);}
ps("\nSeagull breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=60+(i*2),bw=540+(i*25),ws=150+(i*5),fs=45+(i*3),bki=(i%4)+1,ay=(i%6)+1;seagull_breeding(lc,bl,bw,ws,fs,bki,ay);}
ps("\nSeagull health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=52+(i*4),bw=480+(i*50),ws=135+(i*9),fs=38+(i*5),bki=(i%7)+1,ay=(i%5)+1;seagull_health(lc,bl,bw,ws,fs,bki,ay);}
ps("\nSeagull market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=63+(i*2),bw=560+(i*20),ws=155+(i*4),fs=48+(i*3),bki=(i%3)+1,ay=(i%4)+1;seagull_market(lc,bl,bw,ws,fs,bki,ay);}
ps("\n");seagull_report();seagull_state();ps("\n=== Demo Complete ===\n");return 0;}
