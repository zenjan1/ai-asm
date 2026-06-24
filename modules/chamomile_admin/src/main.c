/* chamomile_admin: Chamomile (Matricaria chamomilla) medicinal herb management (v1.0)
 * Chamomile planting, bloom, drying, extraction, market
 * Features: flower_diameter_cm, plant_height_cm, oil_yield_ml, bloom_count, drying_temp, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flr_dia,plnt_ht,oil_yd,bloom_ct,dry_tmp,harv_wk,active;} chm_t;
typedef struct{int n_plant,n_bloom,n_dry,n_extract,n_mkt,t_flr,t_ht,t_oil,t_bloom,t_dry;} chm_state_t;
static chm_t chmps[N],chmbs[N-2],chmds[N-4],chmes[N-6],chmms[N-6]; static chm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chm_t*a,int*cnt,int*sum,int mx,int lc,int fd,int ph,int oy,int bc,int dt,int hw){if(*cnt>=mx)return -1;chm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flr_dia=fd;x->plnt_ht=ph;x->oil_yd=oy;x->bloom_ct=bc;x->dry_tmp=dt;x->harv_wk=hw;x->active=1;*sum+=fd;(*cnt)++;ps("[CHM] Chamomile ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" ph=");pi(ph);ps(" oy=");pi(oy);ps(" bc=");pi(bc);ps(" dt=");pi(dt);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int chm_init(void){if(init)return -1;st.n_plant=0;st.n_bloom=0;st.n_dry=0;st.n_extract=0;st.n_mkt=0;st.t_flr=0;st.t_ht=0;st.t_oil=0;st.t_bloom=0;st.t_dry=0;for(int i=0;i<N;i++)chmps[i].active=0;for(int i=0;i<N-2;i++)chmbs[i].active=0;for(int i=0;i<N-4;i++)chmds[i].active=0;for(int i=0;i<N-6;i++)chmes[i].active=0;for(int i=0;i<N-6;i++)chmms[i].active=0;init=1;ps("[CHM] Chamomile initialized\n");return 0;}
int chm_planting(int lc,int fd,int ph,int oy,int bc,int dt,int hw){return add(chmps,&st.n_plant,&st.t_flr,N,lc,fd,ph,oy,bc,dt,hw);}
int chm_bloom(int lc,int fd,int ph,int oy,int bc,int dt,int hw){return add(chmbs,&st.n_bloom,&st.t_ht,N-2,lc,fd,ph,oy,bc,dt,hw);}
int chm_drying(int lc,int fd,int ph,int oy,int bc,int dt,int hw){return add(chmds,&st.n_dry,&st.t_oil,N-4,lc,fd,ph,oy,bc,dt,hw);}
int chm_extraction(int lc,int fd,int ph,int oy,int bc,int dt,int hw){return add(chmes,&st.n_extract,&st.t_bloom,N-6,lc,fd,ph,oy,bc,dt,hw);}
int chm_market(int lc,int fd,int ph,int oy,int bc,int dt,int hw){return add(chmms,&st.n_mkt,&st.t_dry,N-6,lc,fd,ph,oy,bc,dt,hw);}
void chm_report(void){ps("[CHM] Plant: ");pi(st.n_plant);ps(" Flr=");pi(st.t_flr);ps("\nBloom: ");pi(st.n_bloom);ps(" Ht=");pi(st.t_ht);ps("\nDry: ");pi(st.n_dry);ps(" Oil=");pi(st.t_oil);ps("\nExtract: ");pi(st.n_extract);ps(" Bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" Dry=");pi(st.t_dry);ps("\n");}
void chm_state(void){ps("[CHM] Plant=");pi(st.n_plant);ps(" Bloom=");pi(st.n_bloom);ps(" Dry=");pi(st.n_dry);ps(" Extract=");pi(st.n_extract);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chamomile Admin Demo ===\n\n");chm_init();
/* 1=herb_garden 2=medicinal 3=field 4=container 5=organic */
ps("Chamomile planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,fd=1+(i%3),ph=20+(i*5),oy=2+(i%5),bc=10+(i*5),dt=35+(i%10),hw=12+(i%4);chm_planting(lc,fd,ph,oy,bc,dt,hw);}
ps("\nChamomile bloom...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,fd=2+(i%3),ph=25+(i*4),oy=3+(i%4),bc=15+(i*4),dt=38+(i%8),hw=14+(i%3);chm_bloom(lc,fd,ph,oy,bc,dt,hw);}
ps("\nChamomile drying...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,fd=2+(i%2),ph=30+(i*3),oy=4+(i%3),bc=20+(i*3),dt=40+(i%5),hw=16+(i%2);chm_drying(lc,fd,ph,oy,bc,dt,hw);}
ps("\nChamomile extraction...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,fd=1+(i%3),ph=18+(i*6),oy=1+(i%5),bc=8+(i*6),dt=32+(i%12),hw=10+(i%5);chm_extraction(lc,fd,ph,oy,bc,dt,hw);}
ps("\nChamomile market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,fd=3+(i%2),ph=35+(i*3),oy=5+(i%2),bc=25+(i*3),dt=45+(i%3),hw=18+(i%2);chm_market(lc,fd,ph,oy,bc,dt,hw);}
ps("\n");chm_report();chm_state();ps("\n=== Demo Complete ===\n");return 0;}
