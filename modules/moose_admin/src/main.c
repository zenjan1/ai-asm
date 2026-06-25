/* moose_admin: Moose (Alces alces) large forest deer (v1.0)
 * Moose boreal, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, antler_cm, walk_speed, hide_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,antler_cm,wk_sp,hd_idx,age_yr,active;} moose_t;
typedef struct{int n_boreal,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ant,t_wk,t_hd;} moose_state_t;
static moose_t bor[N],bof[N-2],bob[N-4],boh[N-6],bom[N-6]; static moose_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(moose_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int ws,int hi,int ay){if(*cnt>=mx)return -1;moose_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->antler_cm=ac;x->wk_sp=ws;x->hd_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MOOSE] Moose ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" ws=");pi(ws);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int moose_init(void){if(init)return -1;st.n_boreal=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ant=0;st.t_wk=0;st.t_hd=0;for(int i=0;i<N;i++)bor[i].active=0;for(int i=0;i<N-2;i++)bof[i].active=0;for(int i=0;i<N-4;i++)bob[i].active=0;for(int i=0;i<N-6;i++)boh[i].active=0;for(int i=0;i<N-6;i++)bom[i].active=0;init=1;ps("[MOOSE] Moose initialized\n");return 0;}
int moose_boreal(int lc,int bl,int bw,int ac,int ws,int hi,int ay){return add(bor,&st.n_boreal,&st.t_ln,N,lc,bl,bw,ac,ws,hi,ay);}
int moose_feeding(int lc,int bl,int bw,int ac,int ws,int hi,int ay){return add(bof,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,ws,hi,ay);}
int moose_breeding(int lc,int bl,int bw,int ac,int ws,int hi,int ay){return add(bob,&st.n_breed,&st.t_ant,N-4,lc,bl,bw,ac,ws,hi,ay);}
int moose_health(int lc,int bl,int bw,int ac,int ws,int hi,int ay){return add(boh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,ac,ws,hi,ay);}
int moose_market(int lc,int bl,int bw,int ac,int ws,int hi,int ay){return add(bom,&st.n_mkt,&st.t_hd,N-6,lc,bl,bw,ac,ws,hi,ay);}
void moose_report(void){ps("[MOOSE] Boreal: ");pi(st.n_boreal);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ant=");pi(st.t_ant);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Hd=");pi(st.t_hd);ps("\n");}
void moose_state(void){ps("[MOOSE] Boreal=");pi(st.n_boreal);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Moose Admin Demo ===\n\n");moose_init();
ps("Moose boreal...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=250+(i*10),bw=400+(i*30),ac=120+(i*8),ws=4+(i*1),hi=(i%8)+1,ay=(i%20)+1;moose_boreal(lc,bl,bw,ac,ws,hi,ay);}
ps("\nMoose feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=255+(i*8),bw=410+(i*25),ac=124+(i*6),ws=4+(i*1),hi=(i%7)+1,ay=(i%18)+1;moose_feeding(lc,bl,bw,ac,ws,hi,ay);}
ps("\nMoose breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=260+(i*6),bw=420+(i*20),ac=128+(i*5),ws=3+(i*2),hi=(i%6)+1,ay=(i%15)+1;moose_breeding(lc,bl,bw,ac,ws,hi,ay);}
ps("\nMoose health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=245+(i*12),bw=390+(i*35),ac=116+(i*10),ws=3+(i*2),hi=(i%9)+1,ay=(i%12)+1;moose_health(lc,bl,bw,ac,ws,hi,ay);}
ps("\nMoose market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=265+(i*5),bw=430+(i*18),ac=132+(i*4),ws=5+(i*1),hi=(i%5)+1,ay=(i%10)+1;moose_market(lc,bl,bw,ac,ws,hi,ay);}
ps("\n");moose_report();moose_state();ps("\n=== Demo Complete ===\n");return 0;}
