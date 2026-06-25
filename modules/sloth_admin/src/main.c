/* sloth_admin: Sloth (Bradypus variegatus) arboreal mammal (v1.0)
 * Sloth canopy, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_cm, move_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,mv_sp,fr_idx,age_yr,active;} slth_t;
typedef struct{int n_canopy,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_mv,t_fr;} slth_state_t;
static slth_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static slth_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(slth_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int ms,int fi,int ay){if(*cnt>=mx)return -1;slth_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->mv_sp=ms;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SLTH] Sloth ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" ms=");pi(ms);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int slth_init(void){if(init)return -1;st.n_canopy=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_mv=0;st.t_fr=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SLTH] Sloth initialized\n");return 0;}
int slth_canopy(int lc,int bl,int bw,int cc,int ms,int fi,int ay){return add(spl,&st.n_canopy,&st.t_ln,N,lc,bl,bw,cc,ms,fi,ay);}
int slth_feeding(int lc,int bl,int bw,int cc,int ms,int fi,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,ms,fi,ay);}
int slth_breeding(int lc,int bl,int bw,int cc,int ms,int fi,int ay){return add(sbr,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,ms,fi,ay);}
int slth_health(int lc,int bl,int bw,int cc,int ms,int fi,int ay){return add(shl,&st.n_health,&st.t_mv,N-6,lc,bl,bw,cc,ms,fi,ay);}
int slth_market(int lc,int bl,int bw,int cc,int ms,int fi,int ay){return add(smk,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,cc,ms,fi,ay);}
void slth_report(void){ps("[SLTH] Canopy: ");pi(st.n_canopy);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Mv=");pi(st.t_mv);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void slth_state(void){ps("[SLTH] Canopy=");pi(st.n_canopy);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Sloth Admin Demo ===\n\n");slth_init();
ps("Sloth canopy...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*6),bw=5+(i*2),cc=8+(i%3),ms=0+(i%3),fi=(i%6)+1,ay=(i%5)+1;slth_canopy(lc,bl,bw,cc,ms,fi,ay);}
ps("\nSloth feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=55+(i*5),bw=6+(i*2),cc=9+(i%2),ms=1+(i%2),fi=(i%5)+1,ay=(i%4)+1;slth_feeding(lc,bl,bw,cc,ms,fi,ay);}
ps("\nSloth breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*7),bw=4+(i*3),cc=7+(i%4),ms=0+(i%2),fi=(i%4)+1,ay=(i%3)+1;slth_breeding(lc,bl,bw,cc,ms,fi,ay);}
ps("\nSloth health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=60+(i*4),bw=7+(i),cc=10+(i%2),ms=2+(i%2),fi=(i%7)+1,ay=(i%6)+2;slth_health(lc,bl,bw,cc,ms,fi,ay);}
ps("\nSloth market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=65+(i*3),bw=8+(i),cc=11+(i%2),ms=0+(i%3),fi=(i%3)+3,ay=(i%4)+3;slth_market(lc,bl,bw,cc,ms,fi,ay);}
ps("\n");slth_report();slth_state();ps("\n=== Demo Complete ===\n");return 0;}
