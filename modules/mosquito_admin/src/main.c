/* mosquito_admin: Mosquito (Aedes aegypti) yellow fever mosquito disease vector (v1.0)
 * Mosquito swamp, feeding, breeding, health, market
 * Features: body_len_mm, body_wt_mg, wing_hz, bite_force, resist_idx, age_day
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_hz,bt_fc,rs_idx,age_dy,active;} mosq_t;
typedef struct{int n_swamp,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_bt,t_rs;} mosq_state_t;
static mosq_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static mosq_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mosq_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wz,int bf,int ri,int ad){if(*cnt>=mx)return -1;mosq_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_hz=wz;x->bt_fc=bf;x->rs_idx=ri;x->age_dy=ad;x->active=1;*sum+=bl;(*cnt)++;ps("[MOSQ] Mosquito ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wz=");pi(wz);ps(" bf=");pi(bf);ps(" ri=");pi(ri);ps(" ad=");pi(ad);ps("\n");return *cnt-1;}
int mosq_init(void){if(init)return -1;st.n_swamp=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_bt=0;st.t_rs=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MOSQ] Mosquito initialized\n");return 0;}
int mosq_swamp(int lc,int bl,int bw,int wz,int bf,int ri,int ad){return add(apl,&st.n_swamp,&st.t_ln,N,lc,bl,bw,wz,bf,ri,ad);}
int mosq_feeding(int lc,int bl,int bw,int wz,int bf,int ri,int ad){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wz,bf,ri,ad);}
int mosq_breeding(int lc,int bl,int bw,int wz,int bf,int ri,int ad){return add(abd,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wz,bf,ri,ad);}
int mosq_health(int lc,int bl,int bw,int wz,int bf,int ri,int ad){return add(ahd,&st.n_health,&st.t_bt,N-6,lc,bl,bw,wz,bf,ri,ad);}
int mosq_market(int lc,int bl,int bw,int wz,int bf,int ri,int ad){return add(amd,&st.n_mkt,&st.t_rs,N-6,lc,bl,bw,wz,bf,ri,ad);}
void mosq_report(void){ps("[MOSQ] Swamp: ");pi(st.n_swamp);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Bt=");pi(st.t_bt);ps("\nMkt: ");pi(st.n_mkt);ps(" Rs=");pi(st.t_rs);ps("\n");}
void mosq_state(void){ps("[MOSQ] Swamp=");pi(st.n_swamp);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Mosquito Admin Demo ===\n\n");mosq_init();
ps("Mosquito swamp...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=3+(i*2),bw=2+(i*1),wz=300+(i*20),bf=10+(i*3),ri=(i%8)+1,ad=1+(i*10);mosq_swamp(lc,bl,bw,wz,bf,ri,ad);}
ps("\nMosquito feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=4+(i*2),bw=3+(i*1),wz=320+(i*20),bf=12+(i*3),ri=(i%7)+2,ad=2+(i*8);mosq_feeding(lc,bl,bw,wz,bf,ri,ad);}
ps("\nMosquito breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=5+(i*2),bw=3+(i*1),wz=340+(i*20),bf=14+(i*3),ri=(i%6)+1,ad=3+(i%6);mosq_breeding(lc,bl,bw,wz,bf,ri,ad);}
ps("\nMosquito health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=2+(i*3),bw=2+(i*2),wz=280+(i*25),bf=8+(i*4),ri=(i%5)+3,ad=4+(i%5);mosq_health(lc,bl,bw,wz,bf,ri,ad);}
ps("\nMosquito market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=6+(i*2),bw=4+(i*1),wz=360+(i*20),bf=16+(i*3),ri=(i%4)+4,ad=5+(i%4);mosq_market(lc,bl,bw,wz,bf,ri,ad);}
ps("\n");mosq_report();mosq_state();ps("\n=== Demo Complete ===\n");return 0;}
