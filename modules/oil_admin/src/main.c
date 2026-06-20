/* oil_admin: Oil management technology administration (v1.0)
 * Oil planning, oil execution, oil evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oil_t;
typedef struct{int n_op,n_oe,n_ov,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oil_state_t;
static oil_t ops[N],oes[N-2],ovs[N-4],acs[N-6],mks[N-6]; static oil_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oil_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oil_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OIL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oil_init(void){if(init)return -1;st.n_op=0;st.n_oe=0;st.n_ov=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ops[i].active=0;for(int i=0;i<N-2;i++)oes[i].active=0;for(int i=0;i<N-4;i++)ovs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[OIL] Oil initialized\n");return 0;}
int oil_planning(int t,int c,int a,int b,int d,int e,int y){return add(ops,&st.n_op,&st.t_f1,N,t,c,a,b,d,e,y);}
int oil_execution(int t,int c,int a,int b,int d,int e,int y){return add(oes,&st.n_oe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oil_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ovs,&st.n_ov,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oil_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oil_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oil_report(void){ps("[OIL] Op: ");pi(st.n_op);ps(" PCS=");pi(st.t_f1);ps("\nOe: ");pi(st.n_oe);ps(" PCS=");pi(st.t_f2);ps("\nOv: ");pi(st.n_ov);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oil_state(void){ps("[OIL] Op=");pi(st.n_op);ps(" Oe=");pi(st.n_oe);ps(" Ov=");pi(st.n_ov);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oil Admin Demo ===\n\n");oil_init();
ps("Oil planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oil_planning(t,c,523+(i*17),512+(i*14),492+(i*10),474+(i*6),2020+(i%5));}
ps("\nOil execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oil_execution(t,c,512+(i*15),501+(i*12),483+(i*8),470+(i*5),2021+(i%4));}
ps("\nOil evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oil_evaluation(t,c,504+(i*13),493+(i*10),477+(i*7),466+(i*4),2022+(i%3));}
ps("\nOil accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oil_accessory(t,c,496+(i*11),487+(i*9),473+(i*6),463+(i*3),2023+(i%2));}
ps("\nOil marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oil_market(t,c,490+(i*9),481+(i*7),468+(i*5),460+(i*3),2024);}
ps("\n");oil_report();oil_state();ps("\n=== Demo Complete ===\n");return 0;}
