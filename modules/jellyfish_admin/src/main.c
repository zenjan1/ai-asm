/* jellyfish_admin: Jellyfish (Aurelia aurita) moon jellyfish marine invertebrate (v1.0)
 * Jellyfish tank, feeding, breeding, health, market
 * Features: bell_dia_cm, body_wt_g, tentacle_cm, sting_idx, pulse_rate, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bl_dia,bdy_wt,tn_cm,st_idx,pl_rt,age_yr,active;} jellyfish_t;
typedef struct{int n_tank,n_feed,n_breed,n_health,n_mkt,t_dia,t_wt,t_tn,t_st,t_pl;} jellyfish_state_t;
static jellyfish_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static jellyfish_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jellyfish_t*a,int*cnt,int*sum,int mx,int lc,int bd,int bw,int tc,int si,int pr,int ay){if(*cnt>=mx)return -1;jellyfish_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bl_dia=bd;x->bdy_wt=bw;x->tn_cm=tc;x->st_idx=si;x->pl_rt=pr;x->age_yr=ay;x->active=1;*sum+=bd;(*cnt)++;ps("[JELLY] Jellyfish ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bd=");pi(bd);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" si=");pi(si);ps(" pr=");pi(pr);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int jellyfish_init(void){if(init)return -1;st.n_tank=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_dia=0;st.t_wt=0;st.t_tn=0;st.t_st=0;st.t_pl=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[JELLY] Jellyfish initialized\n");return 0;}
int jellyfish_tank(int lc,int bd,int bw,int tc,int si,int pr,int ay){return add(gpl,&st.n_tank,&st.t_dia,N,lc,bd,bw,tc,si,pr,ay);}
int jellyfish_feeding(int lc,int bd,int bw,int tc,int si,int pr,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bd,bw,tc,si,pr,ay);}
int jellyfish_breeding(int lc,int bd,int bw,int tc,int si,int pr,int ay){return add(gbr,&st.n_breed,&st.t_tn,N-4,lc,bd,bw,tc,si,pr,ay);}
int jellyfish_health(int lc,int bd,int bw,int tc,int si,int pr,int ay){return add(ghl,&st.n_health,&st.t_st,N-6,lc,bd,bw,tc,si,pr,ay);}
int jellyfish_market(int lc,int bd,int bw,int tc,int si,int pr,int ay){return add(gmk,&st.n_mkt,&st.t_pl,N-6,lc,bd,bw,tc,si,pr,ay);}
void jellyfish_report(void){ps("[JELLY] Tank: ");pi(st.n_tank);ps(" Dia=");pi(st.t_dia);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tn=");pi(st.t_tn);ps("\nHlth: ");pi(st.n_health);ps(" St=");pi(st.t_st);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void jellyfish_state(void){ps("[JELLY] Tank=");pi(st.n_tank);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Jellyfish Admin Demo ===\n\n");jellyfish_init();
/* 1=aquarium 2=ocean 3=lagoon 4=reef 5=tank */
ps("Jellyfish tank...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bd=10+(i*3),bw=5+(i*2),tn=15+(i*4),si=(i%5)+1,pr=2+(i%4),ay=(i%3)+1;jellyfish_tank(lc,bd,bw,tn,si,pr,ay);}
ps("\nJellyfish feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bd=12+(i*2),bw=6+(i*2),tn=17+(i*3),si=(i%4)+1,pr=3+(i%3),ay=(i%3)+1;jellyfish_feeding(lc,bd,bw,tn,si,pr,ay);}
ps("\nJellyfish breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bd=8+(i*4),bw=4+(i*3),tn=13+(i*5),si=(i%3)+2,pr=2+(i%4),ay=(i%3)+1;jellyfish_breeding(lc,bd,bw,tn,si,pr,ay);}
ps("\nJellyfish health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bd=15+(i*2),bw=7+(i*2),tn=20+(i*3),si=(i%5)+1,pr=4+(i%3),ay=(i%4)+1;jellyfish_health(lc,bd,bw,tn,si,pr,ay);}
ps("\nJellyfish market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bd=18+(i*2),bw=8+(i*2),tn=23+(i*2),si=(i%4)+1,pr=5+(i%2),ay=(i%3)+1;jellyfish_market(lc,bd,bw,tn,si,pr,ay);}
ps("\n");jellyfish_report();jellyfish_state();ps("\n=== Demo Complete ===\n");return 0;}
