/* corn_admin: Corn (Zea mays) grain crop management (v1.0)
 * Corn planting, pollination, pest control, harvest, market
 * Features: plant_ht_cm, ear_count, kernel_row, cob_len_cm, starch_pct, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,ear_ct,kr_row,cob_ln,strch_pct,harv_wk,active;} crn_t;
typedef struct{int n_plant,n_poll,n_pest,n_harv,n_mkt,t_ht,t_ear,t_row,t_cob,t_strch;} crn_state_t;
static crn_t crnps[N],crnpl[N-2],crnpt[N-4],crnhs[N-6],crnms[N-6]; static crn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crn_t*a,int*cnt,int*sum,int mx,int lc,int ph,int ec,int kr,int cl,int sp,int hw){if(*cnt>=mx)return -1;crn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->ear_ct=ec;x->kr_row=kr;x->cob_ln=cl;x->strch_pct=sp;x->harv_wk=hw;x->active=1;*sum+=ph;(*cnt)++;ps("[CRN] Corn ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" ec=");pi(ec);ps(" kr=");pi(kr);ps(" cl=");pi(cl);ps(" sp=");pi(sp);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int crn_init(void){if(init)return -1;st.n_plant=0;st.n_poll=0;st.n_pest=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_ear=0;st.t_row=0;st.t_cob=0;st.t_strch=0;for(int i=0;i<N;i++)crnps[i].active=0;for(int i=0;i<N-2;i++)crnpl[i].active=0;for(int i=0;i<N-4;i++)crnpt[i].active=0;for(int i=0;i<N-6;i++)crnhs[i].active=0;for(int i=0;i<N-6;i++)crnms[i].active=0;init=1;ps("[CRN] Corn initialized\n");return 0;}
int crn_planting(int lc,int ph,int ec,int kr,int cl,int sp,int hw){return add(crnps,&st.n_plant,&st.t_ht,N,lc,ph,ec,kr,cl,sp,hw);}
int crn_pollination(int lc,int ph,int ec,int kr,int cl,int sp,int hw){return add(crnpl,&st.n_poll,&st.t_ear,N-2,lc,ph,ec,kr,cl,sp,hw);}
int crn_pest_control(int lc,int ph,int ec,int kr,int cl,int sp,int hw){return add(crnpt,&st.n_pest,&st.t_row,N-4,lc,ph,ec,kr,cl,sp,hw);}
int crn_harvest(int lc,int ph,int ec,int kr,int cl,int sp,int hw){return add(crnhs,&st.n_harv,&st.t_cob,N-6,lc,ph,ec,kr,cl,sp,hw);}
int crn_market(int lc,int ph,int ec,int kr,int cl,int sp,int hw){return add(crnms,&st.n_mkt,&st.t_strch,N-6,lc,ph,ec,kr,cl,sp,hw);}
void crn_report(void){ps("[CRN] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nPoll: ");pi(st.n_poll);ps(" Ear=");pi(st.t_ear);ps("\nPest: ");pi(st.n_pest);ps(" Row=");pi(st.t_row);ps("\nHarv: ");pi(st.n_harv);ps(" Cob=");pi(st.t_cob);ps("\nMkt: ");pi(st.n_mkt);ps(" Strch=");pi(st.t_strch);ps("\n");}
void crn_state(void){ps("[CRN] Plant=");pi(st.n_plant);ps(" Poll=");pi(st.n_poll);ps(" Pest=");pi(st.n_pest);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Corn Admin Demo ===\n\n");crn_init();
/* 1=field 2=irrigated 3=organic 4=greenhouse 5=market */
ps("Corn planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=150+(i*15),ec=1+(i%3),kr=12+(i%4),cl=15+(i*2),sp=60+(i*3),hw=14+(i%6);crn_planting(lc,ph,ec,kr,cl,sp,hw);}
ps("\nCorn pollination...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=160+(i*12),ec=2+(i%2),kr=13+(i%3),cl=16+(i*2),sp=62+(i*3),hw=15+(i%5);crn_pollination(lc,ph,ec,kr,cl,sp,hw);}
ps("\nCorn pest control...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=170+(i*10),ec=2+(i%2),kr=14+(i%2),cl=17+(i*2),sp=64+(i*2),hw=16+(i%4);crn_pest_control(lc,ph,ec,kr,cl,sp,hw);}
ps("\nCorn harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=140+(i*18),ec=1+(i%4),kr=11+(i%5),cl=14+(i*3),sp=58+(i*4),hw=12+(i%7);crn_harvest(lc,ph,ec,kr,cl,sp,hw);}
ps("\nCorn market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=180+(i*8),ec=3+(i%2),kr=15+(i*2),cl=18+(i*2),sp=66+(i*2),hw=17+(i%3);crn_market(lc,ph,ec,kr,cl,sp,hw);}
ps("\n");crn_report();crn_state();ps("\n=== Demo Complete ===\n");return 0;}
