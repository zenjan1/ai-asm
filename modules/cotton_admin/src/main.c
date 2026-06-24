/* cotton_admin: Cotton (Gossypium hirsutum) fiber crop management (v1.0)
 * Cotton planting, irrigation, pest control, harvest, market
 * Features: plant_ht_cm, boll_count, fiber_len_mm, lint_pct, seed_oil_pct, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,boll_ct,fbr_ln,lnt_pct,seed_oil,harv_wk,active;} cttn_t;
typedef struct{int n_plant,n_irrig,n_pest,n_harv,n_mkt,t_ht,t_boll,t_fbr,t_lnt,t_oil;} cttn_state_t;
static cttn_t cttnps[N],cttni[N-2],cttnpt[N-4],cttnh[N-6],cttnm[N-6]; static cttn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cttn_t*a,int*cnt,int*sum,int mx,int lc,int ph,int bc,int fl,int lp,int so,int hw){if(*cnt>=mx)return -1;cttn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->boll_ct=bc;x->fbr_ln=fl;x->lnt_pct=lp;x->seed_oil=so;x->harv_wk=hw;x->active=1;*sum+=ph;(*cnt)++;ps("[CTTN] Cotton ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" bc=");pi(bc);ps(" fl=");pi(fl);ps(" lp=");pi(lp);ps(" so=");pi(so);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int cttn_init(void){if(init)return -1;st.n_plant=0;st.n_irrig=0;st.n_pest=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_boll=0;st.t_fbr=0;st.t_lnt=0;st.t_oil=0;for(int i=0;i<N;i++)cttnps[i].active=0;for(int i=0;i<N-2;i++)cttni[i].active=0;for(int i=0;i<N-4;i++)cttnpt[i].active=0;for(int i=0;i<N-6;i++)cttnh[i].active=0;for(int i=0;i<N-6;i++)cttnm[i].active=0;init=1;ps("[CTTN] Cotton initialized\n");return 0;}
int cttn_planting(int lc,int ph,int bc,int fl,int lp,int so,int hw){return add(cttnps,&st.n_plant,&st.t_ht,N,lc,ph,bc,fl,lp,so,hw);}
int cttn_irrigation(int lc,int ph,int bc,int fl,int lp,int so,int hw){return add(cttni,&st.n_irrig,&st.t_boll,N-2,lc,ph,bc,fl,lp,so,hw);}
int cttn_pest_control(int lc,int ph,int bc,int fl,int lp,int so,int hw){return add(cttnpt,&st.n_pest,&st.t_fbr,N-4,lc,ph,bc,fl,lp,so,hw);}
int cttn_harvest(int lc,int ph,int bc,int fl,int lp,int so,int hw){return add(cttnh,&st.n_harv,&st.t_lnt,N-6,lc,ph,bc,fl,lp,so,hw);}
int cttn_market(int lc,int ph,int bc,int fl,int lp,int so,int hw){return add(cttnm,&st.n_mkt,&st.t_oil,N-6,lc,ph,bc,fl,lp,so,hw);}
void cttn_report(void){ps("[CTTN] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nIrrig: ");pi(st.n_irrig);ps(" Boll=");pi(st.t_boll);ps("\nPest: ");pi(st.n_pest);ps(" Fbr=");pi(st.t_fbr);ps("\nHarv: ");pi(st.n_harv);ps(" Lint=");pi(st.t_lnt);ps("\nMkt: ");pi(st.n_mkt);ps(" Oil=");pi(st.t_oil);ps("\n");}
void cttn_state(void){ps("[CTTN] Plant=");pi(st.n_plant);ps(" Irrig=");pi(st.n_irrig);ps(" Pest=");pi(st.n_pest);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cotton Admin Demo ===\n\n");cttn_init();
/* 1=field 2=irrigated 3=organic 4=greenhouse 5=market */
ps("Cotton planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=60+(i*8),bc=10+(i*5),fl=20+(i*3),lp=35+(i*3),so=15+(i*2),hw=16+(i%6);cttn_planting(lc,ph,bc,fl,lp,so,hw);}
ps("\nCotton irrigation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=65+(i*7),bc=12+(i*4),fl=22+(i*3),lp=37+(i*2),so=16+(i*2),hw=17+(i%5);cttn_irrigation(lc,ph,bc,fl,lp,so,hw);}
ps("\nCotton pest control...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=70+(i*6),bc=14+(i*4),fl=24+(i*2),lp=39+(i*2),so=17+(i*2),hw=18+(i%4);cttn_pest_control(lc,ph,bc,fl,lp,so,hw);}
ps("\nCotton harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=55+(i*10),bc=8+(i*6),fl=18+(i*4),lp=33+(i*4),so=14+(i*3),hw=14+(i%7);cttn_harvest(lc,ph,bc,fl,lp,so,hw);}
ps("\nCotton market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=75+(i*5),bc=16+(i*3),fl=26+(i*2),lp=41+(i*2),so=18+(i*2),hw=19+(i%3);cttn_market(lc,ph,bc,fl,lp,so,hw);}
ps("\n");cttn_report();cttn_state();ps("\n=== Demo Complete ===\n");return 0;}
