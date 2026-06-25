/* elk_admin: Elk (Cervus canadensis) North American large antlered mammal (v1.0)
 * Elk forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, antler_cm, run_speed, rack_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ant_cm,rn_sp,rk_idx,age_yr,active;} elk_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ant,t_rn,t_rk;} elk_state_t;
static elk_t frs[N],frf[N-2],frb[N-4],frh[N-6],frm[N-6]; static elk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(elk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int rs,int ri,int ay){if(*cnt>=mx)return -1;elk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ant_cm=ac;x->rn_sp=rs;x->rk_idx=ri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ELK] Elk ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" rs=");pi(rs);ps(" ri=");pi(ri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int elk_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ant=0;st.t_rn=0;st.t_rk=0;for(int i=0;i<N;i++)frs[i].active=0;for(int i=0;i<N-2;i++)frf[i].active=0;for(int i=0;i<N-4;i++)frb[i].active=0;for(int i=0;i<N-6;i++)frh[i].active=0;for(int i=0;i<N-6;i++)frm[i].active=0;init=1;ps("[ELK] Elk initialized\n");return 0;}
int elk_forest(int lc,int bl,int bw,int ac,int rs,int ri,int ay){return add(frs,&st.n_forest,&st.t_ln,N,lc,bl,bw,ac,rs,ri,ay);}
int elk_feeding(int lc,int bl,int bw,int ac,int rs,int ri,int ay){return add(frf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,rs,ri,ay);}
int elk_breeding(int lc,int bl,int bw,int ac,int rs,int ri,int ay){return add(frb,&st.n_breed,&st.t_ant,N-4,lc,bl,bw,ac,rs,ri,ay);}
int elk_health(int lc,int bl,int bw,int ac,int rs,int ri,int ay){return add(frh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,ac,rs,ri,ay);}
int elk_market(int lc,int bl,int bw,int ac,int rs,int ri,int ay){return add(frm,&st.n_mkt,&st.t_rk,N-6,lc,bl,bw,ac,rs,ri,ay);}
void elk_report(void){ps("[ELK] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ant=");pi(st.t_ant);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Rk=");pi(st.t_rk);ps("\n");}
void elk_state(void){ps("[ELK] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Elk Admin Demo ===\n\n");elk_init();
ps("Elk forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=300+(i*20),ac=80+(i*8),rs=10+(i*3),ri=(i%10)+1,ay=(i%20)+1;elk_forest(lc,bl,bw,ac,rs,ri,ay);}
ps("\nElk feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=320+(i*15),ac=85+(i*6),rs=11+(i*2),ri=(i%8)+1,ay=(i%18)+1;elk_feeding(lc,bl,bw,ac,rs,ri,ay);}
ps("\nElk breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=340+(i*12),ac=90+(i*5),rs=9+(i*4),ri=(i%6)+1,ay=(i%15)+1;elk_breeding(lc,bl,bw,ac,rs,ri,ay);}
ps("\nElk health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=290+(i*25),ac=75+(i*10),rs=8+(i*5),ri=(i%12)+1,ay=(i%12)+1;elk_health(lc,bl,bw,ac,rs,ri,ay);}
ps("\nElk market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*5),bw=360+(i*10),ac=95+(i*4),rs=12+(i*2),ri=(i%5)+1,ay=(i%10)+1;elk_market(lc,bl,bw,ac,rs,ri,ay);}
ps("\n");elk_report();elk_state();ps("\n=== Demo Complete ===\n");return 0;}
