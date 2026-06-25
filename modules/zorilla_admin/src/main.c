/* zorilla_admin: Zorilla (Ictonyx striatus) striped polecat (v1.0)
 * Zorilla grassland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, sprint_speed, sc_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,sp_sp,sc_idx,age_yr,active;} zorl_t;
typedef struct{int n_grass,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_sp,t_sc;} zorl_state_t;
static zorl_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static zorl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zorl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int si,int ay){if(*cnt>=mx)return -1;zorl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->sp_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ZORL] Zorilla ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int zorilla_init(void){if(init)return -1;st.n_grass=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_sp=0;st.t_sc=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[ZORL] Zorilla initialized\n");return 0;}
int zorilla_grassland(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(svl,&st.n_grass,&st.t_ln,N,lc,bl,bw,tc,ss,si,ay);}
int zorilla_feeding(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,si,ay);}
int zorilla_breeding(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(svb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,ss,si,ay);}
int zorilla_health(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(svh,&st.n_health,&st.t_sp,N-6,lc,bl,bw,tc,ss,si,ay);}
int zorilla_market(int lc,int bl,int bw,int tc,int ss,int si,int ay){return add(svm,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,tc,ss,si,ay);}
void zorilla_report(void){ps("[ZORL] Grass: ");pi(st.n_grass);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Sp=");pi(st.t_sp);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void zorilla_state(void){ps("[ZORL] Grass=");pi(st.n_grass);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Zorilla Admin Demo ===\n\n");zorilla_init();
ps("Zorilla grassland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*2),bw=500+(i*50),tc=20+(i*2),ds=8+(i*2),fi=(i%8)+1,ay=(i%10)+1;zorilla_grassland(lc,bl,bw,tc,ds,fi,ay);}
ps("\nZorilla feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*2),bw=520+(i*40),tc=21+(i*1),ds=9+(i*1),fi=(i%6)+1,ay=(i%8)+1;zorilla_feeding(lc,bl,bw,tc,ds,fi,ay);}
ps("\nZorilla breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=38+(i*1),bw=540+(i*30),tc=22+(i*1),ds=7+(i*3),fi=(i%5)+1,ay=(i%7)+1;zorilla_breeding(lc,bl,bw,tc,ds,fi,ay);}
ps("\nZorilla health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=34+(i*3),bw=480+(i*60),tc=19+(i*2),ds=6+(i*3),fi=(i%10)+1,ay=(i%6)+1;zorilla_health(lc,bl,bw,tc,ds,fi,ay);}
ps("\nZorilla market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=40+(i*1),bw=560+(i*20),tc=23+(i*1),ds=10+(i*1),fi=(i%4)+1,ay=(i%5)+1;zorilla_market(lc,bl,bw,tc,ds,fi,ay);}
ps("\n");zorilla_report();zorilla_state();ps("\n=== Demo Complete ===\n");return 0;}
