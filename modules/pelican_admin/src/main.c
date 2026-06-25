/* pelican_admin: Pelican (Pelecanus occidentalis) brown pelican large waterbird (v1.0)
 * Pelican coast, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, beak_cm, pouch_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,bk_cm,ph_idx,age_yr,active;} plc_t;
typedef struct{int n_coast,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_bk,t_ph;} plc_state_t;
static plc_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static plc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(plc_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int bc,int pi2,int ay){if(*cnt>=mx)return -1;plc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->bk_cm=bc;x->ph_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PLCN] Pelican ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" bc=");pi(bc);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int plc_init(void){if(init)return -1;st.n_coast=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_bk=0;st.t_ph=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[PLCN] Pelican initialized\n");return 0;}
int plc_coast(int lc,int bl,int bw,int ws,int bc,int pi2,int ay){return add(apl,&st.n_coast,&st.t_ln,N,lc,bl,bw,ws,bc,pi2,ay);}
int plc_feeding(int lc,int bl,int bw,int ws,int bc,int pi2,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,bc,pi2,ay);}
int plc_breeding(int lc,int bl,int bw,int ws,int bc,int pi2,int ay){return add(abd,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,bc,pi2,ay);}
int plc_health(int lc,int bl,int bw,int ws,int bc,int pi2,int ay){return add(ahd,&st.n_health,&st.t_bk,N-6,lc,bl,bw,ws,bc,pi2,ay);}
int plc_market(int lc,int bl,int bw,int ws,int bc,int pi2,int ay){return add(amd,&st.n_mkt,&st.t_ph,N-6,lc,bl,bw,ws,bc,pi2,ay);}
void plc_report(void){ps("[PLCN] Coast: ");pi(st.n_coast);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Bk=");pi(st.t_bk);ps("\nMkt: ");pi(st.n_mkt);ps(" Ph=");pi(st.t_ph);ps("\n");}
void plc_state(void){ps("[PLCN] Coast=");pi(st.n_coast);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Pelican Admin Demo ===\n\n");plc_init();
ps("Pelican coast...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=130+(i*8),bw=4+(i*2),ws=200+(i*10),bc=30+(i*3),pi2=(i%6)+1,ay=2+(i%12);plc_coast(lc,bl,bw,ws,bc,pi2,ay);}
ps("\nPelican feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=135+(i*8),bw=5+(i*2),ws=205+(i*10),bc=32+(i*3),pi2=(i%5)+2,ay=3+(i%10);plc_feeding(lc,bl,bw,ws,bc,pi2,ay);}
ps("\nPelican breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=140+(i*8),bw=5+(i*2),ws=210+(i*10),bc=34+(i*3),pi2=(i%4)+1,ay=4+(i%8);plc_breeding(lc,bl,bw,ws,bc,pi2,ay);}
ps("\nPelican health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=125+(i*10),bw=3+(i*3),ws=195+(i*12),bc=28+(i*4),pi2=(i%3)+3,ay=5+(i%6);plc_health(lc,bl,bw,ws,bc,pi2,ay);}
ps("\nPelican market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=150+(i*8),bw=6+(i*2),ws=220+(i*10),bc=36+(i*3),pi2=(i%6)+1,ay=6+(i%5);plc_market(lc,bl,bw,ws,bc,pi2,ay);}
ps("\n");plc_report();plc_state();ps("\n=== Demo Complete ===\n");return 0;}
