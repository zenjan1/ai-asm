/* coyote_admin: Coyote (Canis latrans) wild canine (v1.0)
 * Coyote desert, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,run_sp,fr_idx,age_yr,active;} cyot_t;
typedef struct{int n_desert,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_run,t_fr;} cyot_state_t;
static cyot_t dsl[N],dsf[N-2],dsb[N-4],dsh[N-6],dsm[N-6]; static cyot_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cyot_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int fi,int ay){if(*cnt>=mx)return -1;cyot_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->run_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CYOT] Coyote ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int coyote_init(void){if(init)return -1;st.n_desert=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_run=0;st.t_fr=0;for(int i=0;i<N;i++)dsl[i].active=0;for(int i=0;i<N-2;i++)dsf[i].active=0;for(int i=0;i<N-4;i++)dsb[i].active=0;for(int i=0;i<N-6;i++)dsh[i].active=0;for(int i=0;i<N-6;i++)dsm[i].active=0;init=1;ps("[CYOT] Coyote initialized\n");return 0;}
int coyote_desert(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dsl,&st.n_desert,&st.t_ln,N,lc,bl,bw,tc,rs,fi,ay);}
int coyote_feeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dsf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,fi,ay);}
int coyote_breeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dsb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,fi,ay);}
int coyote_health(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dsh,&st.n_health,&st.t_run,N-6,lc,bl,bw,tc,rs,fi,ay);}
int coyote_market(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dsm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,rs,fi,ay);}
void coyote_report(void){ps("[CYOT] Desert: ");pi(st.n_desert);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void coyote_state(void){ps("[CYOT] Desert=");pi(st.n_desert);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Coyote Admin Demo ===\n\n");coyote_init();
ps("Coyote desert...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*6),bw=10+(i*3),tc=30+(i*3),rs=30+(i*4),fi=(i%8)+1,ay=(i%12)+1;coyote_desert(lc,bl,bw,tc,rs,fi,ay);}
ps("\nCoyote feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=84+(i*5),bw=12+(i*2),tc=32+(i*2),rs=33+(i*3),fi=(i%6)+1,ay=(i%10)+1;coyote_feeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nCoyote breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=88+(i*4),bw=14+(i*2),tc=34+(i*2),rs=28+(i*5),fi=(i%5)+1,ay=(i%8)+1;coyote_breeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nCoyote health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=78+(i*7),bw=9+(i*4),tc=28+(i*4),rs=25+(i*6),fi=(i%10)+1,ay=(i%7)+1;coyote_health(lc,bl,bw,tc,rs,fi,ay);}
ps("\nCoyote market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=90+(i*3),bw=15+(i*2),tc=35+(i*2),rs=35+(i*3),fi=(i%4)+1,ay=(i%6)+1;coyote_market(lc,bl,bw,tc,rs,fi,ay);}
ps("\n");coyote_report();coyote_state();ps("\n=== Demo Complete ===\n");return 0;}
