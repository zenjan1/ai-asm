/* jackal_admin: Jackal (Canis aureus) golden wild canid mammal (v1.0)
 * Jackal scrub, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,rn_sp,fr_idx,age_yr,active;} jack_t;
typedef struct{int n_scrub,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_rn,t_fr;} jack_state_t;
static jack_t scr[N],scf[N-2],scb[N-4],sch[N-6],scm[N-6]; static jack_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jack_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int rs,int fi,int ay){if(*cnt>=mx)return -1;jack_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->rn_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[JACK] Jackal ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int jackal_init(void){if(init)return -1;st.n_scrub=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_rn=0;st.t_fr=0;for(int i=0;i<N;i++)scr[i].active=0;for(int i=0;i<N-2;i++)scf[i].active=0;for(int i=0;i<N-4;i++)scb[i].active=0;for(int i=0;i<N-6;i++)sch[i].active=0;for(int i=0;i<N-6;i++)scm[i].active=0;init=1;ps("[JACK] Jackal initialized\n");return 0;}
int jackal_scrub(int lc,int bl,int bw,int ec,int rs,int fi,int ay){return add(scr,&st.n_scrub,&st.t_ln,N,lc,bl,bw,ec,rs,fi,ay);}
int jackal_feeding(int lc,int bl,int bw,int ec,int rs,int fi,int ay){return add(scf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,rs,fi,ay);}
int jackal_breeding(int lc,int bl,int bw,int ec,int rs,int fi,int ay){return add(scb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,rs,fi,ay);}
int jackal_health(int lc,int bl,int bw,int ec,int rs,int fi,int ay){return add(sch,&st.n_health,&st.t_rn,N-6,lc,bl,bw,ec,rs,fi,ay);}
int jackal_market(int lc,int bl,int bw,int ec,int rs,int fi,int ay){return add(scm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,ec,rs,fi,ay);}
void jackal_report(void){ps("[JACK] Scrub: ");pi(st.n_scrub);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void jackal_state(void){ps("[JACK] Scrub=");pi(st.n_scrub);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Jackal Admin Demo ===\n\n");jackal_init();
ps("Jackal scrub...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*5),bw=12+(i*2),ec=8+(i*1),rs=14+(i*3),fi=(i%8)+1,ay=(i%12)+1;jackal_scrub(lc,bl,bw,ec,rs,fi,ay);}
ps("\nJackal feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=83+(i*4),bw=13+(i*2),ec=8+(i*1),rs=15+(i*2),fi=(i%7)+1,ay=(i%10)+1;jackal_feeding(lc,bl,bw,ec,rs,fi,ay);}
ps("\nJackal breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=86+(i*3),bw=14+(i*1),ec=9+(i*1),rs=13+(i*4),fi=(i%6)+1,ay=(i%8)+1;jackal_breeding(lc,bl,bw,ec,rs,fi,ay);}
ps("\nJackal health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=78+(i*6),bw=11+(i*3),ec=7+(i*2),rs=12+(i*5),fi=(i%9)+1,ay=(i%7)+1;jackal_health(lc,bl,bw,ec,rs,fi,ay);}
ps("\nJackal market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=88+(i*3),bw=15+(i*1),ec=9+(i*1),rs=16+(i*2),fi=(i%5)+1,ay=(i%6)+1;jackal_market(lc,bl,bw,ec,rs,fi,ay);}
ps("\n");jackal_report();jackal_state();ps("\n=== Demo Complete ===\n");return 0;}
