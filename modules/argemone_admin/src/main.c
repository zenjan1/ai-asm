/* argemone_admin: Argemone management technology administration (v1.0)
 * Argemone planning, argemone execution, argemone evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} arg_t;
typedef struct{int n_argp,n_arge,n_arg2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} arg_state_t;
static arg_t argps[N],argss[N-2],argvss[N-4],argas[N-6],argmks[N-6]; static arg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(arg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;arg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int arg_init(void){if(init)return -1;st.n_argp=0;st.n_arge=0;st.n_arg2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)argps[i].active=0;for(int i=0;i<N-2;i++)argss[i].active=0;for(int i=0;i<N-4;i++)argvss[i].active=0;for(int i=0;i<N-6;i++)argas[i].active=0;for(int i=0;i<N-6;i++)argmks[i].active=0;init=1;ps("[ARG] Argemone initialized\n");return 0;}
int arg_planning(int t,int c,int a,int b,int d,int e,int y){return add(argps,&st.n_argp,&st.t_f1,N,t,c,a,b,d,e,y);}
int arg_execution(int t,int c,int a,int b,int d,int e,int y){return add(argss,&st.n_arge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int arg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(argvss,&st.n_arg2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int arg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(argas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int arg_market(int t,int c,int a,int b,int d,int e,int y){return add(argmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void arg_report(void){ps("[ARG] Argp: ");pi(st.n_argp);ps(" PCS=");pi(st.t_f1);ps("\nArge: ");pi(st.n_arge);ps(" PCS=");pi(st.t_f2);ps("\nArgv: ");pi(st.n_arg2);ps(" PCS=");pi(st.t_f3);ps("\nArgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void arg_state(void){ps("[ARG] Argp=");pi(st.n_argp);ps(" Arge=");pi(st.n_arge);ps(" Argv=");pi(st.n_arg2);ps(" Argc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Argemone Admin Demo ===\n\n");arg_init();
ps("Argemone planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;arg_planning(t,c,897+(i*17),886+(i*14),866+(i*10),848+(i*6),2020+(i%5));}
ps("\nArgemone execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;arg_execution(t,c,886+(i*15),875+(i*12),857+(i*8),844+(i*5),2021+(i%4));}
ps("\nArgemone evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;arg_evaluation(t,c,878+(i*13),867+(i*10),851+(i*7),840+(i*4),2022+(i%3));}
ps("\nArgemone accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arg_accessory(t,c,870+(i*11),861+(i*9),847+(i*6),837+(i*3),2023+(i%2));}
ps("\nArgemone marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arg_market(t,c,864+(i*9),855+(i*7),842+(i*5),834+(i*3),2024);}
ps("\n");arg_report();arg_state();ps("\n=== Demo Complete ===\n");return 0;}
