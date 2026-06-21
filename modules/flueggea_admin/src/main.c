/* flueggea_admin: Flueggea management technology administration (v1.0)
 * Flueggea planning, flueggea execution, flueggea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} flu_t;
typedef struct{int n_flup,n_flue,n_flu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} flu_state_t;
static flu_t flups[N],fluss[N-2],fluvss[N-4],fluas[N-6],flumks[N-6]; static flu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(flu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;flu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FLU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int flu_init(void){if(init)return -1;st.n_flup=0;st.n_flue=0;st.n_flu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)flups[i].active=0;for(int i=0;i<N-2;i++)fluss[i].active=0;for(int i=0;i<N-4;i++)fluvss[i].active=0;for(int i=0;i<N-6;i++)fluas[i].active=0;for(int i=0;i<N-6;i++)flumks[i].active=0;init=1;ps("[FLU] Flueggea initialized\n");return 0;}
int flu_planning(int t,int c,int a,int b,int d,int e,int y){return add(flups,&st.n_flup,&st.t_f1,N,t,c,a,b,d,e,y);}
int flu_execution(int t,int c,int a,int b,int d,int e,int y){return add(fluss,&st.n_flue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int flu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fluvss,&st.n_flu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int flu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fluas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int flu_market(int t,int c,int a,int b,int d,int e,int y){return add(flumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void flu_report(void){ps("[FLU] Flupp: ");pi(st.n_flup);ps(" PCS=");pi(st.t_f1);ps("\nFlue: ");pi(st.n_flue);ps(" PCS=");pi(st.t_f2);ps("\nFluv: ");pi(st.n_flu2);ps(" PCS=");pi(st.t_f3);ps("\nFluc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void flu_state(void){ps("[FLU] Flupp=");pi(st.n_flup);ps(" Flue=");pi(st.n_flue);ps(" Fluv=");pi(st.n_flu2);ps(" Fluc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Flueggea Admin Demo ===\n\n");flu_init();
ps("Flueggea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;flu_planning(t,c,1003+(i*17),992+(i*14),972+(i*10),954+(i*6),2020+(i%5));}
ps("\nFlueggea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;flu_execution(t,c,992+(i*15),981+(i*12),963+(i*8),950+(i*5),2021+(i%4));}
ps("\nFlueggea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;flu_evaluation(t,c,984+(i*13),973+(i*10),957+(i*7),946+(i*4),2022+(i%3));}
ps("\nFlueggea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flu_accessory(t,c,976+(i*11),967+(i*9),953+(i*6),943+(i*3),2023+(i%2));}
ps("\nFlueggea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flu_market(t,c,970+(i*9),961+(i*7),948+(i*5),940+(i*3),2024);}
ps("\n");flu_report();flu_state();ps("\n=== Demo Complete ===\n");return 0;}
