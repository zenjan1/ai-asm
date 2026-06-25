/* panda_admin: Panda (Ailuropoda melanoleuca) giant panda bear (v1.0)
 * Panda bamboo, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, paw_cm, bamboo_kg, patch_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,paw_cm,bm_kg,pt_idx,age_yr,active;} pnda_t;
typedef struct{int n_bamboo,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_paw,t_bm,t_pt;} pnda_state_t;
static pnda_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static pnda_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pnda_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int pc,int bk,int pi2,int ay){if(*cnt>=mx)return -1;pnda_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->paw_cm=pc;x->bm_kg=bk;x->pt_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PANDA] Panda ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" pc=");pi(pc);ps(" bk=");pi(bk);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int pnda_init(void){if(init)return -1;st.n_bamboo=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_paw=0;st.t_bm=0;st.t_pt=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[PANDA] Panda initialized\n");return 0;}
int pnda_bamboo(int lc,int bl,int bw,int pc,int bk,int pi2,int ay){return add(apl,&st.n_bamboo,&st.t_ln,N,lc,bl,bw,pc,bk,pi2,ay);}
int pnda_feeding(int lc,int bl,int bw,int pc,int bk,int pi2,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,pc,bk,pi2,ay);}
int pnda_breeding(int lc,int bl,int bw,int pc,int bk,int pi2,int ay){return add(abd,&st.n_breed,&st.t_paw,N-4,lc,bl,bw,pc,bk,pi2,ay);}
int pnda_health(int lc,int bl,int bw,int pc,int bk,int pi2,int ay){return add(ahd,&st.n_health,&st.t_bm,N-6,lc,bl,bw,pc,bk,pi2,ay);}
int pnda_market(int lc,int bl,int bw,int pc,int bk,int pi2,int ay){return add(amd,&st.n_mkt,&st.t_pt,N-6,lc,bl,bw,pc,bk,pi2,ay);}
void pnda_report(void){ps("[PANDA] Bamboo: ");pi(st.n_bamboo);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Paw=");pi(st.t_paw);ps("\nHealth: ");pi(st.n_health);ps(" Bm=");pi(st.t_bm);ps("\nMkt: ");pi(st.n_mkt);ps(" Pt=");pi(st.t_pt);ps("\n");}
void pnda_state(void){ps("[PANDA] Bamboo=");pi(st.n_bamboo);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Panda Admin Demo ===\n\n");pnda_init();
ps("Panda bamboo...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*8),bw=80+(i*10),pc=18+(i*3),bk=10+(i*5),pi2=(i%6)+1,ay=2+(i%15);pnda_bamboo(lc,bl,bw,pc,bk,pi2,ay);}
ps("\nPanda feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=125+(i*8),bw=85+(i*10),pc=19+(i*3),bk=12+(i*5),pi2=(i%5)+2,ay=3+(i%12);pnda_feeding(lc,bl,bw,pc,bk,pi2,ay);}
ps("\nPanda breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=130+(i*8),bw=90+(i*10),pc=20+(i*3),bk=14+(i*5),pi2=(i%4)+1,ay=4+(i%10);pnda_breeding(lc,bl,bw,pc,bk,pi2,ay);}
ps("\nPanda health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=115+(i*10),bw=75+(i*12),pc=17+(i*4),bk=8+(i*6),pi2=(i%3)+3,ay=5+(i%8);pnda_health(lc,bl,bw,pc,bk,pi2,ay);}
ps("\nPanda market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=140+(i*8),bw=100+(i*10),pc=22+(i*3),bk=16+(i*5),pi2=(i%6)+1,ay=6+(i%6);pnda_market(lc,bl,bw,pc,bk,pi2,ay);}
ps("\n");pnda_report();pnda_state();ps("\n=== Demo Complete ===\n");return 0;}
