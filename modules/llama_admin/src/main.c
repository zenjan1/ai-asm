/* llama_admin: Llama (Lama glama) domestic llama camelid (v1.0)
 * Llama pasture, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, neck_cm, wool_g, spit_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,neck_cm,wl_g,sp_idx,age_yr,active;} llama_t;
typedef struct{int n_past,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_neck,t_wl,t_sp;} llama_state_t;
static llama_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static llama_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(llama_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nc,int wg,int si,int ay){if(*cnt>=mx)return -1;llama_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->neck_cm=nc;x->wl_g=wg;x->sp_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[LLAMA] Llama ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nc=");pi(nc);ps(" wg=");pi(wg);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int llama_init(void){if(init)return -1;st.n_past=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_neck=0;st.t_wl=0;st.t_sp=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[LLAMA] Llama initialized\n");return 0;}
int llama_pasture(int lc,int bl,int bw,int nc,int wg,int si,int ay){return add(gpl,&st.n_past,&st.t_ln,N,lc,bl,bw,nc,wg,si,ay);}
int llama_feeding(int lc,int bl,int bw,int nc,int wg,int si,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nc,wg,si,ay);}
int llama_breeding(int lc,int bl,int bw,int nc,int wg,int si,int ay){return add(gbr,&st.n_breed,&st.t_neck,N-4,lc,bl,bw,nc,wg,si,ay);}
int llama_health(int lc,int bl,int bw,int nc,int wg,int si,int ay){return add(ghl,&st.n_health,&st.t_wl,N-6,lc,bl,bw,nc,wg,si,ay);}
int llama_market(int lc,int bl,int bw,int nc,int wg,int si,int ay){return add(gmk,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,nc,wg,si,ay);}
void llama_report(void){ps("[LLAMA] Past: ");pi(st.n_past);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Neck=");pi(st.t_neck);ps("\nHlth: ");pi(st.n_health);ps(" Wl=");pi(st.t_wl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void llama_state(void){ps("[LLAMA] Past=");pi(st.n_past);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Llama Admin Demo ===\n\n");llama_init();
/* 1=pasture 2=farm 3=ranch 4=field 5=market */
ps("Llama pasture...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*6),bw=120+(i*10),nc=60+(i*3),wg=2000+(i*200),si=(i%5)+1,ay=(i%6)+1;llama_pasture(lc,bl,bw,nc,wg,si,ay);}
ps("\nLlama feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=155+(i*5),bw=125+(i*8),nc=62+(i*2),wg=2200+(i*150),si=(i%4)+1,ay=(i%5)+1;llama_feeding(lc,bl,bw,nc,wg,si,ay);}
ps("\nLlama breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=145+(i*7),bw=115+(i*12),nc=58+(i*4),wg=1800+(i*250),si=(i%3)+2,ay=(i%4)+1;llama_breeding(lc,bl,bw,nc,wg,si,ay);}
ps("\nLlama health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=160+(i*4),bw=130+(i*7),nc=65+(i*2),wg=2400+(i*120),si=(i%5)+1,ay=(i%5)+1;llama_health(lc,bl,bw,nc,wg,si,ay);}
ps("\nLlama market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=165+(i*3),bw=135+(i*6),nc=68+(i*2),wg=2600+(i*100),si=(i%4)+1,ay=(i%3)+2;llama_market(lc,bl,bw,nc,wg,si,ay);}
ps("\n");llama_report();llama_state();ps("\n=== Demo Complete ===\n");return 0;}
