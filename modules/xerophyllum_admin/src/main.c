/* xerophyllum_admin: Xerophyllum management technology administration (v1.0)
 * Xerophyllum planning, xerophyllum execution, xerophyllum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} xer_t;
typedef struct{int n_xerp,n_xere,n_xer2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} xer_state_t;
static xer_t xerps[N],xerss[N-2],xervss[N-4],xeras[N-6],xermks[N-6]; static xer_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(xer_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;xer_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[XER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int xer_init(void){if(init)return -1;st.n_xerp=0;st.n_xere=0;st.n_xer2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)xerps[i].active=0;for(int i=0;i<N-2;i++)xerss[i].active=0;for(int i=0;i<N-4;i++)xervss[i].active=0;for(int i=0;i<N-6;i++)xeras[i].active=0;for(int i=0;i<N-6;i++)xermks[i].active=0;init=1;ps("[XER] Xerophyllum initialized\n");return 0;}
int xer_planning(int t,int c,int a,int b,int d,int e,int y){return add(xerps,&st.n_xerp,&st.t_f1,N,t,c,a,b,d,e,y);}
int xer_execution(int t,int c,int a,int b,int d,int e,int y){return add(xerss,&st.n_xere,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int xer_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(xervss,&st.n_xer2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int xer_accessory(int t,int c,int a,int b,int d,int e,int y){return add(xeras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int xer_market(int t,int c,int a,int b,int d,int e,int y){return add(xermks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void xer_report(void){ps("[XER] Xerpp: ");pi(st.n_xerp);ps(" PCS=");pi(st.t_f1);ps("\nXerpe: ");pi(st.n_xere);ps(" PCS=");pi(st.t_f2);ps("\nXerv: ");pi(st.n_xer2);ps(" PCS=");pi(st.t_f3);ps("\nXerc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void xer_state(void){ps("[XER] Xerpp=");pi(st.n_xerp);ps(" Xerpe=");pi(st.n_xere);ps(" Xerv=");pi(st.n_xer2);ps(" Xerc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Xerophyllum Admin Demo ===\n\n");xer_init();
ps("Xerophyllum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;xer_planning(t,c,1204+(i*17),1193+(i*14),1173+(i*10),1155+(i*6),2020+(i%5));}
ps("\nXerophyllum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;xer_execution(t,c,1193+(i*15),1182+(i*12),1164+(i*8),1151+(i*5),2021+(i%4));}
ps("\nXerophyllum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;xer_evaluation(t,c,1185+(i*13),1174+(i*10),1158+(i*7),1147+(i*4),2022+(i%3));}
ps("\nXerophyllum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xer_accessory(t,c,1177+(i*11),1168+(i*9),1154+(i*6),1144+(i*3),2023+(i%2));}
ps("\nXerophyllum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xer_market(t,c,1171+(i*9),1162+(i*7),1149+(i*5),1141+(i*3),2024);}
ps("\n");xer_report();xer_state();ps("\n=== Demo Complete ===\n");return 0;}
