/* pygeum_admin: Pygeum management technology administration (v1.0)
 * Pygeum planning, pygeum execution, pygeum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pyg_t;
typedef struct{int n_pygp,n_pyge,n_pyg2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pyg_state_t;
static pyg_t pygps[N],pygss[N-2],pygvss[N-4],pygas[N-6],pygmks[N-6]; static pyg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pyg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pyg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PYG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pyg_init(void){if(init)return -1;st.n_pygp=0;st.n_pyge=0;st.n_pyg2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pygps[i].active=0;for(int i=0;i<N-2;i++)pygss[i].active=0;for(int i=0;i<N-4;i++)pygvss[i].active=0;for(int i=0;i<N-6;i++)pygas[i].active=0;for(int i=0;i<N-6;i++)pygmks[i].active=0;init=1;ps("[PYG] Pygeum initialized\n");return 0;}
int pyg_planning(int t,int c,int a,int b,int d,int e,int y){return add(pygps,&st.n_pygp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pyg_execution(int t,int c,int a,int b,int d,int e,int y){return add(pygss,&st.n_pyge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pyg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pygvss,&st.n_pyg2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pyg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pygas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pyg_market(int t,int c,int a,int b,int d,int e,int y){return add(pygmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pyg_report(void){ps("[PYG] pygp: ");pi(st.n_pygp);ps(" PCS=");pi(st.t_f1);ps("\npyge: ");pi(st.n_pyge);ps(" PCS=");pi(st.t_f2);ps("\npygv: ");pi(st.n_pyg2);ps(" PCS=");pi(st.t_f3);ps("\npygc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pyg_state(void){ps("[PYG] pygp=");pi(st.n_pygp);ps(" pyge=");pi(st.n_pyge);ps(" pygv=");pi(st.n_pyg2);ps(" pygc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pygeum Admin Demo ===\n\n");pyg_init();
ps("Pygeum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pyg_planning(t,c,1091+(i*17),1080+(i*14),1060+(i*10),1042+(i*6),2020+(i%5));}
ps("\nPygeum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pyg_execution(t,c,1080+(i*15),1069+(i*12),1051+(i*8),1038+(i*5),2021+(i%4));}
ps("\nPygeum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pyg_evaluation(t,c,1072+(i*13),1061+(i*10),1045+(i*7),1034+(i*4),2022+(i%3));}
ps("\nPygeum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pyg_accessory(t,c,1064+(i*11),1055+(i*9),1041+(i*6),1031+(i*3),2023+(i%2));}
ps("\nPygeum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pyg_market(t,c,1058+(i*9),1049+(i*7),1036+(i*5),1028+(i*3),2024);}
ps("\n");pyg_report();pyg_state();ps("\n=== Demo Complete ===\n");return 0;}
