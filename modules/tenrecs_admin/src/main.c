/* tenrecs_admin: Tenrecs (Tenrec) insectivorous mammals (v1.0)
 * Tenrecs forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, claw_cm, tail_cm, tr_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,tail_cm,tr_idx,age_yr,active;} tnrc_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_tail,t_tr;} tnrc_state_t;
static tnrc_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static tnrc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tnrc_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int tc,int tr,int ay){if(*cnt>=mx)return -1;tnrc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->tail_cm=tc;x->tr_idx=tr;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TNRC] Tenrec ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" tc=");pi(tc);ps(" tr=");pi(tr);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int tenrecs_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_tail=0;st.t_tr=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[TNRC] Tenrecs initialized\n");return 0;}
int tenrecs_forest(int lc,int bl,int bw,int cc,int tc,int tr,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,cc,tc,tr,ay);}
int tenrecs_feeding(int lc,int bl,int bw,int cc,int tc,int tr,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,tc,tr,ay);}
int tenrecs_breeding(int lc,int bl,int bw,int cc,int tc,int tr,int ay){return add(svb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,tc,tr,ay);}
int tenrecs_health(int lc,int bl,int bw,int cc,int tc,int tr,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,cc,tc,tr,ay);}
int tenrecs_market(int lc,int bl,int bw,int cc,int tc,int tr,int ay){return add(svm,&st.n_mkt,&st.t_tr,N-6,lc,bl,bw,cc,tc,tr,ay);}
void tenrecs_report(void){ps("[TNRC] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Tr=");pi(st.t_tr);ps("\n");}
void tenrecs_state(void){ps("[TNRC] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tenrecs Admin Demo ===\n\n");tenrecs_init();
ps("Tenrecs forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*1),bw=80+(i*8),cc=1+(i%2),tc=3+(i*1),tr=(i%8)+1,ay=(i%5)+1;tenrecs_forest(lc,bl,bw,cc,tc,tr,ay);}
ps("\nTenrecs feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=21+(i*1),bw=85+(i*6),cc=1+(i%2),tc=3+(i*1),tr=(i%6)+1,ay=(i%4)+1;tenrecs_feeding(lc,bl,bw,cc,tc,tr,ay);}
ps("\nTenrecs breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=22+(i*1),bw=90+(i*5),cc=2+(i%2),tc=4+(i*1),tr=(i%5)+1,ay=(i%3)+1;tenrecs_breeding(lc,bl,bw,cc,tc,tr,ay);}
ps("\nTenrecs health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=19+(i*2),bw=75+(i*10),cc=1+(i%2),tc=2+(i*1),tr=(i%10)+1,ay=(i%5)+1;tenrecs_health(lc,bl,bw,cc,tc,tr,ay);}
ps("\nTenrecs market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=23+(i*1),bw=95+(i*4),cc=2+(i%2),tc=5+(i*1),tr=(i%4)+1,ay=(i%3)+1;tenrecs_market(lc,bl,bw,cc,tc,tr,ay);}
ps("\n");tenrecs_report();tenrecs_state();ps("\n=== Demo Complete ===\n");return 0;}
