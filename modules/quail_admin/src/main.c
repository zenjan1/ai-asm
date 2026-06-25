/* quail_admin: Quail (Coturnix coturnix) common quail small game bird (v1.0)
 * Quail field, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, call_vol, plumage_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,cl_vl,pl_idx,age_yr,active;} quil_t;
typedef struct{int n_field,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_cl,t_pl;} quil_state_t;
static quil_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static quil_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(quil_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int cv,int pi2,int ay){if(*cnt>=mx)return -1;quil_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->cl_vl=cv;x->pl_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[QUIL] Quail ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" cv=");pi(cv);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int quil_init(void){if(init)return -1;st.n_field=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_cl=0;st.t_pl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[QUIL] Quail initialized\n");return 0;}
int quil_field(int lc,int bl,int bw,int wc,int cv,int pi2,int ay){return add(apl,&st.n_field,&st.t_ln,N,lc,bl,bw,wc,cv,pi2,ay);}
int quil_feeding(int lc,int bl,int bw,int wc,int cv,int pi2,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,cv,pi2,ay);}
int quil_breeding(int lc,int bl,int bw,int wc,int cv,int pi2,int ay){return add(abd,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,cv,pi2,ay);}
int quil_health(int lc,int bl,int bw,int wc,int cv,int pi2,int ay){return add(ahd,&st.n_health,&st.t_cl,N-6,lc,bl,bw,wc,cv,pi2,ay);}
int quil_market(int lc,int bl,int bw,int wc,int cv,int pi2,int ay){return add(amd,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,wc,cv,pi2,ay);}
void quil_report(void){ps("[QUIL] Field: ");pi(st.n_field);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void quil_state(void){ps("[QUIL] Field=");pi(st.n_field);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Quail Admin Demo ===\n\n");quil_init();
ps("Quail field...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=18+(i*2),bw=100+(i*15),wc=9+(i*2),cv=30+(i*5),pi2=(i%6)+1,ay=1+(i%3);quil_field(lc,bl,bw,wc,cv,pi2,ay);}
ps("\nQuail feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=19+(i*2),bw=105+(i*15),wc=10+(i*2),cv=32+(i*5),pi2=(i%5)+2,ay=1+(i%3);quil_feeding(lc,bl,bw,wc,cv,pi2,ay);}
ps("\nQuail breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=20+(i*2),bw=110+(i*15),wc=10+(i*2),cv=35+(i*5),pi2=(i%4)+1,ay=1+(i%2);quil_breeding(lc,bl,bw,wc,cv,pi2,ay);}
ps("\nQuail health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=17+(i*3),bw=95+(i*18),wc=8+(i*3),cv=28+(i*6),pi2=(i%3)+3,ay=1+(i%2);quil_health(lc,bl,bw,wc,cv,pi2,ay);}
ps("\nQuail market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=22+(i*2),bw=120+(i*15),wc=11+(i*2),cv=40+(i*5),pi2=(i%6)+1,ay=2+(i%2);quil_market(lc,bl,bw,wc,cv,pi2,ay);}
ps("\n");quil_report();quil_state();ps("\n=== Demo Complete ===\n");return 0;}
