/* wulfenia_admin: Wulfenia management technology administration (v1.0)
 * Wulfenia planning, wulfenia execution, wulfenia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wul_t;
typedef struct{int n_wulp,n_wule,n_wul2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wul_state_t;
static wul_t wulps[N],wulss[N-2],wulvss[N-4],wulas[N-6],wulmks[N-6]; static wul_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wul_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wul_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WUL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wul_init(void){if(init)return -1;st.n_wulp=0;st.n_wule=0;st.n_wul2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wulps[i].active=0;for(int i=0;i<N-2;i++)wulss[i].active=0;for(int i=0;i<N-4;i++)wulvss[i].active=0;for(int i=0;i<N-6;i++)wulas[i].active=0;for(int i=0;i<N-6;i++)wulmks[i].active=0;init=1;ps("[WUL] Wulfenia initialized\n");return 0;}
int wul_planning(int t,int c,int a,int b,int d,int e,int y){return add(wulps,&st.n_wulp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wul_execution(int t,int c,int a,int b,int d,int e,int y){return add(wulss,&st.n_wule,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wul_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wulvss,&st.n_wul2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wul_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wulas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wul_market(int t,int c,int a,int b,int d,int e,int y){return add(wulmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wul_report(void){ps("[WUL] Wulpp: ");pi(st.n_wulp);ps(" PCS=");pi(st.t_f1);ps("\nWule: ");pi(st.n_wule);ps(" PCS=");pi(st.t_f2);ps("\nWulv: ");pi(st.n_wul2);ps(" PCS=");pi(st.t_f3);ps("\nWulc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wul_state(void){ps("[WUL] Wulpp=");pi(st.n_wulp);ps(" Wule=");pi(st.n_wule);ps(" Wulv=");pi(st.n_wul2);ps(" Wulc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wulfenia Admin Demo ===\n\n");wul_init();
ps("Wulfenia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wul_planning(t,c,1112+(i*17),1101+(i*14),1081+(i*10),1063+(i*6),2020+(i%5));}
ps("\nWulfenia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wul_execution(t,c,1101+(i*15),1090+(i*12),1072+(i*8),1059+(i*5),2021+(i%4));}
ps("\nWulfenia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wul_evaluation(t,c,1093+(i*13),1082+(i*10),1066+(i*7),1055+(i*4),2022+(i%3));}
ps("\nWulfenia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wul_accessory(t,c,1085+(i*11),1076+(i*9),1062+(i*6),1052+(i*3),2023+(i%2));}
ps("\nWulfenia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wul_market(t,c,1079+(i*9),1070+(i*7),1057+(i*5),1049+(i*3),2024);}
ps("\n");wul_report();wul_state();ps("\n=== Demo Complete ===\n");return 0;}
