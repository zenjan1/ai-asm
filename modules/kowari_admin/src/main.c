/* kowari_admin: Kowari (Dasyuroides) small dasyurid marsupials (v1.0)
 * Kowari forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, claw_ct, tail_cm, kw_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_ct,tail_cm,kw_idx,age_yr,active;} kwar_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_tail,t_kw;} kwar_state_t;
static kwar_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static kwar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kwar_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ct,int tc,int kw,int ay){if(*cnt>=mx)return -1;kwar_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_ct=ct;x->tail_cm=tc;x->kw_idx=kw;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KWAR] Kowari ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ct=");pi(ct);ps(" tc=");pi(tc);ps(" kw=");pi(kw);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int kowari_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_tail=0;st.t_kw=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[KWAR] Kowari initialized\n");return 0;}
int kowari_forest(int lc,int bl,int bw,int ct,int tc,int kw,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ct,tc,kw,ay);}
int kowari_feeding(int lc,int bl,int bw,int ct,int tc,int kw,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ct,tc,kw,ay);}
int kowari_breeding(int lc,int bl,int bw,int ct,int tc,int kw,int ay){return add(svb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,ct,tc,kw,ay);}
int kowari_health(int lc,int bl,int bw,int ct,int tc,int kw,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,ct,tc,kw,ay);}
int kowari_market(int lc,int bl,int bw,int ct,int tc,int kw,int ay){return add(svm,&st.n_mkt,&st.t_kw,N-6,lc,bl,bw,ct,tc,kw,ay);}
void kowari_report(void){ps("[KWAR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Kw=");pi(st.t_kw);ps("\n");}
void kowari_state(void){ps("[KWAR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Kowari Admin Demo ===\n\n");kowari_init();
ps("Kowari forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=18+(i*1),bw=60+(i*5),ct=5+(i%3),tc=14+(i*1),kw=(i%8)+1,ay=(i%5)+1;kowari_forest(lc,bl,bw,ct,tc,kw,ay);}
ps("\nKowari feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=19+(i*1),bw=65+(i*4),ct=5+(i%3),tc=15+(i*1),kw=(i%6)+1,ay=(i%4)+1;kowari_feeding(lc,bl,bw,ct,tc,kw,ay);}
ps("\nKowari breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=20+(i*1),bw=70+(i*3),ct=6+(i%3),tc=16+(i*1),kw=(i%5)+1,ay=(i%3)+1;kowari_breeding(lc,bl,bw,ct,tc,kw,ay);}
ps("\nKowari health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=17+(i*2),bw=55+(i*6),ct=4+(i%3),tc=13+(i*1),kw=(i%10)+1,ay=(i%5)+1;kowari_health(lc,bl,bw,ct,tc,kw,ay);}
ps("\nKowari market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=21+(i*1),bw=75+(i*3),ct=6+(i%3),tc=17+(i*1),kw=(i%4)+1,ay=(i%3)+1;kowari_market(lc,bl,bw,ct,tc,kw,ay);}
ps("\n");kowari_report();kowari_state();ps("\n=== Demo Complete ===\n");return 0;}
