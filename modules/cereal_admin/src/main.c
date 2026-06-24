/* cereal_admin: Cereal grain (wheat, rice, barley, oats) crop management (v1.0)
 * Cereal planting, irrigation, pest_control, harvest, market
 * Features: yield_kg_per_hectare, plant_height_cm, head_length_cm, grain_count, protein_pct, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,yield_kg,plnt_ht,head_ln,grain_ct,prot_pct,harv_wk,active;} cers_t;
typedef struct{int n_plant,n_irrig,n_pest,n_harv,n_mkt,t_yield,t_ht,t_head,t_grain,t_prot;} cers_state_t;
static cers_t cersps[N],cersis[N-2],cerspe[N-4],cershs[N-6],cersms[N-6]; static cers_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cers_t*a,int*cnt,int*sum,int mx,int lc,int yk,int ph,int hl,int gc,int pp,int hw){if(*cnt>=mx)return -1;cers_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->yield_kg=yk;x->plnt_ht=ph;x->head_ln=hl;x->grain_ct=gc;x->prot_pct=pp;x->harv_wk=hw;x->active=1;*sum+=yk;(*cnt)++;ps("[CERS] Cereal ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" yk=");pi(yk);ps(" ph=");pi(ph);ps(" hl=");pi(hl);ps(" gc=");pi(gc);ps(" pp=");pi(pp);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int cers_init(void){if(init)return -1;st.n_plant=0;st.n_irrig=0;st.n_pest=0;st.n_harv=0;st.n_mkt=0;st.t_yield=0;st.t_ht=0;st.t_head=0;st.t_grain=0;st.t_prot=0;for(int i=0;i<N;i++)cersps[i].active=0;for(int i=0;i<N-2;i++)cersis[i].active=0;for(int i=0;i<N-4;i++)cerspe[i].active=0;for(int i=0;i<N-6;i++)cershs[i].active=0;for(int i=0;i<N-6;i++)cersms[i].active=0;init=1;ps("[CERS] Cereal initialized\n");return 0;}
int cers_planting(int lc,int yk,int ph,int hl,int gc,int pp,int hw){return add(cersps,&st.n_plant,&st.t_yield,N,lc,yk,ph,hl,gc,pp,hw);}
int cers_irrigation(int lc,int yk,int ph,int hl,int gc,int pp,int hw){return add(cersis,&st.n_irrig,&st.t_ht,N-2,lc,yk,ph,hl,gc,pp,hw);}
int cers_pest_control(int lc,int yk,int ph,int hl,int gc,int pp,int hw){return add(cerspe,&st.n_pest,&st.t_head,N-4,lc,yk,ph,hl,gc,pp,hw);}
int cers_harvest(int lc,int yk,int ph,int hl,int gc,int pp,int hw){return add(cershs,&st.n_harv,&st.t_grain,N-6,lc,yk,ph,hl,gc,pp,hw);}
int cers_market(int lc,int yk,int ph,int hl,int gc,int pp,int hw){return add(cersms,&st.n_mkt,&st.t_prot,N-6,lc,yk,ph,hl,gc,pp,hw);}
void cers_report(void){ps("[CERS] Plant: ");pi(st.n_plant);ps(" Yield=");pi(st.t_yield);ps("\nIrrig: ");pi(st.n_irrig);ps(" Ht=");pi(st.t_ht);ps("\nPest: ");pi(st.n_pest);ps(" Head=");pi(st.t_head);ps("\nHarv: ");pi(st.n_harv);ps(" Grain=");pi(st.t_grain);ps("\nMkt: ");pi(st.n_mkt);ps(" Prot=");pi(st.t_prot);ps("\n");}
void cers_state(void){ps("[CERS] Plant=");pi(st.n_plant);ps(" Irrig=");pi(st.n_irrig);ps(" Pest=");pi(st.n_pest);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cereal Admin Demo ===\n\n");cers_init();
/* 1=wheat_field 2=rice_paddy 3=barley_field 4=oat_field 5=mixed_grain */
ps("Cereal planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,yk=2000+(i*500),ph=60+(i*10),hl=8+(i*2),gc=30+(i*10),pp=10+(i%5),hw=20+(i%4);cers_planting(lc,yk,ph,hl,gc,pp,hw);}
ps("\nCereal irrigation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,yk=2500+(i*400),ph=70+(i*8),hl=10+(i*2),gc=35+(i*8),pp=11+(i%4),hw=22+(i%3);cers_irrigation(lc,yk,ph,hl,gc,pp,hw);}
ps("\nCereal pest control...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,yk=3000+(i*300),ph=80+(i*6),hl=12+(i*2),gc=40+(i*6),pp=12+(i%3),hw=24+(i*2);cers_pest_control(lc,yk,ph,hl,gc,pp,hw);}
ps("\nCereal harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,yk=1500+(i*600),ph=50+(i*12),hl=6+(i*3),gc=25+(i*12),pp=9+(i%6),hw=18+(i%5);cers_harvest(lc,yk,ph,hl,gc,pp,hw);}
ps("\nCereal market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,yk=3500+(i*300),ph=90+(i*5),hl=14+(i*2),gc=45+(i*5),pp=13+(i%2),hw=26+(i*2);cers_market(lc,yk,ph,hl,gc,pp,hw);}
ps("\n");cers_report();cers_state();ps("\n=== Demo Complete ===\n");return 0;}
