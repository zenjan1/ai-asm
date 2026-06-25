/* cormorant_admin: Cormorant (Phalacrocorax carbo) great diving seabird (v1.0)
 * Cormorant coast, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, swim_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,sw_sp,pm_idx,age_yr,active;} crmt_t;
typedef struct{int n_coast,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_sw,t_pm;} crmt_state_t;
static crmt_t csl[N],csf[N-2],csb[N-4],csh[N-6],csm[N-6]; static crmt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crmt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int ss,int pi2,int ay){if(*cnt>=mx)return -1;crmt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->sw_sp=ss;x->pm_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CRMT] Cormorant ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" ss=");pi(ss);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int cormorant_init(void){if(init)return -1;st.n_coast=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_sw=0;st.t_pm=0;for(int i=0;i<N;i++)csl[i].active=0;for(int i=0;i<N-2;i++)csf[i].active=0;for(int i=0;i<N-4;i++)csb[i].active=0;for(int i=0;i<N-6;i++)csh[i].active=0;for(int i=0;i<N-6;i++)csm[i].active=0;init=1;ps("[CRMT] Cormorant initialized\n");return 0;}
int cormorant_coast(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(csl,&st.n_coast,&st.t_ln,N,lc,bl,bw,bc,ss,pi2,ay);}
int cormorant_feeding(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(csf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,ss,pi2,ay);}
int cormorant_breeding(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(csb,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,ss,pi2,ay);}
int cormorant_health(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(csh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,bc,ss,pi2,ay);}
int cormorant_market(int lc,int bl,int bw,int bc,int ss,int pi2,int ay){return add(csm,&st.n_mkt,&st.t_pm,N-6,lc,bl,bw,bc,ss,pi2,ay);}
void cormorant_report(void){ps("[CRMT] Coast: ");pi(st.n_coast);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Pm=");pi(st.t_pm);ps("\n");}
void cormorant_state(void){ps("[CRMT] Coast=");pi(st.n_coast);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cormorant Admin Demo ===\n\n");cormorant_init();
ps("Cormorant coast...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=70+(i*5),bw=150+(i*30),bc=5+(i*1),ss=5+(i*2),pi2=(i%8)+1,ay=(i%12)+1;cormorant_coast(lc,bl,bw,bc,ss,pi2,ay);}
ps("\nCormorant feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=73+(i*4),bw=160+(i*25),bc=5+(i%3),ss=6+(i*1),pi2=(i%6)+1,ay=(i%10)+1;cormorant_feeding(lc,bl,bw,bc,ss,pi2,ay);}
ps("\nCormorant breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=76+(i*3),bw=170+(i*20),bc=6+(i%2),ss=4+(i*3),pi2=(i%5)+1,ay=(i%8)+1;cormorant_breeding(lc,bl,bw,bc,ss,pi2,ay);}
ps("\nCormorant health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=68+(i*6),bw=145+(i*35),bc=4+(i%4),ss=3+(i*4),pi2=(i%10)+1,ay=(i%7)+1;cormorant_health(lc,bl,bw,bc,ss,pi2,ay);}
ps("\nCormorant market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=78+(i*3),bw=180+(i*15),bc=6+(i%2),ss=7+(i*2),pi2=(i%4)+1,ay=(i%6)+1;cormorant_market(lc,bl,bw,bc,ss,pi2,ay);}
ps("\n");cormorant_report();cormorant_state();ps("\n=== Demo Complete ===\n");return 0;}
