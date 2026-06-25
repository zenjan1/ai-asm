/* badger_admin: Badger (Meles meles) European burrowing mustelid (v1.0)
 * Badger setts, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_cm, dig_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,dg_sp,fr_idx,age_yr,active;} bdgr_t;
typedef struct{int n_sett,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_dg,t_fr;} bdgr_state_t;
static bdgr_t stl[N],stf[N-2],stb[N-4],sth[N-6],stm[N-6]; static bdgr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bdgr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int ds,int fi,int ay){if(*cnt>=mx)return -1;bdgr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->dg_sp=ds;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BDGR] Badger ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" ds=");pi(ds);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int badger_init(void){if(init)return -1;st.n_sett=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_dg=0;st.t_fr=0;for(int i=0;i<N;i++)stl[i].active=0;for(int i=0;i<N-2;i++)stf[i].active=0;for(int i=0;i<N-4;i++)stb[i].active=0;for(int i=0;i<N-6;i++)sth[i].active=0;for(int i=0;i<N-6;i++)stm[i].active=0;init=1;ps("[BDGR] Badger initialized\n");return 0;}
int badger_setts(int lc,int bl,int bw,int cc,int ds,int fi,int ay){return add(stl,&st.n_sett,&st.t_ln,N,lc,bl,bw,cc,ds,fi,ay);}
int badger_feeding(int lc,int bl,int bw,int cc,int ds,int fi,int ay){return add(stf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,ds,fi,ay);}
int badger_breeding(int lc,int bl,int bw,int cc,int ds,int fi,int ay){return add(stb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,ds,fi,ay);}
int badger_health(int lc,int bl,int bw,int cc,int ds,int fi,int ay){return add(sth,&st.n_health,&st.t_dg,N-6,lc,bl,bw,cc,ds,fi,ay);}
int badger_market(int lc,int bl,int bw,int cc,int ds,int fi,int ay){return add(stm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,cc,ds,fi,ay);}
void badger_report(void){ps("[BDGR] Sett: ");pi(st.n_sett);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Dg=");pi(st.t_dg);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void badger_state(void){ps("[BDGR] Sett=");pi(st.n_sett);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Badger Admin Demo ===\n\n");badger_init();
ps("Badger setts...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=55+(i*4),bw=8+(i*3),cc=3+(i%4),ds=4+(i*2),fi=(i%8)+1,ay=(i%12)+1;badger_setts(lc,bl,bw,cc,ds,fi,ay);}
ps("\nBadger feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=58+(i*3),bw=9+(i*2),cc=4+(i%3),ds=5+(i*2),fi=(i%6)+1,ay=(i%10)+1;badger_feeding(lc,bl,bw,cc,ds,fi,ay);}
ps("\nBadger breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=60+(i*3),bw=10+(i*2),cc=3+(i%5),ds=3+(i*3),fi=(i%5)+1,ay=(i%8)+1;badger_breeding(lc,bl,bw,cc,ds,fi,ay);}
ps("\nBadger health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=53+(i*5),bw=7+(i*4),cc=2+(i%6),ds=2+(i*4),fi=(i%10)+1,ay=(i%7)+1;badger_health(lc,bl,bw,cc,ds,fi,ay);}
ps("\nBadger market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=62+(i*3),bw=11+(i*2),cc=4+(i%3),ds=6+(i*2),fi=(i%4)+1,ay=(i%6)+1;badger_market(lc,bl,bw,cc,ds,fi,ay);}
ps("\n");badger_report();badger_state();ps("\n=== Demo Complete ===\n");return 0;}
