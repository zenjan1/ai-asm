/* burnet_admin: Burnet (Sanguisorba) medicinal herb management (v1.0)
 * Burnet propagation, drying, extraction, quality, market
 * Features: plant_ht_cm, flower_spike_cm, leaf_count, root_mass_g, oil_pct, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,flr_sp,leaf_ct,root_ms,oil_pct,harv_wk,active;} burn_t;
typedef struct{int n_prop,n_dry,n_extract,n_qual,n_mkt,t_ht,flr_sp,t_leaf,t_root,t_oil;} burn_state_t;
static burn_t burnps[N],burnds[N-2],burnes[N-4],burnqs[N-6],burnms[N-6]; static burn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(burn_t*a,int*cnt,int*sum,int mx,int lc,int ph,int fs,int lct,int rms,int opc,int hw){if(*cnt>=mx)return -1;burn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->flr_sp=fs;x->leaf_ct=lct;x->root_ms=rms;x->oil_pct=opc;x->harv_wk=hw;x->active=1;*sum+=ph;(*cnt)++;ps("[BURN] Burnet ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" fs=");pi(fs);ps(" lct=");pi(lct);ps(" rms=");pi(rms);ps(" opc=");pi(opc);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int burn_init(void){if(init)return -1;st.n_prop=0;st.n_dry=0;st.n_extract=0;st.n_qual=0;st.n_mkt=0;st.t_ht=0;st.flr_sp=0;st.t_leaf=0;st.t_root=0;st.t_oil=0;for(int i=0;i<N;i++)burnps[i].active=0;for(int i=0;i<N-2;i++)burnds[i].active=0;for(int i=0;i<N-4;i++)burnes[i].active=0;for(int i=0;i<N-6;i++)burnqs[i].active=0;for(int i=0;i<N-6;i++)burnms[i].active=0;init=1;ps("[BURN] Burnet initialized\n");return 0;}
int burn_propagation(int lc,int ph,int fs,int lct,int rms,int opc,int hw){return add(burnps,&st.n_prop,&st.t_ht,N,lc,ph,fs,lct,rms,opc,hw);}
int burn_drying(int lc,int ph,int fs,int lct,int rms,int opc,int hw){return add(burnds,&st.n_dry,&st.flr_sp,N-2,lc,ph,fs,lct,rms,opc,hw);}
int burn_extraction(int lc,int ph,int fs,int lct,int rms,int opc,int hw){return add(burnes,&st.n_extract,&st.t_leaf,N-4,lc,ph,fs,lct,rms,opc,hw);}
int burn_quality(int lc,int ph,int fs,int lct,int rms,int opc,int hw){return add(burnqs,&st.n_qual,&st.t_root,N-6,lc,ph,fs,lct,rms,opc,hw);}
int burn_market(int lc,int ph,int fs,int lct,int rms,int opc,int hw){return add(burnms,&st.n_mkt,&st.t_oil,N-6,lc,ph,fs,lct,rms,opc,hw);}
void burn_report(void){ps("[BURN] Prop: ");pi(st.n_prop);ps(" Ht=");pi(st.t_ht);ps("\nDry: ");pi(st.n_dry);ps(" Spike=");pi(st.flr_sp);ps("\nExtract: ");pi(st.n_extract);ps(" Leaf=");pi(st.t_leaf);ps("\nQual: ");pi(st.n_qual);ps(" Root=");pi(st.t_root);ps("\nMkt: ");pi(st.n_mkt);ps(" Oil=");pi(st.t_oil);ps("\n");}
void burn_state(void){ps("[BURN] Prop=");pi(st.n_prop);ps(" Dry=");pi(st.n_dry);ps(" Extract=");pi(st.n_extract);ps(" Qual=");pi(st.n_qual);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Burnet Admin Demo ===\n\n");burn_init();
/* 1=meadow 2=hedge_row 3=woodland_edge 4=field 5=garden */
ps("Burnet propagation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=30+(i*8),fs=5+(i*3),lct=10+(i*5),rms=15+(i*8),opc=1+(i%5),hw=8+(i%4);burn_propagation(lc,ph,fs,lct,rms,opc,hw);}
ps("\nBurnet drying...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=35+(i*6),fs=6+(i*2),lct=12+(i*4),rms=20+(i*6),opc=2+(i%4),hw=10+(i%3);burn_drying(lc,ph,fs,lct,rms,opc,hw);}
ps("\nBurnet extraction...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=40+(i*5),fs=7+(i*2),lct=15+(i*3),rms=25+(i*5),opc=3+(i%3),hw=12+(i%2);burn_extraction(lc,ph,fs,lct,rms,opc,hw);}
ps("\nBurnet quality...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=25+(i*10),fs=4+(i*4),lct=8+(i*6),rms=10+(i*10),opc=1+(i%5),hw=6+(i%5);burn_quality(lc,ph,fs,lct,rms,opc,hw);}
ps("\nBurnet market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=45+(i*4),fs=8+(i*2),lct=18+(i*3),rms=30+(i*4),opc=4+(i%2),hw=14+(i%2);burn_market(lc,ph,fs,lct,rms,opc,hw);}
ps("\n");burn_report();burn_state();ps("\n=== Demo Complete ===\n");return 0;}
