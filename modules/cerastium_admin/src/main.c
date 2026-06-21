/* cerastium_admin: Cerastium management technology administration (v1.0)
 * Cerastium planning, cerastium execution, cerastium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cer_t;
typedef struct{int n_cerp,n_cere,n_cer2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cer_state_t;
static cer_t cerps[N],cerss[N-2],cervss[N-4],ceras[N-6],cermks[N-6]; static cer_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cer_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cer_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cer_init(void){if(init)return -1;st.n_cerp=0;st.n_cere=0;st.n_cer2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cerps[i].active=0;for(int i=0;i<N-2;i++)cerss[i].active=0;for(int i=0;i<N-4;i++)cervss[i].active=0;for(int i=0;i<N-6;i++)ceras[i].active=0;for(int i=0;i<N-6;i++)cermks[i].active=0;init=1;ps("[CER] Cerastium initialized\n");return 0;}
int cer_planning(int t,int c,int a,int b,int d,int e,int y){return add(cerps,&st.n_cerp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cer_execution(int t,int c,int a,int b,int d,int e,int y){return add(cerss,&st.n_cere,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cer_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cervss,&st.n_cer2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cer_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ceras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cer_market(int t,int c,int a,int b,int d,int e,int y){return add(cermks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cer_report(void){ps("[CER] Cerp: ");pi(st.n_cerp);ps(" PCS=");pi(st.t_f1);ps("\nCere: ");pi(st.n_cere);ps(" PCS=");pi(st.t_f2);ps("\nCerv: ");pi(st.n_cer2);ps(" PCS=");pi(st.t_f3);ps("\nCerc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cer_state(void){ps("[CER] Cerp=");pi(st.n_cerp);ps(" Cere=");pi(st.n_cere);ps(" Cerv=");pi(st.n_cer2);ps(" Cerc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cerastium Admin Demo ===\n\n");cer_init();
ps("Cerastium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cer_planning(t,c,906+(i*17),895+(i*14),875+(i*10),857+(i*6),2020+(i%5));}
ps("\nCerastium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cer_execution(t,c,895+(i*15),884+(i*12),866+(i*8),853+(i*5),2021+(i%4));}
ps("\nCerastium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cer_evaluation(t,c,887+(i*13),876+(i*10),860+(i*7),849+(i*4),2022+(i%3));}
ps("\nCerastium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cer_accessory(t,c,879+(i*11),870+(i*9),856+(i*6),846+(i*3),2023+(i%2));}
ps("\nCerastium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cer_market(t,c,873+(i*9),864+(i*7),851+(i*5),843+(i*3),2024);}
ps("\n");cer_report();cer_state();ps("\n=== Demo Complete ===\n");return 0;}
