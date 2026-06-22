/* dasiphora_admin: Dasiphora management technology administration (v1.0)
 * Dasiphora planning, dasiphora execution, dasiphora evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dasi_t;
typedef struct{int n_dasip,n_dasi,n_dasi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dasi_state_t;
static dasi_t dasis[N],dasie[N-2],dasi2[N-4],dasiac[N-6],dasiam[N-6]; static dasi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dasi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dasi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DASI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dasi_init(void){if(init)return -1;st.n_dasip=0;st.n_dasi=0;st.n_dasi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dasis[i].active=0;for(int i=0;i<N-2;i++)dasie[i].active=0;for(int i=0;i<N-4;i++)dasi2[i].active=0;for(int i=0;i<N-6;i++)dasiac[i].active=0;for(int i=0;i<N-6;i++)dasiam[i].active=0;init=1;ps("[DASI] Dasiphora initialized\n");return 0;}
int dasi_planning(int t,int c,int a,int b,int d,int e,int y){return add(dasis,&st.n_dasip,&st.t_f1,N,t,c,a,b,d,e,y);}
int dasi_execution(int t,int c,int a,int b,int d,int e,int y){return add(dasie,&st.n_dasi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dasi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dasi2,&st.n_dasi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dasi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dasiac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dasi_market(int t,int c,int a,int b,int d,int e,int y){return add(dasiam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dasi_report(void){ps("[DASI] Sip: ");pi(st.n_dasip);ps(" PCS=");pi(st.t_f1);ps("\nSie: ");pi(st.n_dasi);ps(" PCS=");pi(st.t_f2);ps("\nSi2: ");pi(st.n_dasi2);ps(" PCS=");pi(st.t_f3);ps("\nSac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dasi_state(void){ps("[DASI] Sip=");pi(st.n_dasip);ps(" Si=");pi(st.n_dasi);ps(" Si2=");pi(st.n_dasi2);ps(" Sac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dasiphora Admin Demo ===\n\n");dasi_init();
ps("Dasiphora planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dasi_planning(t,c,1490+(i*17),1479+(i*14),1459+(i*10),1441+(i*6),2020+(i%5));}
ps("\nDasiphora execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dasi_execution(t,c,1479+(i*15),1468+(i*12),1450+(i*8),1437+(i*5),2021+(i%4));}
ps("\nDasiphora evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dasi_evaluation(t,c,1471+(i*13),1460+(i*10),1444+(i*7),1433+(i*4),2022+(i%3));}
ps("\nDasiphora accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dasi_accessory(t,c,1463+(i*11),1454+(i*9),1440+(i*6),1430+(i*3),2023+(i%2));}
ps("\nDasiphora marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dasi_market(t,c,1457+(i*9),1448+(i*7),1435+(i*5),1427+(i*3),2024);}
ps("\n");dasi_report();dasi_state();ps("\n=== Demo Complete ===\n");return 0;}
