/* empetrum_admin: Empetrum management technology administration (v1.0)
 * Empetrum planning, empetrum execution, empetrum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} empe_t;
typedef struct{int n_empep,n_empe,n_empe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} empe_state_t;
static empe_t empes[N],empee[N-2],empe2[N-4],empeac[N-6],empeam[N-6]; static empe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(empe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;empe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EMPE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int empe_init(void){if(init)return -1;st.n_empep=0;st.n_empe=0;st.n_empe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)empes[i].active=0;for(int i=0;i<N-2;i++)empee[i].active=0;for(int i=0;i<N-4;i++)empe2[i].active=0;for(int i=0;i<N-6;i++)empeac[i].active=0;for(int i=0;i<N-6;i++)empeam[i].active=0;init=1;ps("[EMPE] Empetrum initialized\n");return 0;}
int empe_planning(int t,int c,int a,int b,int d,int e,int y){return add(empes,&st.n_empep,&st.t_f1,N,t,c,a,b,d,e,y);}
int empe_execution(int t,int c,int a,int b,int d,int e,int y){return add(empee,&st.n_empe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int empe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(empe2,&st.n_empe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int empe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(empeac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int empe_market(int t,int c,int a,int b,int d,int e,int y){return add(empeam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void empe_report(void){ps("[EMPE] Pep: ");pi(st.n_empep);ps(" PCS=");pi(st.t_f1);ps("\nPee: ");pi(st.n_empe);ps(" PCS=");pi(st.t_f2);ps("\nPe2: ");pi(st.n_empe2);ps(" PCS=");pi(st.t_f3);ps("\nPac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void empe_state(void){ps("[EMPE] Pep=");pi(st.n_empep);ps(" Pe=");pi(st.n_empe);ps(" Pe2=");pi(st.n_empe2);ps(" Pac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Empetrum Admin Demo ===\n\n");empe_init();
ps("Empetrum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;empe_planning(t,c,1505+(i*17),1494+(i*14),1474+(i*10),1456+(i*6),2020+(i%5));}
ps("\nEmpetrum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;empe_execution(t,c,1494+(i*15),1483+(i*12),1465+(i*8),1452+(i*5),2021+(i%4));}
ps("\nEmpetrum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;empe_evaluation(t,c,1486+(i*13),1475+(i*10),1459+(i*7),1448+(i*4),2022+(i%3));}
ps("\nEmpetrum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;empe_accessory(t,c,1478+(i*11),1469+(i*9),1455+(i*6),1445+(i*3),2023+(i%2));}
ps("\nEmpetrum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;empe_market(t,c,1472+(i*9),1463+(i*7),1450+(i*5),1442+(i*3),2024);}
ps("\n");empe_report();empe_state();ps("\n=== Demo Complete ===\n");return 0;}
