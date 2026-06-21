/* wormwood_admin: Wormwood management technology administration (v1.0)
 * Wormwood planning, wormwood execution, wormwood evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wm_t;
typedef struct{int n_wmp,n_wme,n_wmv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wm_state_t;
static wm_t wmps[N],wmss[N-2],wmvss[N-4],wmas[N-6],wmmks[N-6]; static wm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WMD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wm_init(void){if(init)return -1;st.n_wmp=0;st.n_wme=0;st.n_wmv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wmps[i].active=0;for(int i=0;i<N-2;i++)wmss[i].active=0;for(int i=0;i<N-4;i++)wmvss[i].active=0;for(int i=0;i<N-6;i++)wmas[i].active=0;for(int i=0;i<N-6;i++)wmmks[i].active=0;init=1;ps("[WMD] Wormwood initialized\n");return 0;}
int wm_planning(int t,int c,int a,int b,int d,int e,int y){return add(wmps,&st.n_wmp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wm_execution(int t,int c,int a,int b,int d,int e,int y){return add(wmss,&st.n_wme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wmvss,&st.n_wmv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wmas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wm_market(int t,int c,int a,int b,int d,int e,int y){return add(wmmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wm_report(void){ps("[WMD] Wmp: ");pi(st.n_wmp);ps(" PCS=");pi(st.t_f1);ps("\nWme: ");pi(st.n_wme);ps(" PCS=");pi(st.t_f2);ps("\nWmv: ");pi(st.n_wmv);ps(" PCS=");pi(st.t_f3);ps("\nWmc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wm_state(void){ps("[WMD] Wmp=");pi(st.n_wmp);ps(" Wme=");pi(st.n_wme);ps(" Wmv=");pi(st.n_wmv);ps(" Wmc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wormwood Admin Demo ===\n\n");wm_init();
ps("Wormwood planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wm_planning(t,c,682+(i*17),671+(i*14),651+(i*10),633+(i*6),2020+(i%5));}
ps("\nWormwood execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wm_execution(t,c,671+(i*15),660+(i*12),642+(i*8),629+(i*5),2021+(i%4));}
ps("\nWormwood evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wm_evaluation(t,c,663+(i*13),652+(i*10),636+(i*7),625+(i*4),2022+(i%3));}
ps("\nWormwood accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wm_accessory(t,c,655+(i*11),646+(i*9),632+(i*6),622+(i*3),2023+(i%2));}
ps("\nWormwood marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wm_market(t,c,649+(i*9),640+(i*7),627+(i*5),619+(i*3),2024);}
ps("\n");wm_report();wm_state();ps("\n=== Demo Complete ===\n");return 0;}
