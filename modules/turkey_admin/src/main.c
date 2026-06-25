/* turkey_admin: Turkey (Meleagris gallopavo) poultry bird (v1.0)
 * Turkey farm, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, run_speed, feather_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,run_sp,ft_idx,age_yr,active;} trky_t;
typedef struct{int n_farm,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_run,t_ft;} trky_state_t;
static trky_t tpl[N],tfd[N-2],tbr[N-4],thl[N-6],tmk[N-6]; static trky_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(trky_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int fi,int ay){if(*cnt>=mx)return -1;trky_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->run_sp=rs;x->ft_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TRKY] Turkey ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int trky_init(void){if(init)return -1;st.n_farm=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_run=0;st.t_ft=0;for(int i=0;i<N;i++)tpl[i].active=0;for(int i=0;i<N-2;i++)tfd[i].active=0;for(int i=0;i<N-4;i++)tbr[i].active=0;for(int i=0;i<N-6;i++)thl[i].active=0;for(int i=0;i<N-6;i++)tmk[i].active=0;init=1;ps("[TRKY] Turkey initialized\n");return 0;}
int trky_farm(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(tpl,&st.n_farm,&st.t_ln,N,lc,bl,bw,tc,rs,fi,ay);}
int trky_feeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(tfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,fi,ay);}
int trky_breeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(tbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,fi,ay);}
int trky_health(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(thl,&st.n_health,&st.t_run,N-6,lc,bl,bw,tc,rs,fi,ay);}
int trky_market(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(tmk,&st.n_mkt,&st.t_ft,N-6,lc,bl,bw,tc,rs,fi,ay);}
void trky_report(void){ps("[TRKY] Farm: ");pi(st.n_farm);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Ft=");pi(st.t_ft);ps("\n");}
void trky_state(void){ps("[TRKY] Farm=");pi(st.n_farm);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Turkey Admin Demo ===\n\n");trky_init();
ps("Turkey farm...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*8),bw=5+(i*3),tc=30+(i*4),rs=10+(i*2),fi=(i%6)+1,ay=(i%4)+1;trky_farm(lc,bl,bw,tc,rs,fi,ay);}
ps("\nTurkey feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=90+(i*6),bw=8+(i*2),tc=35+(i*3),rs=12+(i*2),fi=(i%5)+1,ay=(i%3)+1;trky_feeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nTurkey breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=70+(i*10),bw=4+(i*4),tc=25+(i*5),rs=8+(i*3),fi=(i%4)+1,ay=(i%3)+1;trky_breeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nTurkey health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=100+(i*5),bw=10+(i*2),tc=40+(i*2),rs=14+(i),fi=(i%7)+1,ay=(i%5)+2;trky_health(lc,bl,bw,tc,rs,fi,ay);}
ps("\nTurkey market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=110+(i*4),bw=12+(i),tc=45+(i),rs=7+(i*2),fi=(i%3)+3,ay=(i%4)+3;trky_market(lc,bl,bw,tc,rs,fi,ay);}
ps("\n");trky_report();trky_state();ps("\n=== Demo Complete ===\n");return 0;}
