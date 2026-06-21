/* acacia_admin: Acacia management technology administration (v1.0)
 * Acacia planning, acacia execution, acacia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aca_t;
typedef struct{int n_acap,n_acae,n_aca2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aca_state_t;
static aca_t acaps[N],acass[N-2],acavss[N-4],acaas[N-6],acamks[N-6]; static aca_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aca_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aca_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aca_init(void){if(init)return -1;st.n_acap=0;st.n_acae=0;st.n_aca2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acaps[i].active=0;for(int i=0;i<N-2;i++)acass[i].active=0;for(int i=0;i<N-4;i++)acavss[i].active=0;for(int i=0;i<N-6;i++)acaas[i].active=0;for(int i=0;i<N-6;i++)acamks[i].active=0;init=1;ps("[ACA] Acacia initialized\n");return 0;}
int aca_planning(int t,int c,int a,int b,int d,int e,int y){return add(acaps,&st.n_acap,&st.t_f1,N,t,c,a,b,d,e,y);}
int aca_execution(int t,int c,int a,int b,int d,int e,int y){return add(acass,&st.n_acae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aca_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(acavss,&st.n_aca2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aca_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aca_market(int t,int c,int a,int b,int d,int e,int y){return add(acamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aca_report(void){ps("[ACA] Acap: ");pi(st.n_acap);ps(" PCS=");pi(st.t_f1);ps("\nAcae: ");pi(st.n_acae);ps(" PCS=");pi(st.t_f2);ps("\nAcav: ");pi(st.n_aca2);ps(" PCS=");pi(st.t_f3);ps("\nAcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aca_state(void){ps("[ACA] Acap=");pi(st.n_acap);ps(" Acae=");pi(st.n_acae);ps(" Acav=");pi(st.n_aca2);ps(" Acc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Acacia Admin Demo ===\n\n");aca_init();
ps("Acacia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aca_planning(t,c,890+(i*17),879+(i*14),859+(i*10),841+(i*6),2020+(i%5));}
ps("\nAcacia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aca_execution(t,c,879+(i*15),868+(i*12),850+(i*8),837+(i*5),2021+(i%4));}
ps("\nAcacia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aca_evaluation(t,c,871+(i*13),860+(i*10),844+(i*7),833+(i*4),2022+(i%3));}
ps("\nAcacia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aca_accessory(t,c,863+(i*11),854+(i*9),840+(i*6),830+(i*3),2023+(i%2));}
ps("\nAcacia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aca_market(t,c,857+(i*9),848+(i*7),835+(i*5),827+(i*3),2024);}
ps("\n");aca_report();aca_state();ps("\n=== Demo Complete ===\n");return 0;}
