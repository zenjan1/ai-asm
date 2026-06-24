/* coyote_admin: Coyote (Canis latrans) wild canine management (v1.0)
 * Coyote housing, feeding, breeding, veterinary, market
 * Features: body_len_cm, body_wt_kg, territory_km, prey_idx, pup_count, health_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,terr_km,prey_idx,pup_ct,hlth_idx,active;} cyt_t;
typedef struct{int n_house,n_feed,n_breed,n_vet,n_mkt,t_ln,t_wt,t_terr,t_prey,t_pup;} cyt_state_t;
static cyt_t cyths[N],cytfd[N-2],cytbr[N-4],cytvt[N-6],cytmt[N-6]; static cyt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cyt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tk,int px,int pc,int hi){if(*cnt>=mx)return -1;cyt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->terr_km=tk;x->prey_idx=px;x->pup_ct=pc;x->hlth_idx=hi;x->active=1;*sum+=bl;(*cnt)++;ps("[CYT] Coyote ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tk=");pi(tk);ps(" px=");pi(px);ps(" pc=");pi(pc);ps(" hi=");pi(hi);ps("\n");return *cnt-1;}
int cyt_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_vet=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_terr=0;st.t_prey=0;st.t_pup=0;for(int i=0;i<N;i++)cyths[i].active=0;for(int i=0;i<N-2;i++)cytfd[i].active=0;for(int i=0;i<N-4;i++)cytbr[i].active=0;for(int i=0;i<N-6;i++)cytvt[i].active=0;for(int i=0;i<N-6;i++)cytmt[i].active=0;init=1;ps("[CYT] Coyote initialized\n");return 0;}
int cyt_housing(int lc,int bl,int bw,int tk,int px,int pc,int hi){return add(cyths,&st.n_house,&st.t_ln,N,lc,bl,bw,tk,px,pc,hi);}
int cyt_feeding(int lc,int bl,int bw,int tk,int px,int pc,int hi){return add(cytfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tk,px,pc,hi);}
int cyt_breeding(int lc,int bl,int bw,int tk,int px,int pc,int hi){return add(cytbr,&st.n_breed,&st.t_terr,N-4,lc,bl,bw,tk,px,pc,hi);}
int cyt_veterinary(int lc,int bl,int bw,int tk,int px,int pc,int hi){return add(cytvt,&st.n_vet,&st.t_prey,N-6,lc,bl,bw,tk,px,pc,hi);}
int cyt_market(int lc,int bl,int bw,int tk,int px,int pc,int hi){return add(cytmt,&st.n_mkt,&st.t_pup,N-6,lc,bl,bw,tk,px,pc,hi);}
void cyt_report(void){ps("[CYT] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Terr=");pi(st.t_terr);ps("\nVet: ");pi(st.n_vet);ps(" Prey=");pi(st.t_prey);ps("\nMkt: ");pi(st.n_mkt);ps(" Pup=");pi(st.t_pup);ps("\n");}
void cyt_state(void){ps("[CYT] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Vet=");pi(st.n_vet);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Coyote Admin Demo ===\n\n");cyt_init();
/* 1=wilderness 2=sanctuary 3=zoo 4=reserve 5=market */
ps("Coyote housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*8),bw=12+(i*3),tk=10+(i*4),px=(i%6)+1,pc=(i%5),hi=7+(i%4);cyt_housing(lc,bl,bw,tk,px,pc,hi);}
ps("\nCoyote feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=95+(i*7),bw=14+(i*3),tk=12+(i*3),px=(i%5)+1,pc=(i%4),hi=7+(i%3);cyt_feeding(lc,bl,bw,tk,px,pc,hi);}
ps("\nCoyote breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=100+(i*6),bw=16+(i*3),tk=14+(i*3),px=(i%4)+1,pc=1+(i%3),hi=8+(i%3);cyt_breeding(lc,bl,bw,tk,px,pc,hi);}
ps("\nCoyote veterinary...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=85+(i*10),bw=10+(i*4),tk=8+(i*5),px=(i%7)+1,pc=(i%6),hi=6+(i%5);cyt_veterinary(lc,bl,bw,tk,px,pc,hi);}
ps("\nCoyote market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=105+(i*5),bw=18+(i*2),tk=16+(i*2),px=(i%3)+4,pc=2+(i%2),hi=9+(i%2);cyt_market(lc,bl,bw,tk,px,pc,hi);}
ps("\n");cyt_report();cyt_state();ps("\n=== Demo Complete ===\n");return 0;}
