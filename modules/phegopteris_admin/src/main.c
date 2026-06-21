/* phegopteris_admin: Phegopteris management technology administration (v1.0)
 * Phegopteris planning, phegopteris execution, phegopteris evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} phe_t;
typedef struct{int n_phep,n_phee,n_phe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} phe_state_t;
static phe_t pheps[N],phess[N-2],phevss[N-4],pheas[N-6],phemks[N-6]; static phe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(phe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;phe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PHE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int phe_init(void){if(init)return -1;st.n_phep=0;st.n_phee=0;st.n_phe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pheps[i].active=0;for(int i=0;i<N-2;i++)phess[i].active=0;for(int i=0;i<N-4;i++)phevss[i].active=0;for(int i=0;i<N-6;i++)pheas[i].active=0;for(int i=0;i<N-6;i++)phemks[i].active=0;init=1;ps("[PHE] Phegopteris initialized\n");return 0;}
int phe_planning(int t,int c,int a,int b,int d,int e,int y){return add(pheps,&st.n_phep,&st.t_f1,N,t,c,a,b,d,e,y);}
int phe_execution(int t,int c,int a,int b,int d,int e,int y){return add(phess,&st.n_phee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int phe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(phevss,&st.n_phe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int phe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pheas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int phe_market(int t,int c,int a,int b,int d,int e,int y){return add(phemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void phe_report(void){ps("[PHE] phep: ");pi(st.n_phep);ps(" PCS=");pi(st.t_f1);ps("\nphee: ");pi(st.n_phee);ps(" PCS=");pi(st.t_f2);ps("\nphev: ");pi(st.n_phe2);ps(" PCS=");pi(st.t_f3);ps("\nphec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void phe_state(void){ps("[PHE] phep=");pi(st.n_phep);ps(" phee=");pi(st.n_phee);ps(" phev=");pi(st.n_phe2);ps(" phec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Phegopteris Admin Demo ===\n\n");phe_init();
ps("Phegopteris planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;phe_planning(t,c,1082+(i*17),1071+(i*14),1051+(i*10),1033+(i*6),2020+(i%5));}
ps("\nPhegopteris execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;phe_execution(t,c,1071+(i*15),1060+(i*12),1042+(i*8),1029+(i*5),2021+(i%4));}
ps("\nPhegopteris evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;phe_evaluation(t,c,1063+(i*13),1052+(i*10),1036+(i*7),1025+(i*4),2022+(i%3));}
ps("\nPhegopteris accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phe_accessory(t,c,1055+(i*11),1046+(i*9),1032+(i*6),1022+(i*3),2023+(i%2));}
ps("\nPhegopteris marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phe_market(t,c,1049+(i*9),1040+(i*7),1027+(i*5),1019+(i*3),2024);}
ps("\n");phe_report();phe_state();ps("\n=== Demo Complete ===\n");return 0;}
