/* cirsium_admin: Cirsium management technology administration (v1.0)
 * Cirsium planning, cirsium execution, cirsium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cir_t;
typedef struct{int n_cirp,n_cire,n_cir2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cir_state_t;
static cir_t cirps[N],cirss[N-2],cirvss[N-4],ciras[N-6],cirmks[N-6]; static cir_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cir_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cir_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CIR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cir_init(void){if(init)return -1;st.n_cirp=0;st.n_cire=0;st.n_cir2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cirps[i].active=0;for(int i=0;i<N-2;i++)cirss[i].active=0;for(int i=0;i<N-4;i++)cirvss[i].active=0;for(int i=0;i<N-6;i++)ciras[i].active=0;for(int i=0;i<N-6;i++)cirmks[i].active=0;init=1;ps("[CIR] Cirsium initialized\n");return 0;}
int cir_planning(int t,int c,int a,int b,int d,int e,int y){return add(cirps,&st.n_cirp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cir_execution(int t,int c,int a,int b,int d,int e,int y){return add(cirss,&st.n_cire,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cir_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cirvss,&st.n_cir2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cir_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ciras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cir_market(int t,int c,int a,int b,int d,int e,int y){return add(cirmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cir_report(void){ps("[CIR] Cirpp: ");pi(st.n_cirp);ps(" PCS=");pi(st.t_f1);ps("\nCire: ");pi(st.n_cire);ps(" PCS=");pi(st.t_f2);ps("\nCirv: ");pi(st.n_cir2);ps(" PCS=");pi(st.t_f3);ps("\nCirc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cir_state(void){ps("[CIR] Cirpp=");pi(st.n_cirp);ps(" Cire=");pi(st.n_cire);ps(" Cirv=");pi(st.n_cir2);ps(" Circ=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cirsium Admin Demo ===\n\n");cir_init();
ps("Cirsium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cir_planning(t,c,1129+(i*17),1118+(i*14),1098+(i*10),1080+(i*6),2020+(i%5));}
ps("\nCirsium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cir_execution(t,c,1118+(i*15),1107+(i*12),1089+(i*8),1076+(i*5),2021+(i%4));}
ps("\nCirsium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cir_evaluation(t,c,1110+(i*13),1099+(i*10),1083+(i*7),1072+(i*4),2022+(i%3));}
ps("\nCirsium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cir_accessory(t,c,1102+(i*11),1093+(i*9),1079+(i*6),1069+(i*3),2023+(i%2));}
ps("\nCirsium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cir_market(t,c,1096+(i*9),1087+(i*7),1074+(i*5),1066+(i*3),2024);}
ps("\n");cir_report();cir_state();ps("\n=== Demo Complete ===\n");return 0;}
