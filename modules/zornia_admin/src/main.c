/* zornia_admin: Zornia management technology administration (v1.0)
 * Zornia planning, zornia execution, zornia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zor_t;
typedef struct{int n_zorp,n_zore,n_zor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zor_state_t;
static zor_t zorps[N],zorss[N-2],zorvss[N-4],zoras[N-6],zormks[N-6]; static zor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zor_init(void){if(init)return -1;st.n_zorp=0;st.n_zore=0;st.n_zor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zorps[i].active=0;for(int i=0;i<N-2;i++)zorss[i].active=0;for(int i=0;i<N-4;i++)zorvss[i].active=0;for(int i=0;i<N-6;i++)zoras[i].active=0;for(int i=0;i<N-6;i++)zormks[i].active=0;init=1;ps("[ZOR] Zornia initialized\n");return 0;}
int zor_planning(int t,int c,int a,int b,int d,int e,int y){return add(zorps,&st.n_zorp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zor_execution(int t,int c,int a,int b,int d,int e,int y){return add(zorss,&st.n_zore,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zorvss,&st.n_zor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zoras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zor_market(int t,int c,int a,int b,int d,int e,int y){return add(zormks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zor_report(void){ps("[ZOR] Zorpp: ");pi(st.n_zorp);ps(" PCS=");pi(st.t_f1);ps("\nZore: ");pi(st.n_zore);ps(" PCS=");pi(st.t_f2);ps("\nZorv: ");pi(st.n_zor2);ps(" PCS=");pi(st.t_f3);ps("\nZorc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zor_state(void){ps("[ZOR] Zorpp=");pi(st.n_zorp);ps(" Zore=");pi(st.n_zore);ps(" Zorv=");pi(st.n_zor2);ps(" Zorc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zornia Admin Demo ===\n\n");zor_init();
ps("Zornia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zor_planning(t,c,1116+(i*17),1105+(i*14),1085+(i*10),1067+(i*6),2020+(i%5));}
ps("\nZornia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zor_execution(t,c,1105+(i*15),1094+(i*12),1076+(i*8),1063+(i*5),2021+(i%4));}
ps("\nZornia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zor_evaluation(t,c,1097+(i*13),1086+(i*10),1070+(i*7),1059+(i*4),2022+(i%3));}
ps("\nZornia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zor_accessory(t,c,1089+(i*11),1080+(i*9),1066+(i*6),1056+(i*3),2023+(i%2));}
ps("\nZornia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zor_market(t,c,1083+(i*9),1074+(i*7),1061+(i*5),1053+(i*3),2024);}
ps("\n");zor_report();zor_state();ps("\n=== Demo Complete ===\n");return 0;}
