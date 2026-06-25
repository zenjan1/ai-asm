/* dingo_admin: Dingo (Canis dingo) Australian wild canine (v1.0)
 * Dingo outback, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,run_sp,fr_idx,age_yr,active;} dng_t;
typedef struct{int n_outbk,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_run,t_fr;} dng_state_t;
static dng_t otl[N],otf[N-2],otb[N-4],oth[N-6],otm[N-6]; static dng_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dng_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int fi,int ay){if(*cnt>=mx)return -1;dng_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->run_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DNG] Dingo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dingo_init(void){if(init)return -1;st.n_outbk=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_run=0;st.t_fr=0;for(int i=0;i<N;i++)otl[i].active=0;for(int i=0;i<N-2;i++)otf[i].active=0;for(int i=0;i<N-4;i++)otb[i].active=0;for(int i=0;i<N-6;i++)oth[i].active=0;for(int i=0;i<N-6;i++)otm[i].active=0;init=1;ps("[DNG] Dingo initialized\n");return 0;}
int dingo_outback(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(otl,&st.n_outbk,&st.t_ln,N,lc,bl,bw,tc,rs,fi,ay);}
int dingo_feeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(otf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,fi,ay);}
int dingo_breeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(otb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,fi,ay);}
int dingo_health(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(oth,&st.n_health,&st.t_run,N-6,lc,bl,bw,tc,rs,fi,ay);}
int dingo_market(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(otm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,rs,fi,ay);}
void dingo_report(void){ps("[DNG] Outbk: ");pi(st.n_outbk);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void dingo_state(void){ps("[DNG] Outbk=");pi(st.n_outbk);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dingo Admin Demo ===\n\n");dingo_init();
ps("Dingo outback...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*6),bw=15+(i*4),tc=25+(i*3),rs=35+(i*4),fi=(i%8)+1,ay=(i%12)+1;dingo_outback(lc,bl,bw,tc,rs,fi,ay);}
ps("\nDingo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=94+(i*5),bw=17+(i*3),tc=27+(i*2),rs=37+(i*3),fi=(i%6)+1,ay=(i%10)+1;dingo_feeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nDingo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=98+(i*4),bw=19+(i*3),tc=29+(i*2),rs=33+(i*5),fi=(i%5)+1,ay=(i%8)+1;dingo_breeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nDingo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=88+(i*7),bw=14+(i*5),tc=24+(i*4),rs=30+(i*6),fi=(i%10)+1,ay=(i%7)+1;dingo_health(lc,bl,bw,tc,rs,fi,ay);}
ps("\nDingo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=100+(i*3),bw=20+(i*3),tc=30+(i*2),rs=40+(i*3),fi=(i%4)+1,ay=(i%6)+1;dingo_market(lc,bl,bw,tc,rs,fi,ay);}
ps("\n");dingo_report();dingo_state();ps("\n=== Demo Complete ===\n");return 0;}
