/* cucumis_admin: Cucumis management technology administration (v1.0)
 * Cucumis planning, cucumis execution, cucumis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cuc_t;
typedef struct{int n_cucp,n_cuce,n_cuc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cuc_state_t;
static cuc_t cucps[N],cucss[N-2],cucvss[N-4],cucas[N-6],cucmks[N-6]; static cuc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cuc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cuc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CUC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cuc_init(void){if(init)return -1;st.n_cucp=0;st.n_cuce=0;st.n_cuc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cucps[i].active=0;for(int i=0;i<N-2;i++)cucss[i].active=0;for(int i=0;i<N-4;i++)cucvss[i].active=0;for(int i=0;i<N-6;i++)cucas[i].active=0;for(int i=0;i<N-6;i++)cucmks[i].active=0;init=1;ps("[CUC] Cucumis initialized\n");return 0;}
int cuc_planning(int t,int c,int a,int b,int d,int e,int y){return add(cucps,&st.n_cucp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cuc_execution(int t,int c,int a,int b,int d,int e,int y){return add(cucss,&st.n_cuce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cuc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cucvss,&st.n_cuc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cuc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cucas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cuc_market(int t,int c,int a,int b,int d,int e,int y){return add(cucmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cuc_report(void){ps("[CUC] Cucpp: ");pi(st.n_cucp);ps(" PCS=");pi(st.t_f1);ps("\nCuce: ");pi(st.n_cuce);ps(" PCS=");pi(st.t_f2);ps("\nCucv: ");pi(st.n_cuc2);ps(" PCS=");pi(st.t_f3);ps("\nCucc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cuc_state(void){ps("[CUC] Cucpp=");pi(st.n_cucp);ps(" Cuce=");pi(st.n_cuce);ps(" Cucv=");pi(st.n_cuc2);ps(" Cucc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cucumis Admin Demo ===\n\n");cuc_init();
ps("Cucumis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cuc_planning(t,c,1134+(i*17),1123+(i*14),1103+(i*10),1085+(i*6),2020+(i%5));}
ps("\nCucumis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cuc_execution(t,c,1123+(i*15),1112+(i*12),1094+(i*8),1081+(i*5),2021+(i%4));}
ps("\nCucumis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cuc_evaluation(t,c,1115+(i*13),1104+(i*10),1088+(i*7),1077+(i*4),2022+(i%3));}
ps("\nCucumis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cuc_accessory(t,c,1107+(i*11),1098+(i*9),1084+(i*6),1074+(i*3),2023+(i%2));}
ps("\nCucumis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cuc_market(t,c,1101+(i*9),1092+(i*7),1079+(i*5),1071+(i*3),2024);}
ps("\n");cuc_report();cuc_state();ps("\n=== Demo Complete ===\n");return 0;}
