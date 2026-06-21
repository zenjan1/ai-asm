/* gorteria_admin: Gorteria management technology administration (v1.0)
 * Gorteria planning, gorteria execution, gorteria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gor_t;
typedef struct{int n_gorp,n_gore,n_gor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gor_state_t;
static gor_t gorps[N],gorss[N-2],gorvss[N-4],goras[N-6],gormks[N-6]; static gor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gor_init(void){if(init)return -1;st.n_gorp=0;st.n_gore=0;st.n_gor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gorps[i].active=0;for(int i=0;i<N-2;i++)gorss[i].active=0;for(int i=0;i<N-4;i++)gorvss[i].active=0;for(int i=0;i<N-6;i++)goras[i].active=0;for(int i=0;i<N-6;i++)gormks[i].active=0;init=1;ps("[GOR] Gorteria initialized\n");return 0;}
int gor_planning(int t,int c,int a,int b,int d,int e,int y){return add(gorps,&st.n_gorp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gor_execution(int t,int c,int a,int b,int d,int e,int y){return add(gorss,&st.n_gore,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gorvss,&st.n_gor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(goras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gor_market(int t,int c,int a,int b,int d,int e,int y){return add(gormks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gor_report(void){ps("[GOR] Gorrp: ");pi(st.n_gorp);ps(" PCS=");pi(st.t_f1);ps("\nGore: ");pi(st.n_gore);ps(" PCS=");pi(st.t_f2);ps("\nGorv: ");pi(st.n_gor2);ps(" PCS=");pi(st.t_f3);ps("\nGorc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gor_state(void){ps("[GOR] Gorrp=");pi(st.n_gorp);ps(" Gore=");pi(st.n_gore);ps(" Gorv=");pi(st.n_gor2);ps(" Gorc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gorteria Admin Demo ===\n\n");gor_init();
ps("Gorteria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gor_planning(t,c,1014+(i*17),1003+(i*14),983+(i*10),965+(i*6),2020+(i%5));}
ps("\nGorteria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gor_execution(t,c,1003+(i*15),992+(i*12),974+(i*8),961+(i*5),2021+(i%4));}
ps("\nGorteria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gor_evaluation(t,c,995+(i*13),984+(i*10),968+(i*7),957+(i*4),2022+(i%3));}
ps("\nGorteria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gor_accessory(t,c,987+(i*11),978+(i*9),964+(i*6),954+(i*3),2023+(i%2));}
ps("\nGorteria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gor_market(t,c,981+(i*9),972+(i*7),959+(i*5),951+(i*3),2024);}
ps("\n");gor_report();gor_state();ps("\n=== Demo Complete ===\n");return 0;}
