/* cougau_admin: Cougar (Puma concolor) big cat wildlife management (v1.0)
 * Cougar housing, feeding, breeding, veterinary, market
 * Features: body_len_cm, body_wt_kg, territory_km, prey_idx, cub_count, health_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,terr_km,prey_idx,cub_ct,hlth_idx,active;} cgr_t;
typedef struct{int n_house,n_feed,n_breed,n_vet,n_mkt,t_ln,t_wt,t_terr,t_prey,t_cub;} cgr_state_t;
static cgr_t cgrhs[N],cgrfd[N-2],cgrbr[N-4],cgrvt[N-6],cgrmk[N-6]; static cgr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cgr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tk,int px,int cc,int hi){if(*cnt>=mx)return -1;cgr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->terr_km=tk;x->prey_idx=px;x->cub_ct=cc;x->hlth_idx=hi;x->active=1;*sum+=bl;(*cnt)++;ps("[CGR] Cougar ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tk=");pi(tk);ps(" px=");pi(px);ps(" cc=");pi(cc);ps(" hi=");pi(hi);ps("\n");return *cnt-1;}
int cgr_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_vet=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_terr=0;st.t_prey=0;st.t_cub=0;for(int i=0;i<N;i++)cgrhs[i].active=0;for(int i=0;i<N-2;i++)cgrfd[i].active=0;for(int i=0;i<N-4;i++)cgrbr[i].active=0;for(int i=0;i<N-6;i++)cgrvt[i].active=0;for(int i=0;i<N-6;i++)cgrmk[i].active=0;init=1;ps("[CGR] Cougar initialized\n");return 0;}
int cgr_housing(int lc,int bl,int bw,int tk,int px,int cc,int hi){return add(cgrhs,&st.n_house,&st.t_ln,N,lc,bl,bw,tk,px,cc,hi);}
int cgr_feeding(int lc,int bl,int bw,int tk,int px,int cc,int hi){return add(cgrfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tk,px,cc,hi);}
int cgr_breeding(int lc,int bl,int bw,int tk,int px,int cc,int hi){return add(cgrbr,&st.n_breed,&st.t_terr,N-4,lc,bl,bw,tk,px,cc,hi);}
int cgr_veterinary(int lc,int bl,int bw,int tk,int px,int cc,int hi){return add(cgrvt,&st.n_vet,&st.t_prey,N-6,lc,bl,bw,tk,px,cc,hi);}
int cgr_market(int lc,int bl,int bw,int tk,int px,int cc,int hi){return add(cgrmk,&st.n_mkt,&st.t_cub,N-6,lc,bl,bw,tk,px,cc,hi);}
void cgr_report(void){ps("[CGR] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Terr=");pi(st.t_terr);ps("\nVet: ");pi(st.n_vet);ps(" Prey=");pi(st.t_prey);ps("\nMkt: ");pi(st.n_mkt);ps(" Cub=");pi(st.t_cub);ps("\n");}
void cgr_state(void){ps("[CGR] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Vet=");pi(st.n_vet);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cougar Admin Demo ===\n\n");cgr_init();
/* 1=wilderness 2=sanctuary 3=zoo 4=reserve 5=market */
ps("Cougar housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=130+(i*10),bw=50+(i*8),tk=20+(i*5),px=(i%6)+1,cc=(i%4),hi=7+(i%4);cgr_housing(lc,bl,bw,tk,px,cc,hi);}
ps("\nCougar feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=135+(i*9),bw=55+(i*7),tk=22+(i*4),px=(i%5)+1,cc=(i%3),hi=7+(i%3);cgr_feeding(lc,bl,bw,tk,px,cc,hi);}
ps("\nCougar breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=140+(i*8),bw=60+(i*6),tk=24+(i*4),px=(i%4)+1,cc=1+(i%3),hi=8+(i%3);cgr_breeding(lc,bl,bw,tk,px,cc,hi);}
ps("\nCougar veterinary...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=125+(i*12),bw=45+(i*10),tk=18+(i*6),px=(i%7)+1,cc=(i%5),hi=6+(i%5);cgr_veterinary(lc,bl,bw,tk,px,cc,hi);}
ps("\nCougar market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=145+(i*7),bw=65+(i*5),tk=26+(i*3),px=(i%3)+4,cc=2+(i%2),hi=9+(i%2);cgr_market(lc,bl,bw,tk,px,cc,hi);}
ps("\n");cgr_report();cgr_state();ps("\n=== Demo Complete ===\n");return 0;}
