/* cattle_admin: Cattle (Bos taurus) ranch and herd management (v1.0)
 * Cattle grazing, breeding, veterinary, milking, market
 * Features: herd_count, avg_weight_kg, milk_yield_l, calf_count, grazing_acres, vet_cost
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,herd_ct,avg_wt,milk_yd,calf_ct,graze_ac,vet_cost,active;} catt_t;
typedef struct{int n_graze,n_breed,n_vet,n_milk,n_mkt,t_herd,t_wt,t_milk,t_calf,t_graze;} catt_state_t;
static catt_t cattgs[N],cattbs[N-2],cattvs[N-4],cattms[N-6],cattks[N-6]; static catt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(catt_t*a,int*cnt,int*sum,int mx,int lc,int hc,int aw,int my,int cc,int ga,int vc){if(*cnt>=mx)return -1;catt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->herd_ct=hc;x->avg_wt=aw;x->milk_yd=my;x->calf_ct=cc;x->graze_ac=ga;x->vet_cost=vc;x->active=1;*sum+=hc;(*cnt)++;ps("[CATT] Cattle ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" hc=");pi(hc);ps(" aw=");pi(aw);ps(" my=");pi(my);ps(" cc=");pi(cc);ps(" ga=");pi(ga);ps(" vc=");pi(vc);ps("\n");return *cnt-1;}
int catt_init(void){if(init)return -1;st.n_graze=0;st.n_breed=0;st.n_vet=0;st.n_milk=0;st.n_mkt=0;st.t_herd=0;st.t_wt=0;st.t_milk=0;st.t_calf=0;st.t_graze=0;for(int i=0;i<N;i++)cattgs[i].active=0;for(int i=0;i<N-2;i++)cattbs[i].active=0;for(int i=0;i<N-4;i++)cattvs[i].active=0;for(int i=0;i<N-6;i++)cattms[i].active=0;for(int i=0;i<N-6;i++)cattks[i].active=0;init=1;ps("[CATT] Cattle initialized\n");return 0;}
int catt_grazing(int lc,int hc,int aw,int my,int cc,int ga,int vc){return add(cattgs,&st.n_graze,&st.t_herd,N,lc,hc,aw,my,cc,ga,vc);}
int catt_breeding(int lc,int hc,int aw,int my,int cc,int ga,int vc){return add(cattbs,&st.n_breed,&st.t_wt,N-2,lc,hc,aw,my,cc,ga,vc);}
int catt_veterinary(int lc,int hc,int aw,int my,int cc,int ga,int vc){return add(cattvs,&st.n_vet,&st.t_milk,N-4,lc,hc,aw,my,cc,ga,vc);}
int catt_milking(int lc,int hc,int aw,int my,int cc,int ga,int vc){return add(cattms,&st.n_milk,&st.t_calf,N-6,lc,hc,aw,my,cc,ga,vc);}
int catt_market(int lc,int hc,int aw,int my,int cc,int ga,int vc){return add(cattks,&st.n_mkt,&st.t_graze,N-6,lc,hc,aw,my,cc,ga,vc);}
void catt_report(void){ps("[CATT] Graze: ");pi(st.n_graze);ps(" Herd=");pi(st.t_herd);ps("\nBreed: ");pi(st.n_breed);ps(" Wt=");pi(st.t_wt);ps("\nVet: ");pi(st.n_vet);ps(" Milk=");pi(st.t_milk);ps("\nMilk: ");pi(st.n_milk);ps(" Calf=");pi(st.t_calf);ps("\nMkt: ");pi(st.n_mkt);ps(" Graze=");pi(st.t_graze);ps("\n");}
void catt_state(void){ps("[CATT] Graze=");pi(st.n_graze);ps(" Breed=");pi(st.n_breed);ps(" Vet=");pi(st.n_vet);ps(" Milk=");pi(st.n_milk);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cattle Admin Demo ===\n\n");catt_init();
/* 1=ranch 2=dairy 3=feedlot 4=pasture 5=range */
ps("Cattle grazing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,hc=10+(i*5),aw=300+(i*20),my=5+(i*2),cc=2+(i%5),ga=20+(i*10),vc=50+(i*20);catt_grazing(lc,hc,aw,my,cc,ga,vc);}
ps("\nCattle breeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,hc=12+(i*4),aw=350+(i*18),my=6+(i*2),cc=3+(i%4),ga=25+(i*8),vc=60+(i*15);catt_breeding(lc,hc,aw,my,cc,ga,vc);}
ps("\nCattle veterinary...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,hc=15+(i*3),aw=400+(i*15),my=7+(i*2),cc=4+(i%3),ga=30+(i*6),vc=70+(i*12);catt_veterinary(lc,hc,aw,my,cc,ga,vc);}
ps("\nCattle milking...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,hc=8+(i*6),aw=280+(i*22),my=4+(i*3),cc=1+(i%5),ga=15+(i*12),vc=40+(i*25);catt_milking(lc,hc,aw,my,cc,ga,vc);}
ps("\nCattle market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,hc=18+(i*3),aw=450+(i*12),my=8+(i*2),cc=5+(i%3),ga=35+(i*5),vc=80+(i*10);catt_market(lc,hc,aw,my,cc,ga,vc);}
ps("\n");catt_report();catt_state();ps("\n=== Demo Complete ===\n");return 0;}
