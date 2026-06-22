/* jaeckia_admin: Jaeckia management technology administration (v1.0)
 * Jaeckia planning, jaeckia execution, jaeckia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jaec_t;
typedef struct{int n_jaecp,n_jaec,n_jaec2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jaec_state_t;
static jaec_t jaeccs[N],jaec_e[N-2],jaec2[N-4],jaecac[N-6],jaecam[N-6]; static jaec_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(jaec_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jaec_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JAEC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jaec_init(void){if(init)return -1;st.n_jaecp=0;st.n_jaec=0;st.n_jaec2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jaeccs[i].active=0;for(int i=0;i<N-2;i++)jaec_e[i].active=0;for(int i=0;i<N-4;i++)jaec2[i].active=0;for(int i=0;i<N-6;i++)jaecac[i].active=0;for(int i=0;i<N-6;i++)jaecam[i].active=0;init=1;ps("[JAEC] Jaeckia initialized\n");return 0;}
int jaec_planning(int t,int c,int a,int b,int d,int e,int y){return add(jaeccs,&st.n_jaecp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jaec_execution(int t,int c,int a,int b,int d,int e,int y){return add(jaec_e,&st.n_jaec,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jaec_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jaec2,&st.n_jaec2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jaec_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jaecac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jaec_market(int t,int c,int a,int b,int d,int e,int y){return add(jaecam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jaec_report(void){ps("[JAEC] Jaecp: ");pi(st.n_jaecp);ps(" PCS=");pi(st.t_f1);ps("\nJaec: ");pi(st.n_jaec);ps(" PCS=");pi(st.t_f2);ps("\nJaec2: ");pi(st.n_jaec2);ps(" PCS=");pi(st.t_f3);ps("\nJac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jaec_state(void){ps("[JAEC] Jaecp=");pi(st.n_jaecp);ps(" Jaec=");pi(st.n_jaec);ps(" Jaec2=");pi(st.n_jaec2);ps(" Jac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jaeckia Admin Demo ===\n\n");jaec_init();
ps("Jaeckia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jaec_planning(t,c,1543+(i*17),1532+(i*14),1512+(i*10),1494+(i*6),2020+(i%5));}
ps("\nJaeckia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jaec_execution(t,c,1532+(i*15),1521+(i*12),1503+(i*8),1490+(i*5),2021+(i%4));}
ps("\nJaeckia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jaec_evaluation(t,c,1524+(i*13),1513+(i*10),1497+(i*7),1486+(i*4),2022+(i%3));}
ps("\nJaeckia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jaec_accessory(t,c,1516+(i*11),1507+(i*9),1493+(i*6),1483+(i*3),2023+(i%2));}
ps("\nJaeckia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jaec_market(t,c,1510+(i*9),1501+(i*7),1488+(i*5),1480+(i*3),2024);}
ps("\n");jaec_report();jaec_state();ps("\n=== Demo Complete ===\n");return 0;}
