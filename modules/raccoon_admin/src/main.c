/* raccoon_admin: Raccoon (Procyon lotor) common raccoon masked mammal (v1.0)
 * Raccoon den, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, paw_idx, night_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,pw_idx,ng_vl,age_yr,active;} racn_t;
typedef struct{int n_den,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_pw,t_ng;} racn_state_t;
static racn_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static racn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(racn_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int pi2,int nv,int ay){if(*cnt>=mx)return -1;racn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->pw_idx=pi2;x->ng_vl=nv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[RACN] Raccoon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" pi=");pi(pi2);ps(" nv=");pi(nv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int racn_init(void){if(init)return -1;st.n_den=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_pw=0;st.t_ng=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[RACN] Raccoon initialized\n");return 0;}
int racn_den(int lc,int bl,int bw,int tl,int pi2,int nv,int ay){return add(apl,&st.n_den,&st.t_ln,N,lc,bl,bw,tl,pi2,nv,ay);}
int racn_feeding(int lc,int bl,int bw,int tl,int pi2,int nv,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,pi2,nv,ay);}
int racn_breeding(int lc,int bl,int bw,int tl,int pi2,int nv,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,pi2,nv,ay);}
int racn_health(int lc,int bl,int bw,int tl,int pi2,int nv,int ay){return add(ahd,&st.n_health,&st.t_pw,N-6,lc,bl,bw,tl,pi2,nv,ay);}
int racn_market(int lc,int bl,int bw,int tl,int pi2,int nv,int ay){return add(amd,&st.n_mkt,&st.t_ng,N-6,lc,bl,bw,tl,pi2,nv,ay);}
void racn_report(void){ps("[RACN] Den: ");pi(st.n_den);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Pw=");pi(st.t_pw);ps("\nMkt: ");pi(st.n_mkt);ps(" Ng=");pi(st.t_ng);ps("\n");}
void racn_state(void){ps("[RACN] Den=");pi(st.n_den);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Raccoon Admin Demo ===\n\n");racn_init();
ps("Raccoon den...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*4),bw=6+(i*2),tl=30+(i*3),pi2=(i%7)+1,nv=30+(i*5),ay=2+(i%10);racn_den(lc,bl,bw,tl,pi2,nv,ay);}
ps("\nRaccoon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=62+(i*4),bw=7+(i*2),tl=32+(i*3),pi2=(i%6)+2,nv=32+(i*5),ay=3+(i%8);racn_feeding(lc,bl,bw,tl,pi2,nv,ay);}
ps("\nRaccoon breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=65+(i*4),bw=7+(i*2),tl=34+(i*3),pi2=(i%5)+1,nv=35+(i*5),ay=4+(i%6);racn_breeding(lc,bl,bw,tl,pi2,nv,ay);}
ps("\nRaccoon health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=58+(i*5),bw=5+(i*3),tl=28+(i*4),pi2=(i%4)+3,nv=28+(i*6),ay=5+(i%5);racn_health(lc,bl,bw,tl,pi2,nv,ay);}
ps("\nRaccoon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=70+(i*4),bw=8+(i*2),tl=36+(i*3),pi2=(i%3)+4,nv=40+(i*5),ay=6+(i%4);racn_market(lc,bl,bw,tl,pi2,nv,ay);}
ps("\n");racn_report();racn_state();ps("\n=== Demo Complete ===\n");return 0;}
