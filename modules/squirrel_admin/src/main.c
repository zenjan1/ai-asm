/* squirrel_admin: Squirrel (Sciurus carolinensis) rodent (v1.0)
 * Squirrel park, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,run_sp,fr_idx,age_yr,active;} sqrl_t;
typedef struct{int n_park,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_run,t_fr;} sqrl_state_t;
static sqrl_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static sqrl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sqrl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int fi,int ay){if(*cnt>=mx)return -1;sqrl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->run_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SQRL] Squirrel ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int sqrl_init(void){if(init)return -1;st.n_park=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_run=0;st.t_fr=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SQRL] Squirrel initialized\n");return 0;}
int sqrl_park(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(spl,&st.n_park,&st.t_ln,N,lc,bl,bw,tc,rs,fi,ay);}
int sqrl_feeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,fi,ay);}
int sqrl_breeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(sbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,fi,ay);}
int sqrl_health(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(shl,&st.n_health,&st.t_run,N-6,lc,bl,bw,tc,rs,fi,ay);}
int sqrl_market(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(smk,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,rs,fi,ay);}
void sqrl_report(void){ps("[SQRL] Park: ");pi(st.n_park);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void sqrl_state(void){ps("[SQRL] Park=");pi(st.n_park);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Squirrel Admin Demo ===\n\n");sqrl_init();
ps("Squirrel park...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*4),bw=0+(i%3),tc=18+(i*3),rs=12+(i*3),fi=(i%6)+1,ay=(i%4)+1;sqrl_park(lc,bl,bw,tc,rs,fi,ay);}
ps("\nSquirrel feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=28+(i*3),bw=0+(i%4),tc=20+(i*2),rs=14+(i*2),fi=(i%5)+1,ay=(i%3)+1;sqrl_feeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nSquirrel breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=22+(i*5),bw=0+(i%2),tc=16+(i*4),rs=10+(i*4),fi=(i%4)+1,ay=(i%3)+1;sqrl_breeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nSquirrel health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=30+(i*2),bw=1+(i%3),tc=22+(i*2),rs=16+(i*2),fi=(i%7)+1,ay=(i%5)+2;sqrl_health(lc,bl,bw,tc,rs,fi,ay);}
ps("\nSquirrel market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=35+(i),bw=1+(i%2),tc=25+(i),rs=8+(i*3),fi=(i%3)+3,ay=(i%4)+3;sqrl_market(lc,bl,bw,tc,rs,fi,ay);}
ps("\n");sqrl_report();sqrl_state();ps("\n=== Demo Complete ===\n");return 0;}
