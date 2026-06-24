/* date_admin: Date (Phoenix dactylifera) date palm tree management (v1.0)
 * Date planting, irrigation, pollination, harvest, market
 * Features: tree_height_m, trunk_dia_cm, frond_count, fruit_wt_kg, yield_idx, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,trk_dia,frnd_ct,fr_wt,yld_idx,hv_wk,active;} date_t;
typedef struct{int n_plant,n_irri,n_poll,n_harv,n_mkt,t_ht,t_dia,t_fr,t_fw,t_yl;} date_state_t;
static date_t dpl[N],dir[N-2],dpo[N-4],dhv[N-6],dmk[N-6]; static date_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(date_t*a,int*cnt,int*sum,int mx,int lc,int th,int td,int fc,int fw,int yi,int hw){if(*cnt>=mx)return -1;date_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->trk_dia=td;x->frnd_ct=fc;x->fr_wt=fw;x->yld_idx=yi;x->hv_wk=hw;x->active=1;*sum+=th;(*cnt)++;ps("[DATE] Date ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" td=");pi(td);ps(" fc=");pi(fc);ps(" fw=");pi(fw);ps(" yi=");pi(yi);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int date_init(void){if(init)return -1;st.n_plant=0;st.n_irri=0;st.n_poll=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_dia=0;st.t_fr=0;st.t_fw=0;st.t_yl=0;for(int i=0;i<N;i++)dpl[i].active=0;for(int i=0;i<N-2;i++)dir[i].active=0;for(int i=0;i<N-4;i++)dpo[i].active=0;for(int i=0;i<N-6;i++)dhv[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DATE] Date initialized\n");return 0;}
int date_planting(int lc,int th,int td,int fc,int fw,int yi,int hw){return add(dpl,&st.n_plant,&st.t_ht,N,lc,th,td,fc,fw,yi,hw);}
int date_irrigation(int lc,int th,int td,int fc,int fw,int yi,int hw){return add(dir,&st.n_irri,&st.t_dia,N-2,lc,th,td,fc,fw,yi,hw);}
int date_pollination(int lc,int th,int td,int fc,int fw,int yi,int hw){return add(dpo,&st.n_poll,&st.t_fr,N-4,lc,th,td,fc,fw,yi,hw);}
int date_harvest(int lc,int th,int td,int fc,int fw,int yi,int hw){return add(dhv,&st.n_harv,&st.t_fw,N-6,lc,th,td,fc,fw,yi,hw);}
int date_market(int lc,int th,int td,int fc,int fw,int yi,int hw){return add(dmk,&st.n_mkt,&st.t_yl,N-6,lc,th,td,fc,fw,yi,hw);}
void date_report(void){ps("[DATE] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nIrri: ");pi(st.n_irri);ps(" Dia=");pi(st.t_dia);ps("\nPoll: ");pi(st.n_poll);ps(" Fr=");pi(st.t_fr);ps("\nHarv: ");pi(st.n_harv);ps(" Fw=");pi(st.t_fw);ps("\nMkt: ");pi(st.n_mkt);ps(" Yl=");pi(st.t_yl);ps("\n");}
void date_state(void){ps("[DATE] Plant=");pi(st.n_plant);ps(" Irri=");pi(st.n_irri);ps(" Poll=");pi(st.n_poll);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Date Admin Demo ===\n\n");date_init();
/* 1=oasis 2=plantation 3=grove 4=desert 5=market */
ps("Date planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=8+(i*2),td=20+(i*5),fc=30+(i*4),fw=15+(i*6),yi=(i%8)+1,hw=(i%12)+1;date_planting(lc,th,td,fc,fw,yi,hw);}
ps("\nDate irrigation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=10+(i*2),td=25+(i*4),fc=35+(i*3),fw=18+(i*5),yi=(i%7)+1,hw=(i%10)+1;date_irrigation(lc,th,td,fc,fw,yi,hw);}
ps("\nDate pollination...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=12+(i*2),td=30+(i*3),fc=40+(i*3),fw=20+(i*4),yi=(i%6)+1,hw=(i%8)+1;date_pollination(lc,th,td,fc,fw,yi,hw);}
ps("\nDate harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=9+(i*3),td=22+(i*5),fc=28+(i*5),fw=12+(i*7),yi=(i%9)+1,hw=(i%11)+1;date_harvest(lc,th,td,fc,fw,yi,hw);}
ps("\nDate market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=14+(i*2),td=35+(i*3),fc=45+(i*3),fw=25+(i*3),yi=(i%5)+4,hw=(i%6)+4;date_market(lc,th,td,fc,fw,yi,hw);}
ps("\n");date_report();date_state();ps("\n=== Demo Complete ===\n");return 0;}
