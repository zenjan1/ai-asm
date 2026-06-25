/* camel_admin: Camel (Camelus dromedarius) dromedary desert ungulate (v1.0)
 * Camel desert, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, hump_cm, walk_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hump_cm,wk_sp,fr_idx,age_yr,active;} cml_t;
typedef struct{int n_desert,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hump,t_wk,t_fr;} cml_state_t;
static cml_t dsl[N],dsf[N-2],dsb[N-4],dsh[N-6],dsm[N-6]; static cml_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cml_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ws,int fi,int ay){if(*cnt>=mx)return -1;cml_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hump_cm=hc;x->wk_sp=ws;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CML] Camel ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ws=");pi(ws);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int camel_init(void){if(init)return -1;st.n_desert=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hump=0;st.t_wk=0;st.t_fr=0;for(int i=0;i<N;i++)dsl[i].active=0;for(int i=0;i<N-2;i++)dsf[i].active=0;for(int i=0;i<N-4;i++)dsb[i].active=0;for(int i=0;i<N-6;i++)dsh[i].active=0;for(int i=0;i<N-6;i++)dsm[i].active=0;init=1;ps("[CML] Camel initialized\n");return 0;}
int camel_desert(int lc,int bl,int bw,int hc,int ws,int fi,int ay){return add(dsl,&st.n_desert,&st.t_ln,N,lc,bl,bw,hc,ws,fi,ay);}
int camel_feeding(int lc,int bl,int bw,int hc,int ws,int fi,int ay){return add(dsf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ws,fi,ay);}
int camel_breeding(int lc,int bl,int bw,int hc,int ws,int fi,int ay){return add(dsb,&st.n_breed,&st.t_hump,N-4,lc,bl,bw,hc,ws,fi,ay);}
int camel_health(int lc,int bl,int bw,int hc,int ws,int fi,int ay){return add(dsh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,hc,ws,fi,ay);}
int camel_market(int lc,int bl,int bw,int hc,int ws,int fi,int ay){return add(dsm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,hc,ws,fi,ay);}
void camel_report(void){ps("[CML] Desert: ");pi(st.n_desert);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hump=");pi(st.t_hump);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void camel_state(void){ps("[CML] Desert=");pi(st.n_desert);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Camel Admin Demo ===\n\n");camel_init();
ps("Camel desert...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=400+(i*30),hc=20+(i*3),ws=4+(i*2),fi=(i%8)+1,ay=(i%20)+1;camel_desert(lc,bl,bw,hc,ws,fi,ay);}
ps("\nCamel feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=420+(i*25),hc=22+(i*2),ws=5+(i*1),fi=(i%6)+1,ay=(i%18)+1;camel_feeding(lc,bl,bw,hc,ws,fi,ay);}
ps("\nCamel breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=450+(i*20),hc=24+(i*2),ws=3+(i*3),fi=(i%5)+1,ay=(i%15)+1;camel_breeding(lc,bl,bw,hc,ws,fi,ay);}
ps("\nCamel health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=380+(i*40),hc=18+(i*4),ws=2+(i*4),fi=(i%10)+1,ay=(i%12)+1;camel_health(lc,bl,bw,hc,ws,fi,ay);}
ps("\nCamel market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*5),bw=470+(i*15),hc=26+(i*2),ws=6+(i*2),fi=(i%4)+1,ay=(i%10)+1;camel_market(lc,bl,bw,hc,ws,fi,ay);}
ps("\n");camel_report();camel_state();ps("\n=== Demo Complete ===\n");return 0;}
