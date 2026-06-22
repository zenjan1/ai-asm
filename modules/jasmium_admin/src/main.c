/* jasmium_admin: Jasmium management technology administration (v1.0)
 * Jasmium planning, jasmium execution, jasmium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jasm_t;
typedef struct{int n_jasmp,n_jasm,n_jasm2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jasm_state_t;
static jasm_t jasms[N],jasme[N-2],jasm2[N-4],jasmac[N-6],jasmam[N-6]; static jasm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(jasm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jasm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JASM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jasm_init(void){if(init)return -1;st.n_jasmp=0;st.n_jasm=0;st.n_jasm2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jasms[i].active=0;for(int i=0;i<N-2;i++)jasme[i].active=0;for(int i=0;i<N-4;i++)jasm2[i].active=0;for(int i=0;i<N-6;i++)jasmac[i].active=0;for(int i=0;i<N-6;i++)jasmam[i].active=0;init=1;ps("[JASM] Jasmium initialized\n");return 0;}
int jasm_planning(int t,int c,int a,int b,int d,int e,int y){return add(jasms,&st.n_jasmp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jasm_execution(int t,int c,int a,int b,int d,int e,int y){return add(jasme,&st.n_jasm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jasm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jasm2,&st.n_jasm2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jasm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jasmac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jasm_market(int t,int c,int a,int b,int d,int e,int y){return add(jasmam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jasm_report(void){ps("[JASM] Jasmp: ");pi(st.n_jasmp);ps(" PCS=");pi(st.t_f1);ps("\nJasme: ");pi(st.n_jasm);ps(" PCS=");pi(st.t_f2);ps("\nJasm2: ");pi(st.n_jasm2);ps(" PCS=");pi(st.t_f3);ps("\nJac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jasm_state(void){ps("[JASM] Jasmp=");pi(st.n_jasmp);ps(" Jasm=");pi(st.n_jasm);ps(" Jasm2=");pi(st.n_jasm2);ps(" Jac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jasmium Admin Demo ===\n\n");jasm_init();
ps("Jasmium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jasm_planning(t,c,1545+(i*17),1534+(i*14),1514+(i*10),1496+(i*6),2020+(i%5));}
ps("\nJasmium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jasm_execution(t,c,1534+(i*15),1523+(i*12),1505+(i*8),1492+(i*5),2021+(i%4));}
ps("\nJasmium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jasm_evaluation(t,c,1526+(i*13),1515+(i*10),1499+(i*7),1488+(i*4),2022+(i%3));}
ps("\nJasmium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jasm_accessory(t,c,1518+(i*11),1509+(i*9),1495+(i*6),1485+(i*3),2023+(i%2));}
ps("\nJasmium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jasm_market(t,c,1512+(i*9),1503+(i*7),1490+(i*5),1482+(i*3),2024);}
ps("\n");jasm_report();jasm_state();ps("\n=== Demo Complete ===\n");return 0;}
