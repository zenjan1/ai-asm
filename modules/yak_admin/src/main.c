/* yak_admin: Yak (Bos grunniens) high altitude bovine (v1.0)
 * Yak plateau, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, walk_speed, wool_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,wk_sp,wl_idx,age_yr,active;} yak_t;
typedef struct{int n_plat,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_wk,t_wl;} yak_state_t;
static yak_t ypl[N],yfd[N-2],ybr[N-4],yhl[N-6],ymk[N-6]; static yak_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(yak_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ws,int wi,int ay){if(*cnt>=mx)return -1;yak_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->wk_sp=ws;x->wl_idx=wi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[YAK] Yak ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ws=");pi(ws);ps(" wi=");pi(wi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int yak_init(void){if(init)return -1;st.n_plat=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_wk=0;st.t_wl=0;for(int i=0;i<N;i++)ypl[i].active=0;for(int i=0;i<N-2;i++)yfd[i].active=0;for(int i=0;i<N-4;i++)ybr[i].active=0;for(int i=0;i<N-6;i++)yhl[i].active=0;for(int i=0;i<N-6;i++)ymk[i].active=0;init=1;ps("[YAK] Yak initialized\n");return 0;}
int yak_plateau(int lc,int bl,int bw,int hc,int ws,int wi,int ay){return add(ypl,&st.n_plat,&st.t_ln,N,lc,bl,bw,hc,ws,wi,ay);}
int yak_feeding(int lc,int bl,int bw,int hc,int ws,int wi,int ay){return add(yfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ws,wi,ay);}
int yak_breeding(int lc,int bl,int bw,int hc,int ws,int wi,int ay){return add(ybr,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,ws,wi,ay);}
int yak_health(int lc,int bl,int bw,int hc,int ws,int wi,int ay){return add(yhl,&st.n_health,&st.t_wk,N-6,lc,bl,bw,hc,ws,wi,ay);}
int yak_market(int lc,int bl,int bw,int hc,int ws,int wi,int ay){return add(ymk,&st.n_mkt,&st.t_wl,N-6,lc,bl,bw,hc,ws,wi,ay);}
void yak_report(void){ps("[YAK] Plateau: ");pi(st.n_plat);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Wl=");pi(st.t_wl);ps("\n");}
void yak_state(void){ps("[YAK] Plat=");pi(st.n_plat);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Yak Admin Demo ===\n\n");yak_init();
ps("Yak plateau...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*15),bw=500+(i*50),hc=60+(i*8),ws=3+(i*2),wl=(i%6)+1,ay=(i%5)+1;yak_plateau(lc,bl,bw,hc,ws,wl,ay);}
ps("\nYak feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=220+(i*12),bw=550+(i*40),hc=65+(i*6),ws=4+(i),wl=(i%5)+1,ay=(i%4)+1;yak_feeding(lc,bl,bw,hc,ws,wl,ay);}
ps("\nYak breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=180+(i*18),bw=450+(i*60),hc=55+(i*10),ws=2+(i*3),wl=(i%4)+1,ay=(i%3)+1;yak_breeding(lc,bl,bw,hc,ws,wl,ay);}
ps("\nYak health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=240+(i*10),bw=600+(i*30),hc=70+(i*5),ws=5+(i),wl=(i%7)+1,ay=(i%6)+2;yak_health(lc,bl,bw,hc,ws,wl,ay);}
ps("\nYak market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=260+(i*8),bw=650+(i*25),hc=75+(i*4),ws=2+(i*2),wl=(i%3)+3,ay=(i%4)+3;yak_market(lc,bl,bw,hc,ws,wl,ay);}
ps("\n");yak_report();yak_state();ps("\n=== Demo Complete ===\n");return 0;}
