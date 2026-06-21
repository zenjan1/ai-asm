/* lisianthus_admin: Lisianthus management technology administration (v1.0)
 * Lisianthus planning, lisianthus execution, lisianthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lis_t;
typedef struct{int n_lisp,n_lise,n_lis2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lis_state_t;
static lis_t lisps[N],liss[N-2],lisvss[N-4],lisas[N-6],lismks[N-6]; static lis_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lis_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lis_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LIS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lis_init(void){if(init)return -1;st.n_lisp=0;st.n_lise=0;st.n_lis2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lisps[i].active=0;for(int i=0;i<N-2;i++)liss[i].active=0;for(int i=0;i<N-4;i++)lisvss[i].active=0;for(int i=0;i<N-6;i++)lisas[i].active=0;for(int i=0;i<N-6;i++)lismks[i].active=0;init=1;ps("[LIS] Lisianthus initialized\n");return 0;}
int lis_planning(int t,int c,int a,int b,int d,int e,int y){return add(lisps,&st.n_lisp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lis_execution(int t,int c,int a,int b,int d,int e,int y){return add(liss,&st.n_lise,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lis_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lisvss,&st.n_lis2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lis_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lisas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lis_market(int t,int c,int a,int b,int d,int e,int y){return add(lismks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lis_report(void){ps("[LIS] LISP: ");pi(st.n_lisp);ps(" PCS=");pi(st.t_f1);ps("\nLise: ");pi(st.n_lise);ps(" PCS=");pi(st.t_f2);ps("\nLisv: ");pi(st.n_lis2);ps(" PCS=");pi(st.t_f3);ps("\nLisc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lis_state(void){ps("[LIS] LISP=");pi(st.n_lisp);ps(" Lise=");pi(st.n_lise);ps(" Lisv=");pi(st.n_lis2);ps(" Lisc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lisianthus Admin Demo ===\n\n");lis_init();
ps("Lisianthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lis_planning(t,c,1058+(i*17),1047+(i*14),1027+(i*10),1009+(i*6),2020+(i%5));}
ps("\nLisianthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lis_execution(t,c,1047+(i*15),1036+(i*12),1018+(i*8),1005+(i*5),2021+(i%4));}
ps("\nLisianthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lis_evaluation(t,c,1039+(i*13),1028+(i*10),1012+(i*7),1001+(i*4),2022+(i%3));}
ps("\nLisianthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lis_accessory(t,c,1031+(i*11),1022+(i*9),1008+(i*6),998+(i*3),2023+(i%2));}
ps("\nLisianthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lis_market(t,c,1025+(i*9),1016+(i*7),1003+(i*5),995+(i*3),2024);}
ps("\n");lis_report();lis_state();ps("\n=== Demo Complete ===\n");return 0;}
