/* carpet_admin: Carpet grass (Axonopus) turf and lawn management (v1.0)
 * Carpet planting, mowing, fertilizing, aeration, market
 * Features: coverage_sqm, blade_height_cm, density_pct, mow_interval_days, fertilizer_kg, irrigation_lpm
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,cov_sqm,blade_ht,dens_pct,mow_int,fert_kg,irrig_lpm,active;} carp_t;
typedef struct{int n_plant,n_mow,n_fert,n_aer,n_mkt,t_cov,t_blade,t_dens,t_mow,t_fert;} carp_state_t;
static carp_t carpps[N],carpmws[N-2],carpfs[N-4],carpas[N-6],carpms[N-6]; static carp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(carp_t*a,int*cnt,int*sum,int mx,int lc,int cv,int bh,int dp,int mi,int fk,int il){if(*cnt>=mx)return -1;carp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->cov_sqm=cv;x->blade_ht=bh;x->dens_pct=dp;x->mow_int=mi;x->fert_kg=fk;x->irrig_lpm=il;x->active=1;*sum+=cv;(*cnt)++;ps("[CARP] Carpet ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" cv=");pi(cv);ps(" bh=");pi(bh);ps(" dp=");pi(dp);ps(" mi=");pi(mi);ps(" fk=");pi(fk);ps(" il=");pi(il);ps("\n");return *cnt-1;}
int carp_init(void){if(init)return -1;st.n_plant=0;st.n_mow=0;st.n_fert=0;st.n_aer=0;st.n_mkt=0;st.t_cov=0;st.t_blade=0;st.t_dens=0;st.t_mow=0;st.t_fert=0;for(int i=0;i<N;i++)carpps[i].active=0;for(int i=0;i<N-2;i++)carpmws[i].active=0;for(int i=0;i<N-4;i++)carpfs[i].active=0;for(int i=0;i<N-6;i++)carpas[i].active=0;for(int i=0;i<N-6;i++)carpms[i].active=0;init=1;ps("[CARP] Carpet initialized\n");return 0;}
int carp_planting(int lc,int cv,int bh,int dp,int mi,int fk,int il){return add(carpps,&st.n_plant,&st.t_cov,N,lc,cv,bh,dp,mi,fk,il);}
int carp_mowing(int lc,int cv,int bh,int dp,int mi,int fk,int il){return add(carpmws,&st.n_mow,&st.t_blade,N-2,lc,cv,bh,dp,mi,fk,il);}
int carp_fertilizing(int lc,int cv,int bh,int dp,int mi,int fk,int il){return add(carpfs,&st.n_fert,&st.t_dens,N-4,lc,cv,bh,dp,mi,fk,il);}
int carp_aeration(int lc,int cv,int bh,int dp,int mi,int fk,int il){return add(carpas,&st.n_aer,&st.t_mow,N-6,lc,cv,bh,dp,mi,fk,il);}
int carp_market(int lc,int cv,int bh,int dp,int mi,int fk,int il){return add(carpms,&st.n_mkt,&st.t_fert,N-6,lc,cv,bh,dp,mi,fk,il);}
void carp_report(void){ps("[CARP] Plant: ");pi(st.n_plant);ps(" Cov=");pi(st.t_cov);ps("\nMow: ");pi(st.n_mow);ps(" Blade=");pi(st.t_blade);ps("\nFert: ");pi(st.n_fert);ps(" Dens=");pi(st.t_dens);ps("\nAer: ");pi(st.n_aer);ps(" Mow=");pi(st.t_mow);ps("\nMkt: ");pi(st.n_mkt);ps(" Fert=");pi(st.t_fert);ps("\n");}
void carp_state(void){ps("[CARP] Plant=");pi(st.n_plant);ps(" Mow=");pi(st.n_mow);ps(" Fert=");pi(st.n_fert);ps(" Aer=");pi(st.n_aer);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Carpet Admin Demo ===\n\n");carp_init();
/* 1=lawn 2=park 3=sports_field 4=golf_course 5=roadside */
ps("Carpet planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,cv=10+(i*5),bh=5+(i%3),dp=40+(i*4),mi=7+(i%5),fk=2+(i%3),il=10+(i*5);carp_planting(lc,cv,bh,dp,mi,fk,il);}
ps("\nCarpet mowing...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,cv=15+(i*4),bh=6+(i%2),dp=50+(i*3),mi=5+(i%4),fk=3+(i%2),il=12+(i*4);carp_mowing(lc,cv,bh,dp,mi,fk,il);}
ps("\nCarpet fertilizing...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,cv=20+(i*3),bh=7+(i%2),dp=60+(i*3),mi=4+(i%3),fk=4+(i%2),il=15+(i*3);carp_fertilizing(lc,cv,bh,dp,mi,fk,il);}
ps("\nCarpet aeration...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,cv=8+(i*6),bh=4+(i%3),dp=35+(i*5),mi=8+(i%4),fk=1+(i%4),il=8+(i*6);carp_aeration(lc,cv,bh,dp,mi,fk,il);}
ps("\nCarpet market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,cv=25+(i*3),bh=8+(i%2),dp=70+(i*3),mi=3+(i%3),fk=5+(i%2),il=18+(i*3);carp_market(lc,cv,bh,dp,mi,fk,il);}
ps("\n");carp_report();carp_state();ps("\n=== Demo Complete ===\n");return 0;}
