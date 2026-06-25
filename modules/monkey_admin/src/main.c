/* monkey_admin: Monkey (Macaca fascicularis) crab-eating agile primate (v1.0)
 * Monkey jungle, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, climb_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,cl_sp,fr_idx,age_yr,active;} monk_t;
typedef struct{int n_jungle,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cl,t_fr;} monk_state_t;
static monk_t jng[N],jnf[N-2],jnb[N-4],jnh[N-6],jnm[N-6]; static monk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(monk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int cs,int fi,int ay){if(*cnt>=mx)return -1;monk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->cl_sp=cs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MONK] Monkey ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" cs=");pi(cs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int monkey_init(void){if(init)return -1;st.n_jungle=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cl=0;st.t_fr=0;for(int i=0;i<N;i++)jng[i].active=0;for(int i=0;i<N-2;i++)jnf[i].active=0;for(int i=0;i<N-4;i++)jnb[i].active=0;for(int i=0;i<N-6;i++)jnh[i].active=0;for(int i=0;i<N-6;i++)jnm[i].active=0;init=1;ps("[MONK] Monkey initialized\n");return 0;}
int monkey_jungle(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(jng,&st.n_jungle,&st.t_ln,N,lc,bl,bw,tc,cs,fi,ay);}
int monkey_feeding(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(jnf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,cs,fi,ay);}
int monkey_breeding(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(jnb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,cs,fi,ay);}
int monkey_health(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(jnh,&st.n_health,&st.t_cl,N-6,lc,bl,bw,tc,cs,fi,ay);}
int monkey_market(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(jnm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,cs,fi,ay);}
void monkey_report(void){ps("[MONK] Jungle: ");pi(st.n_jungle);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void monkey_state(void){ps("[MONK] Jungle=");pi(st.n_jungle);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Monkey Admin Demo ===\n\n");monkey_init();
ps("Monkey jungle...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=55+(i*4),bw=5+(i*1),tc=50+(i*3),cs=8+(i*2),fi=(i%8)+1,ay=(i%18)+1;monkey_jungle(lc,bl,bw,tc,cs,fi,ay);}
ps("\nMonkey feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=57+(i*3),bw=5+(i*1),tc=51+(i*2),cs=9+(i*2),fi=(i%7)+1,ay=(i%15)+1;monkey_feeding(lc,bl,bw,tc,cs,fi,ay);}
ps("\nMonkey breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=59+(i*3),bw=6+(i*1),tc=52+(i*2),cs=7+(i*3),fi=(i%6)+1,ay=(i%12)+1;monkey_breeding(lc,bl,bw,tc,cs,fi,ay);}
ps("\nMonkey health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=53+(i*5),bw=4+(i*1),tc=48+(i*4),cs=6+(i*4),fi=(i%9)+1,ay=(i%10)+1;monkey_health(lc,bl,bw,tc,cs,fi,ay);}
ps("\nMonkey market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=61+(i*2),bw=6+(i*1),tc=54+(i*2),cs=10+(i*2),fi=(i%5)+1,ay=(i%8)+1;monkey_market(lc,bl,bw,tc,cs,fi,ay);}
ps("\n");monkey_report();monkey_state();ps("\n=== Demo Complete ===\n");return 0;}
