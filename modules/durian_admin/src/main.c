/* durian_admin: Durian (Durio zibethinus) king of fruits tree management (v1.0)
 * Durian planting, irrigation, flowering, harvest, market
 * Features: tree_height_m, fruit_wt_kg, spine_idx, aroma_lv, seed_ct, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,fr_wt,spn_idx,arm_lv,sd_ct,hv_wk,active;} dur_t;
typedef struct{int n_plant,n_irri,n_flow,n_harv,n_mkt,t_ht,t_fw,t_sp,t_ar,t_sd;} dur_state_t;
static dur_t dpl[N],dir[N-2],dfl[N-4],dhv[N-6],dmk[N-6]; static dur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dur_t*a,int*cnt,int*sum,int mx,int lc,int th,int fw,int si,int al,int sc,int hw){if(*cnt>=mx)return -1;dur_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->fr_wt=fw;x->spn_idx=si;x->arm_lv=al;x->sd_ct=sc;x->hv_wk=hw;x->active=1;*sum+=th;(*cnt)++;ps("[DUR] Durian ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" fw=");pi(fw);ps(" si=");pi(si);ps(" al=");pi(al);ps(" sc=");pi(sc);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int dur_init(void){if(init)return -1;st.n_plant=0;st.n_irri=0;st.n_flow=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_fw=0;st.t_sp=0;st.t_ar=0;st.t_sd=0;for(int i=0;i<N;i++)dpl[i].active=0;for(int i=0;i<N-2;i++)dir[i].active=0;for(int i=0;i<N-4;i++)dfl[i].active=0;for(int i=0;i<N-6;i++)dhv[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DUR] Durian initialized\n");return 0;}
int dur_planting(int lc,int th,int fw,int si,int al,int sc,int hw){return add(dpl,&st.n_plant,&st.t_ht,N,lc,th,fw,si,al,sc,hw);}
int dur_irrigation(int lc,int th,int fw,int si,int al,int sc,int hw){return add(dir,&st.n_irri,&st.t_fw,N-2,lc,th,fw,si,al,sc,hw);}
int dur_flowering(int lc,int th,int fw,int si,int al,int sc,int hw){return add(dfl,&st.n_flow,&st.t_sp,N-4,lc,th,fw,si,al,sc,hw);}
int dur_harvest(int lc,int th,int fw,int si,int al,int sc,int hw){return add(dhv,&st.n_harv,&st.t_ar,N-6,lc,th,fw,si,al,sc,hw);}
int dur_market(int lc,int th,int fw,int si,int al,int sc,int hw){return add(dmk,&st.n_mkt,&st.t_sd,N-6,lc,th,fw,si,al,sc,hw);}
void dur_report(void){ps("[DUR] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nIrri: ");pi(st.n_irri);ps(" Fw=");pi(st.t_fw);ps("\nFlow: ");pi(st.n_flow);ps(" Sp=");pi(st.t_sp);ps("\nHarv: ");pi(st.n_harv);ps(" Ar=");pi(st.t_ar);ps("\nMkt: ");pi(st.n_mkt);ps(" Sd=");pi(st.t_sd);ps("\n");}
void dur_state(void){ps("[DUR] Plant=");pi(st.n_plant);ps(" Irri=");pi(st.n_irri);ps(" Flow=");pi(st.n_flow);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Durian Admin Demo ===\n\n");dur_init();
/* 1=orchard 2=plantation 3=grove 4=hill 5=market */
ps("Durian planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=15+(i*3),fw=2+(i%3),si=(i%6)+1,al=(i%8)+1,sc=15+(i*5),hw=(i%12)+1;dur_planting(lc,th,fw,si,al,sc,hw);}
ps("\nDurian irrigation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=18+(i*2),fw=3+(i%2),si=(i%5)+1,al=(i%7)+1,sc=18+(i*4),hw=(i%10)+1;dur_irrigation(lc,th,fw,si,al,sc,hw);}
ps("\nDurian flowering...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=20+(i*2),fw=4+(i%2),si=(i%4)+1,al=(i%6)+1,sc=20+(i*3),hw=(i%8)+1;dur_flowering(lc,th,fw,si,al,sc,hw);}
ps("\nDurian harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=16+(i*4),fw=2+(i%4),si=(i%7)+1,al=(i%5)+1,sc=12+(i*6),hw=(i%11)+1;dur_harvest(lc,th,fw,si,al,sc,hw);}
ps("\nDurian market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=22+(i*2),fw=5+(i%2),si=(i%3)+4,al=(i%4)+4,sc=25+(i*3),hw=(i%6)+4;dur_market(lc,th,fw,si,al,sc,hw);}
ps("\n");dur_report();dur_state();ps("\n=== Demo Complete ===\n");return 0;}
