/* jacquemontia_admin: Jacquemontia management technology administration (v1.0)
 * Jacquemontia planning, jacquemontia execution, jacquemontia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jaqu_t;
typedef struct{int n_jaqup,n_jaqu,n_jaqu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jaqu_state_t;
static jaqu_t jaqus[N],jaque[N-2],jaqu2[N-4],jaquac[N-6],jaquam[N-6]; static jaqu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(jaqu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jaqu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JAQU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jaqu_init(void){if(init)return -1;st.n_jaqup=0;st.n_jaqu=0;st.n_jaqu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jaqus[i].active=0;for(int i=0;i<N-2;i++)jaque[i].active=0;for(int i=0;i<N-4;i++)jaqu2[i].active=0;for(int i=0;i<N-6;i++)jaquac[i].active=0;for(int i=0;i<N-6;i++)jaquam[i].active=0;init=1;ps("[JAQU] Jacquemontia initialized\n");return 0;}
int jaqu_planning(int t,int c,int a,int b,int d,int e,int y){return add(jaqus,&st.n_jaqup,&st.t_f1,N,t,c,a,b,d,e,y);}
int jaqu_execution(int t,int c,int a,int b,int d,int e,int y){return add(jaque,&st.n_jaqu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jaqu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jaqu2,&st.n_jaqu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jaqu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jaquac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jaqu_market(int t,int c,int a,int b,int d,int e,int y){return add(jaquam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jaqu_report(void){ps("[JAQU] Jaqup: ");pi(st.n_jaqup);ps(" PCS=");pi(st.t_f1);ps("\nJaqu: ");pi(st.n_jaqu);ps(" PCS=");pi(st.t_f2);ps("\nJaqu2: ");pi(st.n_jaqu2);ps(" PCS=");pi(st.t_f3);ps("\nJac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jaqu_state(void){ps("[JAQU] Jaqup=");pi(st.n_jaqup);ps(" Jaqu=");pi(st.n_jaqu);ps(" Jaqu2=");pi(st.n_jaqu2);ps(" Jac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jacquemontia Admin Demo ===\n\n");jaqu_init();
ps("Jacquemontia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jaqu_planning(t,c,1542+(i*17),1531+(i*14),1511+(i*10),1493+(i*6),2020+(i%5));}
ps("\nJacquemontia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jaqu_execution(t,c,1531+(i*15),1520+(i*12),1502+(i*8),1489+(i*5),2021+(i%4));}
ps("\nJacquemontia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jaqu_evaluation(t,c,1523+(i*13),1512+(i*10),1496+(i*7),1485+(i*4),2022+(i%3));}
ps("\nJacquemontia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jaqu_accessory(t,c,1515+(i*11),1506+(i*9),1492+(i*6),1482+(i*3),2023+(i%2));}
ps("\nJacquemontia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jaqu_market(t,c,1509+(i*9),1500+(i*7),1487+(i*5),1479+(i*3),2024);}
ps("\n");jaqu_report();jaqu_state();ps("\n=== Demo Complete ===\n");return 0;}
