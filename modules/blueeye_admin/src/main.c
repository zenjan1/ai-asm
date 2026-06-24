/* blueeye_admin: Blue-eye (Sisyrinchium) grass-like perennial management (v1.0)
 * Blueeye planning, planting, evaluation, division, market
 * Features: flower diameter, leaf width, plant height, flower color, corm size, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,leaf_wd,plant_ht,flower_color,corm_sz,bloom_wk,active;} ble_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_flower,t_leaf,t_plant,t_color,t_corm;} ble_state_t;
static ble_t bleps[N],bles[N-2],blesv[N-4],bledv[N-6],blems[N-6]; static ble_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ble_t*a,int*cnt,int*sum,int mx,int lc,int fd,int lw,int ph,int fc,int cs,int bw){if(*cnt>=mx)return -1;ble_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->leaf_wd=lw;x->plant_ht=ph;x->flower_color=fc;x->corm_sz=cs;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[BLE] Blueeye ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" lw=");pi(lw);ps(" ph=");pi(ph);ps(" fc=");pi(fc);ps(" cs=");pi(cs);ps("\n");return *cnt-1;}
int ble_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_plant=0;st.t_color=0;st.t_corm=0;for(int i=0;i<N;i++)bleps[i].active=0;for(int i=0;i<N-2;i++)bles[i].active=0;for(int i=0;i<N-4;i++)blesv[i].active=0;for(int i=0;i<N-6;i++)bledv[i].active=0;for(int i=0;i<N-6;i++)blems[i].active=0;init=1;ps("[BLE] Blueeye initialized\n");return 0;}
/* 1=rockery 2=alpine 3=moist_border 4=woodland 5=container */
int ble_planning(int lc,int fd,int lw,int ph,int fc,int cs,int bw){return add(bleps,&st.n_plan,&st.t_flower,N,lc,fd,lw,ph,fc,cs,bw);}
int ble_execution(int lc,int fd,int lw,int ph,int fc,int cs,int bw){return add(bles,&st.n_exec,&st.t_leaf,N-2,lc,fd,lw,ph,fc,cs,bw);}
int ble_evaluation(int lc,int fd,int lw,int ph,int fc,int cs,int bw){return add(blesv,&st.n_eval,&st.t_plant,N-4,lc,fd,lw,ph,fc,cs,bw);}
int ble_division(int lc,int fd,int lw,int ph,int fc,int cs,int bw){return add(bledv,&st.n_div,&st.t_color,N-6,lc,fd,lw,ph,fc,cs,bw);}
int ble_market(int lc,int fd,int lw,int ph,int fc,int cs,int bw){return add(blems,&st.n_mkt,&st.t_corm,N-6,lc,fd,lw,ph,fc,cs,bw);}
void ble_report(void){ps("[BLE] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" plant=");pi(st.t_plant);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" corm=");pi(st.t_corm);ps("\n");}
void ble_state(void){ps("[BLE] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Blue-eye (Sisyrinchium) Admin Demo ===\n\n");ble_init();
ps("Blueeye planning (rockery layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ble_planning(lc,1+(i*1),2+(i*1),12+(i*3),(i%4)+1,2+(i*1),18+(i%6));}
ps("\nBlueeye execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ble_execution(lc,2+(i*1),3+(i*1),14+(i*2),(i%4)+1,3+(i*1),20+(i%5));}
ps("\nBlueeye evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ble_evaluation(lc,3+(i*1),4+(i*1),16+(i*2),(i%3)+2,4+(i*1),22+(i%4));}
ps("\nBlueeye division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ble_division(lc,1+(i*1),2+(i*1),10+(i*3),(i%4)+1,2+(i*1),16+(i%5));}
ps("\nBlueeye market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ble_market(lc,4+(i*1),5+(i*1),18+(i*2),(i%3)+3,5+(i*1),24+(i%3));}
ps("\n");ble_report();ble_state();ps("\n=== Demo Complete ===\n");return 0;}
