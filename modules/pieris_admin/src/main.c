/* pieris_admin: Pieris management technology administration (v1.0)
 * Pieris planning, pieris execution, pieris evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pie_t;
typedef struct{int n_piep,n_piee,n_pie2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pie_state_t;
static pie_t pieps[N],piess[N-2],pievss[N-4],pieas[N-6],piemks[N-6]; static pie_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pie_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pie_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PIE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pie_init(void){if(init)return -1;st.n_piep=0;st.n_piee=0;st.n_pie2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pieps[i].active=0;for(int i=0;i<N-2;i++)piess[i].active=0;for(int i=0;i<N-4;i++)pievss[i].active=0;for(int i=0;i<N-6;i++)pieas[i].active=0;for(int i=0;i<N-6;i++)piemks[i].active=0;init=1;ps("[PIE] Pieris initialized\n");return 0;}
int pie_planning(int t,int c,int a,int b,int d,int e,int y){return add(pieps,&st.n_piep,&st.t_f1,N,t,c,a,b,d,e,y);}
int pie_execution(int t,int c,int a,int b,int d,int e,int y){return add(piess,&st.n_piee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pie_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pievss,&st.n_pie2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pie_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pieas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pie_market(int t,int c,int a,int b,int d,int e,int y){return add(piemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pie_report(void){ps("[PIE] piep: ");pi(st.n_piep);ps(" PCS=");pi(st.t_f1);ps("\npiee: ");pi(st.n_piee);ps(" PCS=");pi(st.t_f2);ps("\npiev: ");pi(st.n_pie2);ps(" PCS=");pi(st.t_f3);ps("\npiec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pie_state(void){ps("[PIE] piep=");pi(st.n_piep);ps(" piee=");pi(st.n_piee);ps(" piev=");pi(st.n_pie2);ps(" piec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pieris Admin Demo ===\n\n");pie_init();
ps("Pieris planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pie_planning(t,c,1084+(i*17),1073+(i*14),1053+(i*10),1035+(i*6),2020+(i%5));}
ps("\nPieris execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pie_execution(t,c,1073+(i*15),1062+(i*12),1044+(i*8),1031+(i*5),2021+(i%4));}
ps("\nPieris evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pie_evaluation(t,c,1065+(i*13),1054+(i*10),1038+(i*7),1027+(i*4),2022+(i%3));}
ps("\nPieris accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pie_accessory(t,c,1057+(i*11),1048+(i*9),1034+(i*6),1024+(i*3),2023+(i%2));}
ps("\nPieris marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pie_market(t,c,1051+(i*9),1042+(i*7),1029+(i*5),1021+(i*3),2024);}
ps("\n");pie_report();pie_state();ps("\n=== Demo Complete ===\n");return 0;}
