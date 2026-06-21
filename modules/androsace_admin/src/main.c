/* androsace_admin: Androsace management technology administration (v1.0)
 * Androsace planning, androsace execution, androsace evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} and_t;
typedef struct{int n_andp,n_ande,n_and2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} and_state_t;
static and_t andps[N],andss[N-2],andvss[N-4],andas[N-6],andmks[N-6]; static and_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(and_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;and_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AND] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int and_init(void){if(init)return -1;st.n_andp=0;st.n_ande=0;st.n_and2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)andps[i].active=0;for(int i=0;i<N-2;i++)andss[i].active=0;for(int i=0;i<N-4;i++)andvss[i].active=0;for(int i=0;i<N-6;i++)andas[i].active=0;for(int i=0;i<N-6;i++)andmks[i].active=0;init=1;ps("[AND] Androsace initialized\n");return 0;}
int and_planning(int t,int c,int a,int b,int d,int e,int y){return add(andps,&st.n_andp,&st.t_f1,N,t,c,a,b,d,e,y);}
int and_execution(int t,int c,int a,int b,int d,int e,int y){return add(andss,&st.n_ande,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int and_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(andvss,&st.n_and2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int and_accessory(int t,int c,int a,int b,int d,int e,int y){return add(andas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int and_market(int t,int c,int a,int b,int d,int e,int y){return add(andmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void and_report(void){ps("[AND] Andp: ");pi(st.n_andp);ps(" PCS=");pi(st.t_f1);ps("\nAnde: ");pi(st.n_ande);ps(" PCS=");pi(st.t_f2);ps("\nAndv: ");pi(st.n_and2);ps(" PCS=");pi(st.t_f3);ps("\nAndc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void and_state(void){ps("[AND] Andp=");pi(st.n_andp);ps(" Ande=");pi(st.n_ande);ps(" Andv=");pi(st.n_and2);ps(" Andc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Androsace Admin Demo ===\n\n");and_init();
ps("Androsace planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;and_planning(t,c,896+(i*17),885+(i*14),865+(i*10),847+(i*6),2020+(i%5));}
ps("\nAndrosace execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;and_execution(t,c,885+(i*15),874+(i*12),856+(i*8),843+(i*5),2021+(i%4));}
ps("\nAndrosace evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;and_evaluation(t,c,877+(i*13),866+(i*10),850+(i*7),839+(i*4),2022+(i%3));}
ps("\nAndrosace accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;and_accessory(t,c,869+(i*11),860+(i*9),846+(i*6),836+(i*3),2023+(i%2));}
ps("\nAndrosace marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;and_market(t,c,863+(i*9),854+(i*7),841+(i*5),833+(i*3),2024);}
ps("\n");and_report();and_state();ps("\n=== Demo Complete ===\n");return 0;}
