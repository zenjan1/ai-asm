/* tammars_admin: Tammars (Notamacropus) tammar wallabies (v1.0)
 * Tammars forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, pouch_cm, tail_cm, tm_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,pouch_cm,tail_cm,tm_idx,age_yr,active;} tmmr_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_pouch,t_tail,t_tm;} tmmr_state_t;
static tmmr_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static tmmr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tmmr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int pc,int tc,int tm,int ay){if(*cnt>=mx)return -1;tmmr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->pouch_cm=pc;x->tail_cm=tc;x->tm_idx=tm;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TMMR] Tammar ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" pc=");pi(pc);ps(" tc=");pi(tc);ps(" tm=");pi(tm);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int tammars_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_pouch=0;st.t_tail=0;st.t_tm=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[TMMR] Tammars initialized\n");return 0;}
int tammars_forest(int lc,int bl,int bw,int pc,int tc,int tm,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,pc,tc,tm,ay);}
int tammars_feeding(int lc,int bl,int bw,int pc,int tc,int tm,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,pc,tc,tm,ay);}
int tammars_breeding(int lc,int bl,int bw,int pc,int tc,int tm,int ay){return add(svb,&st.n_breed,&st.t_pouch,N-4,lc,bl,bw,pc,tc,tm,ay);}
int tammars_health(int lc,int bl,int bw,int pc,int tc,int tm,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,pc,tc,tm,ay);}
int tammars_market(int lc,int bl,int bw,int pc,int tc,int tm,int ay){return add(svm,&st.n_mkt,&st.t_tm,N-6,lc,bl,bw,pc,tc,tm,ay);}
void tammars_report(void){ps("[TMMR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Pouch=");pi(st.t_pouch);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Tm=");pi(st.t_tm);ps("\n");}
void tammars_state(void){ps("[TMMR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tammars Admin Demo ===\n\n");tammars_init();
ps("Tammars forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=55+(i*2),bw=4+(i*1),pc=8+(i%3),tc=35+(i*2),tm=(i%8)+1,ay=(i%5)+1;tammars_forest(lc,bl,bw,pc,tc,tm,ay);}
ps("\nTammars feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=57+(i*2),bw=5+(i*1),pc=9+(i%2),tc=36+(i*1),tm=(i%6)+1,ay=(i%4)+1;tammars_feeding(lc,bl,bw,pc,tc,tm,ay);}
ps("\nTammars breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=60+(i*1),bw=5+(i*1),pc=10+(i%2),tc=38+(i*1),tm=(i%5)+1,ay=(i%3)+1;tammars_breeding(lc,bl,bw,pc,tc,tm,ay);}
ps("\nTammars health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=52+(i*3),bw=3+(i*1),pc=7+(i%3),tc=33+(i*2),tm=(i%10)+1,ay=(i%5)+1;tammars_health(lc,bl,bw,pc,tc,tm,ay);}
ps("\nTammars market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=62+(i*1),bw=6+(i*1),pc=11+(i%2),tc=40+(i*1),tm=(i%4)+1,ay=(i%3)+1;tammars_market(lc,bl,bw,pc,tc,tm,ay);}
ps("\n");tammars_report();tammars_state();ps("\n=== Demo Complete ===\n");return 0;}
