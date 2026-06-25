/* hagfish_admin: Hagfish (Myxine glutinosa) Atlantic hagfish jawless fish (v1.0)
 * Hagfish tank, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, slime_vol_ml, tooth_ct, nose_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,sl_ml,tc,n_idx,age_yr,active;} hagfish_t;
typedef struct{int n_tank,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_sl,t_tc,t_n;} hagfish_state_t;
static hagfish_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static hagfish_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hagfish_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sm,int tc,int ni,int ay){if(*cnt>=mx)return -1;hagfish_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->sl_ml=sm;x->tc=tc;x->n_idx=ni;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HAG] Hagfish ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sm=");pi(sm);ps(" tc=");pi(tc);ps(" ni=");pi(ni);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hagfish_init(void){if(init)return -1;st.n_tank=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_sl=0;st.t_tc=0;st.t_n=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HAG] Hagfish initialized\n");return 0;}
int hagfish_tank(int lc,int bl,int bw,int sm,int tc,int ni,int ay){return add(gpl,&st.n_tank,&st.t_ln,N,lc,bl,bw,sm,tc,ni,ay);}
int hagfish_feeding(int lc,int bl,int bw,int sm,int tc,int ni,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sm,tc,ni,ay);}
int hagfish_breeding(int lc,int bl,int bw,int sm,int tc,int ni,int ay){return add(gbr,&st.n_breed,&st.t_sl,N-4,lc,bl,bw,sm,tc,ni,ay);}
int hagfish_health(int lc,int bl,int bw,int sm,int tc,int ni,int ay){return add(ghl,&st.n_health,&st.t_tc,N-6,lc,bl,bw,sm,tc,ni,ay);}
int hagfish_market(int lc,int bl,int bw,int sm,int tc,int ni,int ay){return add(gmk,&st.n_mkt,&st.t_n,N-6,lc,bl,bw,sm,tc,ni,ay);}
void hagfish_report(void){ps("[HAG] Tank: ");pi(st.n_tank);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Sl=");pi(st.t_sl);ps("\nHlth: ");pi(st.n_health);ps(" Tc=");pi(st.t_tc);ps("\nMkt: ");pi(st.n_mkt);ps(" N=");pi(st.t_n);ps("\n");}
void hagfish_state(void){ps("[HAG] Tank=");pi(st.n_tank);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hagfish Admin Demo ===\n\n");hagfish_init();
/* 1=deepsea 2=trench 3=abyss 4=benthic 5=lab */
ps("Hagfish tank...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*4),bw=50+(i*10),sm=20+(i*8),tc=1+(i%4),ni=(i%5)+1,ay=(i%5)+1;hagfish_tank(lc,bl,bw,sm,tc,ni,ay);}
ps("\nHagfish feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=42+(i*3),bw=55+(i*8),sm=25+(i*6),tc=2+(i%3),ni=(i%4)+1,ay=(i%4)+1;hagfish_feeding(lc,bl,bw,sm,tc,ni,ay);}
ps("\nHagfish breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=38+(i*5),bw=45+(i*12),sm=18+(i*9),tc=1+(i%5),ni=(i%3)+2,ay=(i%3)+1;hagfish_breeding(lc,bl,bw,sm,tc,ni,ay);}
ps("\nHagfish health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=45+(i*3),bw=60+(i*7),sm=30+(i*5),tc=3+(i%3),ni=(i%5)+1,ay=(i%4)+1;hagfish_health(lc,bl,bw,sm,tc,ni,ay);}
ps("\nHagfish market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=50+(i*2),bw=65+(i*6),sm=35+(i*4),tc=4+(i%2),ni=(i%4)+1,ay=(i%3)+1;hagfish_market(lc,bl,bw,sm,tc,ni,ay);}
ps("\n");hagfish_report();hagfish_state();ps("\n=== Demo Complete ===\n");return 0;}
