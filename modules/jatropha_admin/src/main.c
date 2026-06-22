/* jatropha_admin: Jatropha management technology administration (v1.0)
 * Jatropha planning, jatropha execution, jatropha evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jatr_t;
typedef struct{int n_jatrp,n_jatr,n_jatr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jatr_state_t;
static jatr_t jatrs[N],jatre[N-2],jatr2[N-4],jatraс[N-6],jatram[N-6]; static jatr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(jatr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jatr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JATR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jatr_init(void){if(init)return -1;st.n_jatrp=0;st.n_jatr=0;st.n_jatr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jatrs[i].active=0;for(int i=0;i<N-2;i++)jatre[i].active=0;for(int i=0;i<N-4;i++)jatr2[i].active=0;for(int i=0;i<N-6;i++)jatraс[i].active=0;for(int i=0;i<N-6;i++)jatram[i].active=0;init=1;ps("[JATR] Jatropha initialized\n");return 0;}
int jatr_planning(int t,int c,int a,int b,int d,int e,int y){return add(jatrs,&st.n_jatrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jatr_execution(int t,int c,int a,int b,int d,int e,int y){return add(jatre,&st.n_jatr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jatr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jatr2,&st.n_jatr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jatr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jatraс,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jatr_market(int t,int c,int a,int b,int d,int e,int y){return add(jatram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jatr_report(void){ps("[JATR] Jatrp: ");pi(st.n_jatrp);ps(" PCS=");pi(st.t_f1);ps("\nJatre: ");pi(st.n_jatr);ps(" PCS=");pi(st.t_f2);ps("\nJatr2: ");pi(st.n_jatr2);ps(" PCS=");pi(st.t_f3);ps("\nJac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jatr_state(void){ps("[JATR] Jatrp=");pi(st.n_jatrp);ps(" Jatr=");pi(st.n_jatr);ps(" Jatr2=");pi(st.n_jatr2);ps(" Jac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jatropha Admin Demo ===\n\n");jatr_init();
ps("Jatropha planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jatr_planning(t,c,1546+(i*17),1535+(i*14),1515+(i*10),1497+(i*6),2020+(i%5));}
ps("\nJatropha execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jatr_execution(t,c,1535+(i*15),1524+(i*12),1506+(i*8),1493+(i*5),2021+(i%4));}
ps("\nJatropha evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jatr_evaluation(t,c,1527+(i*13),1516+(i*10),1500+(i*7),1489+(i*4),2022+(i%3));}
ps("\nJatropha accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jatr_accessory(t,c,1519+(i*11),1510+(i*9),1496+(i*6),1486+(i*3),2023+(i%2));}
ps("\nJatropha marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jatr_market(t,c,1513+(i*9),1504+(i*7),1491+(i*5),1483+(i*3),2024);}
ps("\n");jatr_report();jatr_state();ps("\n=== Demo Complete ===\n");return 0;}
