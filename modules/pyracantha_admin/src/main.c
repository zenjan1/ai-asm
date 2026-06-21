/* pyracantha_admin: Pyracantha management technology administration (v1.0)
 * Pyracantha planning, pyracantha execution, pyracantha evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pyx_t;
typedef struct{int n_pyp,n_pye,n_pyv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pyx_state_t;
static pyx_t pyps[N],pyss[N-2],pyvss[N-4],pyas[N-6],pymks[N-6]; static pyx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pyx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pyx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PYX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pyx_init(void){if(init)return -1;st.n_pyp=0;st.n_pye=0;st.n_pyv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pyps[i].active=0;for(int i=0;i<N-2;i++)pyss[i].active=0;for(int i=0;i<N-4;i++)pyvss[i].active=0;for(int i=0;i<N-6;i++)pyas[i].active=0;for(int i=0;i<N-6;i++)pymks[i].active=0;init=1;ps("[PYX] Pyracantha initialized\n");return 0;}
int pyx_planning(int t,int c,int a,int b,int d,int e,int y){return add(pyps,&st.n_pyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pyx_execution(int t,int c,int a,int b,int d,int e,int y){return add(pyss,&st.n_pye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pyx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pyvss,&st.n_pyv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pyx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pyx_market(int t,int c,int a,int b,int d,int e,int y){return add(pymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pyx_report(void){ps("[PYX] Pyp: ");pi(st.n_pyp);ps(" PCS=");pi(st.t_f1);ps("\nPye: ");pi(st.n_pye);ps(" PCS=");pi(st.t_f2);ps("\nPyv: ");pi(st.n_pyv);ps(" PCS=");pi(st.t_f3);ps("\nPyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pyx_state(void){ps("[PYX] Pyp=");pi(st.n_pyp);ps(" Pye=");pi(st.n_pye);ps(" Pyv=");pi(st.n_pyv);ps(" Pyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pyracantha Admin Demo ===\n\n");pyx_init();
ps("Pyracantha planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pyx_planning(t,c,799+(i*17),788+(i*14),768+(i*10),750+(i*6),2020+(i%5));}
ps("\nPyracantha execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pyx_execution(t,c,788+(i*15),777+(i*12),759+(i*8),746+(i*5),2021+(i%4));}
ps("\nPyracantha evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pyx_evaluation(t,c,780+(i*13),769+(i*10),755+(i*7),744+(i*4),2022+(i%3));}
ps("\nPyracantha accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pyx_accessory(t,c,772+(i*11),763+(i*9),749+(i*6),739+(i*3),2023+(i%2));}
ps("\nPyracantha marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pyx_market(t,c,766+(i*9),757+(i*7),744+(i*5),736+(i*3),2024);}
ps("\n");pyx_report();pyx_state();ps("\n=== Demo Complete ===\n");return 0;}
