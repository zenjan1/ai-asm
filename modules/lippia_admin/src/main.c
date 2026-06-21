/* lippia_admin: Lippia management technology administration (v1.0)
 * Lippia planning, lippia execution, lippia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lip_t;
typedef struct{int n_lipp,n_lipe,n_lip2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lip_state_t;
static lip_t lipps[N],lipss[N-2],lipvss[N-4],lipas[N-6],lipmks[N-6]; static lip_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lip_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lip_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LIP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lip_init(void){if(init)return -1;st.n_lipp=0;st.n_lipe=0;st.n_lip2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lipps[i].active=0;for(int i=0;i<N-2;i++)lipss[i].active=0;for(int i=0;i<N-4;i++)lipvss[i].active=0;for(int i=0;i<N-6;i++)lipas[i].active=0;for(int i=0;i<N-6;i++)lipmks[i].active=0;init=1;ps("[LIP] Lippia initialized\n");return 0;}
int lip_planning(int t,int c,int a,int b,int d,int e,int y){return add(lipps,&st.n_lipp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lip_execution(int t,int c,int a,int b,int d,int e,int y){return add(lipss,&st.n_lipe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lip_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lipvss,&st.n_lip2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lip_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lipas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lip_market(int t,int c,int a,int b,int d,int e,int y){return add(lipmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lip_report(void){ps("[LIP] Lipp: ");pi(st.n_lipp);ps(" PCS=");pi(st.t_f1);ps("\nLipe: ");pi(st.n_lipe);ps(" PCS=");pi(st.t_f2);ps("\nLipv: ");pi(st.n_lip2);ps(" PCS=");pi(st.t_f3);ps("\nLipc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lip_state(void){ps("[LIP] Lipp=");pi(st.n_lipp);ps(" Lipe=");pi(st.n_lipe);ps(" Lipv=");pi(st.n_lip2);ps(" Lipc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lippia Admin Demo ===\n\n");lip_init();
ps("Lippia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lip_planning(t,c,1055+(i*17),1044+(i*14),1024+(i*10),1006+(i*6),2020+(i%5));}
ps("\nLippia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lip_execution(t,c,1044+(i*15),1033+(i*12),1015+(i*8),1002+(i*5),2021+(i%4));}
ps("\nLippia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lip_evaluation(t,c,1036+(i*13),1025+(i*10),1009+(i*7),998+(i*4),2022+(i%3));}
ps("\nLippia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lip_accessory(t,c,1028+(i*11),1019+(i*9),1005+(i*6),995+(i*3),2023+(i%2));}
ps("\nLippia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lip_market(t,c,1022+(i*9),1013+(i*7),1000+(i*5),992+(i*3),2024);}
ps("\n");lip_report();lip_state();ps("\n=== Demo Complete ===\n");return 0;}
