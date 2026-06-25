/* harpy_admin: Harpy Eagle (Harpia harpyja) harpy eagle raptor (v1.0)
 * Harpy habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, talon_cm, swoop_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,tl_cm,sw_idx,age_yr,active;} harpy_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_tl,t_sw;} harpy_state_t;
static harpy_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static harpy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(harpy_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int tc,int si,int ay){if(*cnt>=mx)return -1;harpy_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->tl_cm=tc;x->sw_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HARP] Harpy ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" tc=");pi(tc);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int harpy_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_tl=0;st.t_sw=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HARP] Harpy initialized\n");return 0;}
int harpy_habitat(int lc,int bl,int bw,int ws,int tc,int si,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,ws,tc,si,ay);}
int harpy_feeding(int lc,int bl,int bw,int ws,int tc,int si,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,tc,si,ay);}
int harpy_breeding(int lc,int bl,int bw,int ws,int tc,int si,int ay){return add(gbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,tc,si,ay);}
int harpy_health(int lc,int bl,int bw,int ws,int tc,int si,int ay){return add(ghl,&st.n_health,&st.t_tl,N-6,lc,bl,bw,ws,tc,si,ay);}
int harpy_market(int lc,int bl,int bw,int ws,int tc,int si,int ay){return add(gmk,&st.n_mkt,&st.t_sw,N-6,lc,bl,bw,ws,tc,si,ay);}
void harpy_report(void){ps("[HARP] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHlth: ");pi(st.n_health);ps(" Tl=");pi(st.t_tl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sw=");pi(st.t_sw);ps("\n");}
void harpy_state(void){ps("[HARP] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Harpy Eagle Admin Demo ===\n\n");harpy_init();
/* 1=canopy 2=rainforest 3=cliff 4=nest 5=sanctuary */
ps("Harpy habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*5),bw=4+(i*2),ws=180+(i*8),tc=8+(i%4),si=(i%5)+1,ay=(i%6)+1;harpy_habitat(lc,bl,bw,ws,tc,si,ay);}
ps("\nHarpy feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=95+(i*4),bw=5+(i*2),ws=185+(i*6),tc=9+(i%3),si=(i%4)+1,ay=(i%5)+1;harpy_feeding(lc,bl,bw,ws,tc,si,ay);}
ps("\nHarpy breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=85+(i*6),bw=6+(i*2),ws=175+(i*9),tc=7+(i%5),si=(i%3)+2,ay=(i%4)+1;harpy_breeding(lc,bl,bw,ws,tc,si,ay);}
ps("\nHarpy health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=100+(i*4),bw=5+(i*2),ws=190+(i*5),tc=10+(i%3),si=(i%5)+1,ay=(i%5)+1;harpy_health(lc,bl,bw,ws,tc,si,ay);}
ps("\nHarpy market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=105+(i*3),bw=6+(i*2),ws=195+(i*4),tc=11+(i%2),si=(i%4)+1,ay=(i%3)+2;harpy_market(lc,bl,bw,ws,tc,si,ay);}
ps("\n");harpy_report();harpy_state();ps("\n=== Demo Complete ===\n");return 0;}
