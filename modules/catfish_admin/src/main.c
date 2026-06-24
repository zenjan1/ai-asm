/* catfish_admin: Catfish (Ictalurus punctatus) aquaculture pond management (v1.0)
 * Catfish stocking, feeding, water quality, harvest, market
 * Features: pond volume, fish count, avg weight, feed weight, water temp, oxygen ppm
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,pond_vol,fish_ct,avg_wt,feed_kg,wtr_tmp,oxy_ppm,active;} catf_t;
typedef struct{int n_stock,n_feed,n_water,n_harv,n_mkt,t_pond,t_fish,t_wt,t_feed,t_temp;} catf_state_t;
static catf_t catfss[N],catffs[N-2],catfws[N-4],catfhs[N-6],catfms[N-6]; static catf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(catf_t*a,int*cnt,int*sum,int mx,int lc,int pv,int fc,int aw,int fk,int wt,int op){if(*cnt>=mx)return -1;catf_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->pond_vol=pv;x->fish_ct=fc;x->avg_wt=aw;x->feed_kg=fk;x->wtr_tmp=wt;x->oxy_ppm=op;x->active=1;*sum+=pv;(*cnt)++;ps("[CATF] Catfish ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" pv=");pi(pv);ps(" fc=");pi(fc);ps(" aw=");pi(aw);ps(" fk=");pi(fk);ps(" wt=");pi(wt);ps(" op=");pi(op);ps("\n");return *cnt-1;}
int catf_init(void){if(init)return -1;st.n_stock=0;st.n_feed=0;st.n_water=0;st.n_harv=0;st.n_mkt=0;st.t_pond=0;st.t_fish=0;st.t_wt=0;st.t_feed=0;st.t_temp=0;for(int i=0;i<N;i++)catfss[i].active=0;for(int i=0;i<N-2;i++)catffs[i].active=0;for(int i=0;i<N-4;i++)catfws[i].active=0;for(int i=0;i<N-6;i++)catfhs[i].active=0;for(int i=0;i<N-6;i++)catfms[i].active=0;init=1;ps("[CATF] Catfish initialized\n");return 0;}
int catf_stocking(int lc,int pv,int fc,int aw,int fk,int wt,int op){return add(catfss,&st.n_stock,&st.t_pond,N,lc,pv,fc,aw,fk,wt,op);}
int catf_feeding(int lc,int pv,int fc,int aw,int fk,int wt,int op){return add(catffs,&st.n_feed,&st.t_fish,N-2,lc,pv,fc,aw,fk,wt,op);}
int catf_water_quality(int lc,int pv,int fc,int aw,int fk,int wt,int op){return add(catfws,&st.n_water,&st.t_wt,N-4,lc,pv,fc,aw,fk,wt,op);}
int catf_harvest(int lc,int pv,int fc,int aw,int fk,int wt,int op){return add(catfhs,&st.n_harv,&st.t_feed,N-6,lc,pv,fc,aw,fk,wt,op);}
int catf_market(int lc,int pv,int fc,int aw,int fk,int wt,int op){return add(catfms,&st.n_mkt,&st.t_temp,N-6,lc,pv,fc,aw,fk,wt,op);}
void catf_report(void){ps("[CATF] Stock: ");pi(st.n_stock);ps(" Pond=");pi(st.t_pond);ps("\nFeed: ");pi(st.n_feed);ps(" Fish=");pi(st.t_fish);ps("\nWater: ");pi(st.n_water);ps(" Wt=");pi(st.t_wt);ps("\nHarv: ");pi(st.n_harv);ps(" Feed=");pi(st.t_feed);ps("\nMkt: ");pi(st.n_mkt);ps(" Temp=");pi(st.t_temp);ps("\n");}
void catf_state(void){ps("[CATF] Stock=");pi(st.n_stock);ps(" Feed=");pi(st.n_feed);ps(" Water=");pi(st.n_water);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Catfish Admin Demo ===\n\n");catf_init();
/* 1=pond 2=tank 3=raceway 4=cage 5=biofloc */
ps("Catfish stocking...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,pv=1000+(i*500),fc=50+(i*20),aw=100+(i*30),fk=5+(i*3),wt=20+(i%10),op=6+(i%4);catf_stocking(lc,pv,fc,aw,fk,wt,op);}
ps("\nCatfish feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,pv=1200+(i*400),fc=60+(i*15),aw=130+(i*25),fk=6+(i*2),wt=22+(i%8),op=7+(i%3);catf_feeding(lc,pv,fc,aw,fk,wt,op);}
ps("\nCatfish water quality...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,pv=1500+(i*300),fc=70+(i*12),aw=160+(i*20),fk=7+(i*2),wt=24+(i%6),op=8+(i%2);catf_water_quality(lc,pv,fc,aw,fk,wt,op);}
ps("\nCatfish harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,pv=800+(i*600),fc=40+(i*25),aw=80+(i*35),fk=4+(i*4),wt=18+(i%12),op=5+(i%5);catf_harvest(lc,pv,fc,aw,fk,wt,op);}
ps("\nCatfish market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,pv=1800+(i*200),fc=80+(i*10),aw=200+(i*15),fk=8+(i*2),wt=26+(i%4),op=9+(i%2);catf_market(lc,pv,fc,aw,fk,wt,op);}
ps("\n");catf_report();catf_state();ps("\n=== Demo Complete ===\n");return 0;}
