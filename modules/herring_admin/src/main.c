/* herring_admin: Herring (Clupea harengus) Atlantic herring forage fish (v1.0)
 * Herring tank, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, scale_ct, swim_speed_kmh, school_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,sc_ct,sw_kh,sch_idx,age_yr,active;} herring_t;
typedef struct{int n_tank,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_sc,t_sw,t_sch;} herring_state_t;
static herring_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static herring_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(herring_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int sk,int si,int ay){if(*cnt>=mx)return -1;herring_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->sc_ct=sc;x->sw_kh=sk;x->sch_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HERR] Herring ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" sk=");pi(sk);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int herring_init(void){if(init)return -1;st.n_tank=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_sc=0;st.t_sw=0;st.t_sch=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HERR] Herring initialized\n");return 0;}
int herring_tank(int lc,int bl,int bw,int sc,int sk,int si,int ay){return add(gpl,&st.n_tank,&st.t_ln,N,lc,bl,bw,sc,sk,si,ay);}
int herring_feeding(int lc,int bl,int bw,int sc,int sk,int si,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,sk,si,ay);}
int herring_breeding(int lc,int bl,int bw,int sc,int sk,int si,int ay){return add(gbr,&st.n_breed,&st.t_sc,N-4,lc,bl,bw,sc,sk,si,ay);}
int herring_health(int lc,int bl,int bw,int sc,int sk,int si,int ay){return add(ghl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,sc,sk,si,ay);}
int herring_market(int lc,int bl,int bw,int sc,int sk,int si,int ay){return add(gmk,&st.n_mkt,&st.t_sch,N-6,lc,bl,bw,sc,sk,si,ay);}
void herring_report(void){ps("[HERR] Tank: ");pi(st.n_tank);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Sc=");pi(st.t_sc);ps("\nHlth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sch=");pi(st.t_sch);ps("\n");}
void herring_state(void){ps("[HERR] Tank=");pi(st.n_tank);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Herring Admin Demo ===\n\n");herring_init();
/* 1=ocean 2=coast 3=fjord 4=net 5=tank */
ps("Herring tank...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*3),bw=50+(i*10),sc=40+(i*5),sk=20+(i*5),si=(i%5)+1,ay=(i%4)+1;herring_tank(lc,bl,bw,sc,sk,si,ay);}
ps("\nHerring feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=27+(i*2),bw=55+(i*8),sc=42+(i*4),sk=22+(i*4),si=(i%4)+1,ay=(i%3)+1;herring_feeding(lc,bl,bw,sc,sk,si,ay);}
ps("\nHerring breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=23+(i*4),bw=45+(i*12),sc=38+(i*6),sk=18+(i*6),si=(i%3)+2,ay=(i%3)+1;herring_breeding(lc,bl,bw,sc,sk,si,ay);}
ps("\nHerring health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=30+(i*2),bw=60+(i*7),sc=45+(i*4),sk=25+(i*4),si=(i%5)+1,ay=(i%4)+1;herring_health(lc,bl,bw,sc,sk,si,ay);}
ps("\nHerring market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=33+(i*2),bw=65+(i*6),sc=48+(i*3),sk=28+(i*3),si=(i%4)+1,ay=(i%3)+1;herring_market(lc,bl,bw,sc,sk,si,ay);}
ps("\n");herring_report();herring_state();ps("\n=== Demo Complete ===\n");return 0;}
