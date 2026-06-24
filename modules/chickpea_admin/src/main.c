/* chickpea_admin: Chickpea (Cicer arietinum) legume crop management (v1.0)
 * Chickpea planting, inoculation, pest_control, harvest, market
 * Features: plant_height_cm, pod_count, seed_weight_g, protein_pct, root_nodule_count, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,pod_ct,seed_wt,prot_pct,root_nd,harv_wk,active;} chcp_t;
typedef struct{int n_plant,n_inoc,n_pest,n_harv,n_mkt,t_ht,t_pod,t_seed,t_prot,t_nodule;} chcp_state_t;
static chcp_t chcpps[N],chcpis[N-2],chcpps2[N-4],chcpvs[N-6],chcpms[N-6]; static chcp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chcp_t*a,int*cnt,int*sum,int mx,int lc,int ph,int pc,int sw,int pp,int rn,int hw){if(*cnt>=mx)return -1;chcp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->pod_ct=pc;x->seed_wt=sw;x->prot_pct=pp;x->root_nd=rn;x->harv_wk=hw;x->active=1;*sum+=ph;(*cnt)++;ps("[CHCP] Chickpea ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" pc=");pi(pc);ps(" sw=");pi(sw);ps(" pp=");pi(pp);ps(" rn=");pi(rn);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int chcp_init(void){if(init)return -1;st.n_plant=0;st.n_inoc=0;st.n_pest=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_pod=0;st.t_seed=0;st.t_prot=0;st.t_nodule=0;for(int i=0;i<N;i++)chcpps[i].active=0;for(int i=0;i<N-2;i++)chcpis[i].active=0;for(int i=0;i<N-4;i++)chcpps2[i].active=0;for(int i=0;i<N-6;i++)chcpvs[i].active=0;for(int i=0;i<N-6;i++)chcpms[i].active=0;init=1;ps("[CHCP] Chickpea initialized\n");return 0;}
int chcp_planting(int lc,int ph,int pc,int sw,int pp,int rn,int hw){return add(chcpps,&st.n_plant,&st.t_ht,N,lc,ph,pc,sw,pp,rn,hw);}
int chcp_inoculation(int lc,int ph,int pc,int sw,int pp,int rn,int hw){return add(chcpis,&st.n_inoc,&st.t_pod,N-2,lc,ph,pc,sw,pp,rn,hw);}
int chcp_pest_control(int lc,int ph,int pc,int sw,int pp,int rn,int hw){return add(chcpps2,&st.n_pest,&st.t_seed,N-4,lc,ph,pc,sw,pp,rn,hw);}
int chcp_harvest(int lc,int ph,int pc,int sw,int pp,int rn,int hw){return add(chcpvs,&st.n_harv,&st.t_prot,N-6,lc,ph,pc,sw,pp,rn,hw);}
int chcp_market(int lc,int ph,int pc,int sw,int pp,int rn,int hw){return add(chcpms,&st.n_mkt,&st.t_nodule,N-6,lc,ph,pc,sw,pp,rn,hw);}
void chcp_report(void){ps("[CHCP] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nInoc: ");pi(st.n_inoc);ps(" Pod=");pi(st.t_pod);ps("\nPest: ");pi(st.n_pest);ps(" Seed=");pi(st.t_seed);ps("\nHarv: ");pi(st.n_harv);ps(" Prot=");pi(st.t_prot);ps("\nMkt: ");pi(st.n_mkt);ps(" Nodule=");pi(st.t_nodule);ps("\n");}
void chcp_state(void){ps("[CHCP] Plant=");pi(st.n_plant);ps(" Inoc=");pi(st.n_inoc);ps(" Pest=");pi(st.n_pest);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chickpea Admin Demo ===\n\n");chcp_init();
/* 1=field 2=dryland 3=irrigated 4=organic 5=rotation */
ps("Chickpea planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=30+(i*5),pc=10+(i*5),sw=15+(i*3),pp=18+(i%5),rn=5+(i%8),hw=16+(i%4);chcp_planting(lc,ph,pc,sw,pp,rn,hw);}
ps("\nChickpea inoculation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=35+(i*4),pc=15+(i*4),sw=18+(i%2),pp=20+(i%4),rn=8+(i%6),hw=18+(i%3);chcp_inoculation(lc,ph,pc,sw,pp,rn,hw);}
ps("\nChickpea pest control...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=40+(i*3),pc=20+(i*3),sw=20+(i%3),pp=22+(i%3),rn=10+(i%4),hw=20+(i%2);chcp_pest_control(lc,ph,pc,sw,pp,rn,hw);}
ps("\nChickpea harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=25+(i*6),pc=8+(i*6),sw=12+(i*4),pp=16+(i%6),rn=3+(i%9),hw=14+(i%5);chcp_harvest(lc,ph,pc,sw,pp,rn,hw);}
ps("\nChickpea market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=45+(i%3),pc=25+(i%3),sw=22+(i%2),pp=24+(i%2),rn=12+(i%3),hw=22+(i%2);chcp_market(lc,ph,pc,sw,pp,rn,hw);}
ps("\n");chcp_report();chcp_state();ps("\n=== Demo Complete ===\n");return 0;}
