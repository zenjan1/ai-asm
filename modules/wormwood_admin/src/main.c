/* wormwood_admin: Wormwood management technology administration (v1.0)
 * Wormwood planning, wormwood execution, wormwood evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wor_t;
typedef struct{int n_worp,n_wore,n_wor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wor_state_t;
static wor_t worps[N],worss[N-2],worvss[N-4],woras[N-6],wormks[N-6]; static wor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wor_init(void){if(init)return -1;st.n_worp=0;st.n_wore=0;st.n_wor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)worps[i].active=0;for(int i=0;i<N-2;i++)worss[i].active=0;for(int i=0;i<N-4;i++)worvss[i].active=0;for(int i=0;i<N-6;i++)woras[i].active=0;for(int i=0;i<N-6;i++)wormks[i].active=0;init=1;ps("[WOR] Wormwood initialized\n");return 0;}
int wor_planning(int t,int c,int a,int b,int d,int e,int y){return add(worps,&st.n_worp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wor_execution(int t,int c,int a,int b,int d,int e,int y){return add(worss,&st.n_wore,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(worvss,&st.n_wor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(woras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wor_market(int t,int c,int a,int b,int d,int e,int y){return add(wormks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wor_report(void){ps("[WOR] Worp: ");pi(st.n_worp);ps(" PCS=");pi(st.t_f1);ps("\nWore: ");pi(st.n_wore);ps(" PCS=");pi(st.t_f2);ps("\nWorv: ");pi(st.n_wor2);ps(" PCS=");pi(st.t_f3);ps("\nWorc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wor_state(void){ps("[WOR] Worp=");pi(st.n_worp);ps(" Wore=");pi(st.n_wore);ps(" Worv=");pi(st.n_wor2);ps(" Worc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wormwood Admin Demo ===\n\n");wor_init();
ps("Wormwood planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wor_planning(t,c,960+(i*17),949+(i*14),929+(i*10),911+(i*6),2020+(i%5));}
ps("\nWormwood execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wor_execution(t,c,949+(i*15),938+(i*12),920+(i*8),907+(i*5),2021+(i%4));}
ps("\nWormwood evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wor_evaluation(t,c,941+(i*13),930+(i*10),914+(i*7),903+(i*4),2022+(i%3));}
ps("\nWormwood accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wor_accessory(t,c,933+(i*11),924+(i*9),910+(i*6),900+(i*3),2023+(i%2));}
ps("\nWormwood marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wor_market(t,c,927+(i*9),918+(i*7),905+(i*5),897+(i*3),2024);}
ps("\n");wor_report();wor_state();ps("\n=== Demo Complete ===\n");return 0;}
