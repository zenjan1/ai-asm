/* tuna_admin: Tuna (Thunnus thynnus) Atlantic bluefin tuna marine fish (v1.0)
 * Tuna pelagic, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fin_cm, swim_speed, fn_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,fn_idx,age_yr,active;} tuna_t;
typedef struct{int n_pelagic,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_fn;} tuna_state_t;
static tuna_t arr_p[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static tuna_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tuna_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int fni,int ay){if(*cnt>=mx)return -1;tuna_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->fn_idx=fni;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TUNA] Tuna ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" fni=");pi(fni);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int tuna_init(void){if(init)return -1;st.n_pelagic=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_fn=0;for(int i=0;i<N;i++)arr_p[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[TUNA] Tuna initialized\n");return 0;}
int tuna_pelagic(int lc,int bl,int bw,int fc,int ss,int fni,int ay){return add(arr_p,&st.n_pelagic,&st.t_ln,N,lc,bl,bw,fc,ss,fni,ay);}
int tuna_feeding(int lc,int bl,int bw,int fc,int ss,int fni,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,fni,ay);}
int tuna_breeding(int lc,int bl,int bw,int fc,int ss,int fni,int ay){return add(arr_b,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,fni,ay);}
int tuna_health(int lc,int bl,int bw,int fc,int ss,int fni,int ay){return add(arr_h,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,fni,ay);}
int tuna_market(int lc,int bl,int bw,int fc,int ss,int fni,int ay){return add(arr_m,&st.n_mkt,&st.t_fn,N-6,lc,bl,bw,fc,ss,fni,ay);}
void tuna_report(void){ps("[TUNA] Pelagic: ");pi(st.n_pelagic);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Fn=");pi(st.t_fn);ps("\n");}
void tuna_state(void){ps("[TUNA] Pelagic=");pi(st.n_pelagic);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tuna Admin Demo ===\n\n");tuna_init();
ps("Tuna pelagic...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*10),bw=100+(i*20),fc=30+(i*3),ss=40+(i*5),fni=(i%6)+1,ay=(i%15)+1;tuna_pelagic(lc,bl,bw,fc,ss,fni,ay);}
ps("\nTuna feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=160+(i*8),bw=110+(i*18),fc=32+(i*2),ss=42+(i*4),fni=(i%5)+1,ay=(i%12)+1;tuna_feeding(lc,bl,bw,fc,ss,fni,ay);}
ps("\nTuna breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=170+(i*6),bw=120+(i*15),fc=35+(i*2),ss=38+(i*6),fni=(i%4)+1,ay=(i%10)+1;tuna_breeding(lc,bl,bw,fc,ss,fni,ay);}
ps("\nTuna health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=140+(i*12),bw=90+(i*25),fc=28+(i*4),ss=35+(i*7),fni=(i%7)+1,ay=(i%8)+1;tuna_health(lc,bl,bw,fc,ss,fni,ay);}
ps("\nTuna market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=180+(i*5),bw=130+(i*12),fc=38+(i*2),ss=50+(i*4),fni=(i%3)+1,ay=(i%6)+1;tuna_market(lc,bl,bw,fc,ss,fni,ay);}
ps("\n");tuna_report();tuna_state();ps("\n=== Demo Complete ===\n");return 0;}
