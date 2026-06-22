/* zizania_admin: Zizania management technology administration (v1.0)
 * Zizania planning, zizania execution, zizania evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ziz_t;
typedef struct{int n_zizp,n_zize,n_ziz2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ziz_state_t;
static ziz_t zizps[N],zizss[N-2],zizvss[N-4],zizas[N-6],zizmks[N-6]; static ziz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ziz_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ziz_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZIZ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ziz_init(void){if(init)return -1;st.n_zizp=0;st.n_zize=0;st.n_ziz2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zizps[i].active=0;for(int i=0;i<N-2;i++)zizss[i].active=0;for(int i=0;i<N-4;i++)zizvss[i].active=0;for(int i=0;i<N-6;i++)zizas[i].active=0;for(int i=0;i<N-6;i++)zizmks[i].active=0;init=1;ps("[ZIZ] Zizania initialized\n");return 0;}
int ziz_planning(int t,int c,int a,int b,int d,int e,int y){return add(zizps,&st.n_zizp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ziz_execution(int t,int c,int a,int b,int d,int e,int y){return add(zizss,&st.n_zize,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ziz_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zizvss,&st.n_ziz2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ziz_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zizas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ziz_market(int t,int c,int a,int b,int d,int e,int y){return add(zizmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ziz_report(void){ps("[ZIZ] Zizpp: ");pi(st.n_zizp);ps(" PCS=");pi(st.t_f1);ps("\nZizpe: ");pi(st.n_zize);ps(" PCS=");pi(st.t_f2);ps("\nZizv: ");pi(st.n_ziz2);ps(" PCS=");pi(st.t_f3);ps("\nZizc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ziz_state(void){ps("[ZIZ] Zizpp=");pi(st.n_zizp);ps(" Zizpe=");pi(st.n_zize);ps(" Zizv=");pi(st.n_ziz2);ps(" Zizc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zizania Admin Demo ===\n\n");ziz_init();
ps("Zizania planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ziz_planning(t,c,1206+(i*17),1195+(i*14),1175+(i*10),1157+(i*6),2020+(i%5));}
ps("\nZizania execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ziz_execution(t,c,1195+(i*15),1184+(i*12),1166+(i*8),1153+(i*5),2021+(i%4));}
ps("\nZizania evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ziz_evaluation(t,c,1187+(i*13),1176+(i*10),1160+(i*7),1149+(i*4),2022+(i%3));}
ps("\nZizania accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ziz_accessory(t,c,1179+(i*11),1170+(i*9),1156+(i*6),1146+(i*3),2023+(i%2));}
ps("\nZizania marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ziz_market(t,c,1173+(i*9),1164+(i*7),1151+(i*5),1143+(i*3),2024);}
ps("\n");ziz_report();ziz_state();ps("\n=== Demo Complete ===\n");return 0;}
