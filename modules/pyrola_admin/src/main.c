/* pyrola_admin: Pyrola management technology administration (v1.0)
 * Pyrola planning, pyrola execution, pyrola evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pyr_t;
typedef struct{int n_pyrp,n_pyre,n_pyr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pyr_state_t;
static pyr_t pyrps[N],pyrss[N-2],pyrvss[N-4],pyras[N-6],pyrmks[N-6]; static pyr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pyr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pyr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PYR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pyr_init(void){if(init)return -1;st.n_pyrp=0;st.n_pyre=0;st.n_pyr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pyrps[i].active=0;for(int i=0;i<N-2;i++)pyrss[i].active=0;for(int i=0;i<N-4;i++)pyrvss[i].active=0;for(int i=0;i<N-6;i++)pyras[i].active=0;for(int i=0;i<N-6;i++)pyrmks[i].active=0;init=1;ps("[PYR] Pyrola initialized\n");return 0;}
int pyr_planning(int t,int c,int a,int b,int d,int e,int y){return add(pyrps,&st.n_pyrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pyr_execution(int t,int c,int a,int b,int d,int e,int y){return add(pyrss,&st.n_pyre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pyr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pyrvss,&st.n_pyr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pyr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pyras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pyr_market(int t,int c,int a,int b,int d,int e,int y){return add(pyrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pyr_report(void){ps("[PYR] Pyrpp: ");pi(st.n_pyrp);ps(" PCS=");pi(st.t_f1);ps("\nPyre: ");pi(st.n_pyre);ps(" PCS=");pi(st.t_f2);ps("\nPyrv: ");pi(st.n_pyr2);ps(" PCS=");pi(st.t_f3);ps("\nPyrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pyr_state(void){ps("[PYR] Pyrpp=");pi(st.n_pyrp);ps(" Pyre=");pi(st.n_pyre);ps(" Pyrv=");pi(st.n_pyr2);ps(" Pyrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pyrola Admin Demo ===\n\n");pyr_init();
ps("Pyrola planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pyr_planning(t,c,1175+(i*17),1164+(i*14),1144+(i*10),1126+(i*6),2020+(i%5));}
ps("\nPyrola execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pyr_execution(t,c,1164+(i*15),1153+(i*12),1135+(i*8),1122+(i*5),2021+(i%4));}
ps("\nPyrola evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pyr_evaluation(t,c,1156+(i*13),1145+(i*10),1129+(i*7),1118+(i*4),2022+(i%3));}
ps("\nPyrola accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pyr_accessory(t,c,1148+(i*11),1139+(i*9),1125+(i*6),1115+(i*3),2023+(i%2));}
ps("\nPyrola marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pyr_market(t,c,1142+(i*9),1133+(i*7),1120+(i*5),1112+(i*3),2024);}
ps("\n");pyr_report();pyr_state();ps("\n=== Demo Complete ===\n");return 0;}
