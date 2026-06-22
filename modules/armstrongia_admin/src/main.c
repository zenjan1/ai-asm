/* armstrongia_admin: Armstrongia management technology administration (v1.0)
 * Armstrongia planning, armstrongia execution, armstrongia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} arms_t;
typedef struct{int n_armsp,n_arms,n_arms2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} arms_state_t;
static arms_t armss[N],armses[N-2],arms2[N-4],armsac[N-6],armsam[N-6]; static arms_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(arms_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;arms_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARMS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int arms_init(void){if(init)return -1;st.n_armsp=0;st.n_arms=0;st.n_arms2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)armss[i].active=0;for(int i=0;i<N-2;i++)armses[i].active=0;for(int i=0;i<N-4;i++)arms2[i].active=0;for(int i=0;i<N-6;i++)armsac[i].active=0;for(int i=0;i<N-6;i++)armsam[i].active=0;init=1;ps("[ARMS] Armstrongia initialized\n");return 0;}
int arms_planning(int t,int c,int a,int b,int d,int e,int y){return add(armss,&st.n_armsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int arms_execution(int t,int c,int a,int b,int d,int e,int y){return add(armses,&st.n_arms,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int arms_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(arms2,&st.n_arms2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int arms_accessory(int t,int c,int a,int b,int d,int e,int y){return add(armsac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int arms_market(int t,int c,int a,int b,int d,int e,int y){return add(armsam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void arms_report(void){ps("[ARMS] Msp: ");pi(st.n_armsp);ps(" PCS=");pi(st.t_f1);ps("\nMses: ");pi(st.n_arms);ps(" PCS=");pi(st.t_f2);ps("\nMs2: ");pi(st.n_arms2);ps(" PCS=");pi(st.t_f3);ps("\nMsac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void arms_state(void){ps("[ARMS] Msp=");pi(st.n_armsp);ps(" Mses=");pi(st.n_arms);ps(" Ms2=");pi(st.n_arms2);ps(" Msac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Armstrongia Admin Demo ===\n\n");arms_init();
ps("Armstrongia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;arms_planning(t,c,1415+(i*17),1404+(i*14),1384+(i*10),1366+(i*6),2020+(i%5));}
ps("\nArmstrongia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;arms_execution(t,c,1404+(i*15),1393+(i*12),1375+(i*8),1362+(i*5),2021+(i%4));}
ps("\nArmstrongia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;arms_evaluation(t,c,1396+(i*13),1385+(i*10),1369+(i*7),1358+(i*4),2022+(i%3));}
ps("\nArmstrongia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arms_accessory(t,c,1388+(i*11),1379+(i*9),1365+(i*6),1355+(i*3),2023+(i%2));}
ps("\nArmstrongia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arms_market(t,c,1382+(i*9),1373+(i*7),1360+(i*5),1352+(i*3),2024);}
ps("\n");arms_report();arms_state();ps("\n=== Demo Complete ===\n");return 0;}
