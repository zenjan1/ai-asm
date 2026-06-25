/* llama_admin: Llama (Lama glama) domesticated pack animal (v1.0)
 * Llama plateau, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, neck_cm, walk_speed, wool_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,nck_cm,wk_sp,wl_idx,age_yr,active;} llama_t;
typedef struct{int n_plateau,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_nck,t_wk,t_wl;} llama_state_t;
static llama_t plt[N],plf[N-2],plb[N-4],plh[N-6],plm[N-6]; static llama_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(llama_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nc,int ws,int wi,int ay){if(*cnt>=mx)return -1;llama_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->nck_cm=nc;x->wk_sp=ws;x->wl_idx=wi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[LLAMA] Llama ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nc=");pi(nc);ps(" ws=");pi(ws);ps(" wi=");pi(wi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int llama_init(void){if(init)return -1;st.n_plateau=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_nck=0;st.t_wk=0;st.t_wl=0;for(int i=0;i<N;i++)plt[i].active=0;for(int i=0;i<N-2;i++)plf[i].active=0;for(int i=0;i<N-4;i++)plb[i].active=0;for(int i=0;i<N-6;i++)plh[i].active=0;for(int i=0;i<N-6;i++)plm[i].active=0;init=1;ps("[LLAMA] Llama initialized\n");return 0;}
int llama_plateau(int lc,int bl,int bw,int nc,int ws,int wi,int ay){return add(plt,&st.n_plateau,&st.t_ln,N,lc,bl,bw,nc,ws,wi,ay);}
int llama_feeding(int lc,int bl,int bw,int nc,int ws,int wi,int ay){return add(plf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nc,ws,wi,ay);}
int llama_breeding(int lc,int bl,int bw,int nc,int ws,int wi,int ay){return add(plb,&st.n_breed,&st.t_nck,N-4,lc,bl,bw,nc,ws,wi,ay);}
int llama_health(int lc,int bl,int bw,int nc,int ws,int wi,int ay){return add(plh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,nc,ws,wi,ay);}
int llama_market(int lc,int bl,int bw,int nc,int ws,int wi,int ay){return add(plm,&st.n_mkt,&st.t_wl,N-6,lc,bl,bw,nc,ws,wi,ay);}
void llama_report(void){ps("[LLAMA] Plateau: ");pi(st.n_plateau);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Nck=");pi(st.t_nck);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Wl=");pi(st.t_wl);ps("\n");}
void llama_state(void){ps("[LLAMA] Plateau=");pi(st.n_plateau);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Llama Admin Demo ===\n\n");llama_init();
ps("Llama plateau...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=160+(i*6),bw=130+(i*10),nc=90+(i*5),ws=4+(i*1),wi=(i%8)+1,ay=(i%15)+1;llama_plateau(lc,bl,bw,nc,ws,wi,ay);}
ps("\nLlama feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=163+(i*5),bw=133+(i*8),nc=92+(i*4),ws=4+(i*1),wi=(i%7)+1,ay=(i%12)+1;llama_feeding(lc,bl,bw,nc,ws,wi,ay);}
ps("\nLlama breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=166+(i*4),bw=136+(i*7),nc=94+(i*3),ws=3+(i*2),wi=(i%6)+1,ay=(i%10)+1;llama_breeding(lc,bl,bw,nc,ws,wi,ay);}
ps("\nLlama health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=158+(i*7),bw=128+(i*12),nc=88+(i*6),ws=3+(i*2),wi=(i%9)+1,ay=(i%8)+1;llama_health(lc,bl,bw,nc,ws,wi,ay);}
ps("\nLlama market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=168+(i*4),bw=138+(i*6),nc=96+(i*3),ws=5+(i*1),wi=(i%5)+1,ay=(i%7)+1;llama_market(lc,bl,bw,nc,ws,wi,ay);}
ps("\n");llama_report();llama_state();ps("\n=== Demo Complete ===\n");return 0;}
