/* gecko_admin: Gecko (Gekko gecko) tokay gecko reptile (v1.0)
 * Gecko enclosure, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, climb_idx, chirp_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,clmb_idx,chrp_vl,age_yr,active;} gecko_t;
typedef struct{int n_enclos,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_clmb,t_chrp;} gecko_state_t;
static gecko_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static gecko_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gecko_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ci,int cv,int ay){if(*cnt>=mx)return -1;gecko_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->clmb_idx=ci;x->chrp_vl=cv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GECK] Gecko ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ci=");pi(ci);ps(" cv=");pi(cv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gecko_init(void){if(init)return -1;st.n_enclos=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_clmb=0;st.t_chrp=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GECK] Gecko initialized\n");return 0;}
int gecko_enclosure(int lc,int bl,int bw,int tc,int ci,int cv,int ay){return add(gpl,&st.n_enclos,&st.t_ln,N,lc,bl,bw,tc,ci,cv,ay);}
int gecko_feeding(int lc,int bl,int bw,int tc,int ci,int cv,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ci,cv,ay);}
int gecko_breeding(int lc,int bl,int bw,int tc,int ci,int cv,int ay){return add(gbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,ci,cv,ay);}
int gecko_health(int lc,int bl,int bw,int tc,int ci,int cv,int ay){return add(ghl,&st.n_health,&st.t_clmb,N-6,lc,bl,bw,tc,ci,cv,ay);}
int gecko_market(int lc,int bl,int bw,int tc,int ci,int cv,int ay){return add(gmk,&st.n_mkt,&st.t_chrp,N-6,lc,bl,bw,tc,ci,cv,ay);}
void gecko_report(void){ps("[GECK] Enclos: ");pi(st.n_enclos);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHlth: ");pi(st.n_health);ps(" Clmb=");pi(st.t_clmb);ps("\nMkt: ");pi(st.n_mkt);ps(" Chrp=");pi(st.t_chrp);ps("\n");}
void gecko_state(void){ps("[GECK] Enclos=");pi(st.n_enclos);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gecko Admin Demo ===\n\n");gecko_init();
/* 1=terrarium 2=vivarium 3=cage 4=garden 5=lab */
ps("Gecko enclosure...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*2),bw=30+(i*5),tc=8+(i*2),ci=(i%5)+1,cv=20+(i*8),ay=(i%4)+1;gecko_enclosure(lc,bl,bw,tc,ci,cv,ay);}
ps("\nGecko feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=12+(i*2),bw=35+(i*4),tc=9+(i*2),ci=(i%4)+1,cv=25+(i*6),ay=(i%3)+1;gecko_feeding(lc,bl,bw,tc,ci,cv,ay);}
ps("\nGecko breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=8+(i*3),bw=28+(i*6),tc=7+(i*2),ci=(i%3)+2,cv=15+(i*7),ay=(i%3)+2;gecko_breeding(lc,bl,bw,tc,ci,cv,ay);}
ps("\nGecko health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=14+(i*2),bw=40+(i*4),tc=10+(i),ci=(i%5)+1,cv=30+(i*5),ay=(i%4)+1;gecko_health(lc,bl,bw,tc,ci,cv,ay);}
ps("\nGecko market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=16+(i*2),bw=45+(i*3),tc=11+(i),ci=(i%4)+1,cv=35+(i*4),ay=(i%3)+2;gecko_market(lc,bl,bw,tc,ci,cv,ay);}
ps("\n");gecko_report();gecko_state();ps("\n=== Demo Complete ===\n");return 0;}
