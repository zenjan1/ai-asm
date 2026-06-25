/* moth_admin: Moth (Bombyx mori) domestic silk moth nocturnal insect (v1.0)
 * Moth cocoon, feeding, breeding, health, market
 * Features: wing_span_cm, body_wt_g, antenna_cm, pheromone_idx, silk_yield, age_day
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,wing_sp,bdy_wt,ant_cm,ph_idx,sl_yd,age_dy,active;} moth_t;
typedef struct{int n_cocoon,n_feed,n_breed,n_health,n_mkt,t_wing,t_wt,t_ant,t_ph,t_sl;} moth_state_t;
static moth_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static moth_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(moth_t*a,int*cnt,int*sum,int mx,int lc,int ws,int bw,int ac,int ph,int sy,int ad){if(*cnt>=mx)return -1;moth_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->wing_sp=ws;x->bdy_wt=bw;x->ant_cm=ac;x->ph_idx=ph;x->sl_yd=sy;x->age_dy=ad;x->active=1;*sum+=ws;(*cnt)++;ps("[MOTH] Moth ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ws=");pi(ws);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" ph=");pi(ph);ps(" sy=");pi(sy);ps(" ad=");pi(ad);ps("\n");return *cnt-1;}
int moth_init(void){if(init)return -1;st.n_cocoon=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_wing=0;st.t_wt=0;st.t_ant=0;st.t_ph=0;st.t_sl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MOTH] Moth initialized\n");return 0;}
int moth_cocoon(int lc,int ws,int bw,int ac,int ph,int sy,int ad){return add(apl,&st.n_cocoon,&st.t_wing,N,lc,ws,bw,ac,ph,sy,ad);}
int moth_feeding(int lc,int ws,int bw,int ac,int ph,int sy,int ad){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,ws,bw,ac,ph,sy,ad);}
int moth_breeding(int lc,int ws,int bw,int ac,int ph,int sy,int ad){return add(abd,&st.n_breed,&st.t_ant,N-4,lc,ws,bw,ac,ph,sy,ad);}
int moth_health(int lc,int ws,int bw,int ac,int ph,int sy,int ad){return add(ahd,&st.n_health,&st.t_ph,N-6,lc,ws,bw,ac,ph,sy,ad);}
int moth_market(int lc,int ws,int bw,int ac,int ph,int sy,int ad){return add(amd,&st.n_mkt,&st.t_sl,N-6,lc,ws,bw,ac,ph,sy,ad);}
void moth_report(void){ps("[MOTH] Cocoon: ");pi(st.n_cocoon);ps(" Wing=");pi(st.t_wing);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ant=");pi(st.t_ant);ps("\nHealth: ");pi(st.n_health);ps(" Ph=");pi(st.t_ph);ps("\nMkt: ");pi(st.n_mkt);ps(" Sl=");pi(st.t_sl);ps("\n");}
void moth_state(void){ps("[MOTH] Cocoon=");pi(st.n_cocoon);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Moth Admin Demo ===\n\n");moth_init();
ps("Moth cocoon...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ws=4+(i*2),bw=200+(i*20),ac=1+(i*2),ph=(i%7)+1,sy=100+(i*30),ad=1+(i*14);moth_cocoon(lc,ws,bw,ac,ph,sy,ad);}
ps("\nMoth feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ws=5+(i*2),bw=220+(i*20),ac=2+(i*2),ph=(i%6)+2,sy=110+(i*30),ad=2+(i%12);moth_feeding(lc,ws,bw,ac,ph,sy,ad);}
ps("\nMoth breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ws=6+(i*2),bw=240+(i*20),ac=3+(i*2),ph=(i%5)+1,sy=120+(i*30),ad=3+(i%10);moth_breeding(lc,ws,bw,ac,ph,sy,ad);}
ps("\nMoth health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ws=3+(i*3),bw=180+(i*25),ac=1+(i*3),ph=(i%4)+3,sy=90+(i*35),ad=4+(i%8);moth_health(lc,ws,bw,ac,ph,sy,ad);}
ps("\nMoth market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ws=8+(i*2),bw=260+(i*20),ac=4+(i*2),ph=(i%3)+4,sy=140+(i*30),ad=5+(i%6);moth_market(lc,ws,bw,ac,ph,sy,ad);}
ps("\n");moth_report();moth_state();ps("\n=== Demo Complete ===\n");return 0;}
