/* lauranthena_admin: Lauranthena management technology administration (v1.0)
 * Lauranthena planning, lauranthena execution, lauranthena evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lau_t;
typedef struct{int n_laup,n_lau2,n_lauv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lau_state_t;
static lau_t laups[N],lauvss[N-2],lauvss2[N-4],lauas[N-6],laumks[N-6]; static lau_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lau_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lau_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lau_init(void){if(init)return -1;st.n_laup=0;st.n_lau2=0;st.n_lauv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)laups[i].active=0;for(int i=0;i<N-2;i++)lauvss[i].active=0;for(int i=0;i<N-4;i++)lauvss2[i].active=0;for(int i=0;i<N-6;i++)lauas[i].active=0;for(int i=0;i<N-6;i++)laumks[i].active=0;init=1;ps("[LAU] Lauranthena initialized\n");return 0;}
int lau_planning(int t,int c,int a,int b,int d,int e,int y){return add(laups,&st.n_laup,&st.t_f1,N,t,c,a,b,d,e,y);}
int lau_execution(int t,int c,int a,int b,int d,int e,int y){return add(lauvss,&st.n_lau2,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lau_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lauvss2,&st.n_lauv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lau_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lauas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lau_market(int t,int c,int a,int b,int d,int e,int y){return add(laumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lau_report(void){ps("[LAU] Laup: ");pi(st.n_laup);ps(" PCS=");pi(st.t_f1);ps("\nLau2: ");pi(st.n_lau2);ps(" PCS=");pi(st.t_f2);ps("\nLauv: ");pi(st.n_lauv);ps(" PCS=");pi(st.t_f3);ps("\nLauc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lau_state(void){ps("[LAU] Laup=");pi(st.n_laup);ps(" Lau2=");pi(st.n_lau2);ps(" Lauv=");pi(st.n_lauv);ps(" Lauc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lauranthena Admin Demo ===\n\n");lau_init();
ps("Lauranthena planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lau_planning(t,c,1045+(i*17),1034+(i*14),1014+(i*10),996+(i*6),2020+(i%5));}
ps("\nLauranthena execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lau_execution(t,c,1034+(i*15),1023+(i*12),1005+(i*8),992+(i*5),2021+(i%4));}
ps("\nLauranthena evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lau_evaluation(t,c,1026+(i*13),1015+(i*10),999+(i*7),988+(i*4),2022+(i%3));}
ps("\nLauranthena accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lau_accessory(t,c,1018+(i*11),1009+(i*9),995+(i*6),985+(i*3),2023+(i%2));}
ps("\nLauranthena marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lau_market(t,c,1012+(i*9),1003+(i*7),990+(i*5),982+(i*3),2024);}
ps("\n");lau_report();lau_state();ps("\n=== Demo Complete ===\n");return 0;}
