/* moose_admin: Moose (Alces alces) Eurasian moose large deer (v1.0)
 * Moose forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, antler_cm, bite_force, coat_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,antl_cm,bt_fc,ct_idx,age_yr,active;} moos_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_antl,t_bt,t_ct;} moos_state_t;
static moos_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static moos_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(moos_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int bf,int ci,int ay){if(*cnt>=mx)return -1;moos_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->antl_cm=ac;x->bt_fc=bf;x->ct_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MOOS] Moose ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" bf=");pi(bf);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int moos_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_antl=0;st.t_bt=0;st.t_ct=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MOOS] Moose initialized\n");return 0;}
int moos_forest(int lc,int bl,int bw,int ac,int bf,int ci,int ay){return add(apl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ac,bf,ci,ay);}
int moos_feeding(int lc,int bl,int bw,int ac,int bf,int ci,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,bf,ci,ay);}
int moos_breeding(int lc,int bl,int bw,int ac,int bf,int ci,int ay){return add(abd,&st.n_breed,&st.t_antl,N-4,lc,bl,bw,ac,bf,ci,ay);}
int moos_health(int lc,int bl,int bw,int ac,int bf,int ci,int ay){return add(ahd,&st.n_health,&st.t_bt,N-6,lc,bl,bw,ac,bf,ci,ay);}
int moos_market(int lc,int bl,int bw,int ac,int bf,int ci,int ay){return add(amd,&st.n_mkt,&st.t_ct,N-6,lc,bl,bw,ac,bf,ci,ay);}
void moos_report(void){ps("[MOOS] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Antl=");pi(st.t_antl);ps("\nHealth: ");pi(st.n_health);ps(" Bt=");pi(st.t_bt);ps("\nMkt: ");pi(st.n_mkt);ps(" Ct=");pi(st.t_ct);ps("\n");}
void moos_state(void){ps("[MOOS] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Moose Admin Demo ===\n\n");moos_init();
ps("Moose forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*15),bw=400+(i*30),ac=80+(i*8),bf=200+(i*20),ci=(i%6)+1,ay=2+(i%18);moos_forest(lc,bl,bw,ac,bf,ci,ay);}
ps("\nMoose feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*15),bw=420+(i*30),ac=85+(i*8),bf=210+(i*20),ci=(i%5)+2,ay=3+(i%15);moos_feeding(lc,bl,bw,ac,bf,ci,ay);}
ps("\nMoose breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*15),bw=450+(i*30),ac=90+(i*8),bf=220+(i*20),ci=(i%4)+1,ay=4+(i%12);moos_breeding(lc,bl,bw,ac,bf,ci,ay);}
ps("\nMoose health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=190+(i*20),bw=380+(i*40),ac=75+(i*10),bf=190+(i*25),ci=(i%3)+3,ay=5+(i%10);moos_health(lc,bl,bw,ac,bf,ci,ay);}
ps("\nMoose market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=240+(i*15),bw=500+(i*30),ac=100+(i*8),bf=240+(i*20),ci=(i%6)+1,ay=6+(i%8);moos_market(lc,bl,bw,ac,bf,ci,ay);}
ps("\n");moos_report();moos_state();ps("\n=== Demo Complete ===\n");return 0;}
