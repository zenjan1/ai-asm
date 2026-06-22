/* reaumuria_admin: Reaumuria management technology administration (v1.0)
 * Reaumuria planning, reaumuria execution, reaumuria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rea_t;
typedef struct{int n_reap,n_reae,n_rea2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rea_state_t;
static rea_t reaps[N],reass[N-2],reavss[N-4],reaas[N-6],reamks[N-6]; static rea_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rea_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rea_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[REA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rea_init(void){if(init)return -1;st.n_reap=0;st.n_reae=0;st.n_rea2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)reaps[i].active=0;for(int i=0;i<N-2;i++)reass[i].active=0;for(int i=0;i<N-4;i++)reavss[i].active=0;for(int i=0;i<N-6;i++)reaas[i].active=0;for(int i=0;i<N-6;i++)reamks[i].active=0;init=1;ps("[REA] Reaumuria initialized\n");return 0;}
int rea_planning(int t,int c,int a,int b,int d,int e,int y){return add(reaps,&st.n_reap,&st.t_f1,N,t,c,a,b,d,e,y);}
int rea_execution(int t,int c,int a,int b,int d,int e,int y){return add(reass,&st.n_reae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rea_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(reavss,&st.n_rea2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rea_accessory(int t,int c,int a,int b,int d,int e,int y){return add(reaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rea_market(int t,int c,int a,int b,int d,int e,int y){return add(reamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rea_report(void){ps("[REA] Reapp: ");pi(st.n_reap);ps(" PCS=");pi(st.t_f1);ps("\nReae: ");pi(st.n_reae);ps(" PCS=");pi(st.t_f2);ps("\nReav: ");pi(st.n_rea2);ps(" PCS=");pi(st.t_f3);ps("\nReac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rea_state(void){ps("[REA] Reapp=");pi(st.n_reap);ps(" Reae=");pi(st.n_reae);ps(" Reav=");pi(st.n_rea2);ps(" Reac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Reaumuria Admin Demo ===\n\n");rea_init();
ps("Reaumuria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rea_planning(t,c,1177+(i*17),1166+(i*14),1146+(i*10),1128+(i*6),2020+(i%5));}
ps("\nReaumuria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rea_execution(t,c,1166+(i*15),1155+(i*12),1137+(i*8),1124+(i*5),2021+(i%4));}
ps("\nReaumuria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rea_evaluation(t,c,1158+(i*13),1147+(i*10),1131+(i*7),1120+(i*4),2022+(i%3));}
ps("\nReaumuria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rea_accessory(t,c,1150+(i*11),1141+(i*9),1127+(i*6),1117+(i*3),2023+(i%2));}
ps("\nReaumuria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rea_market(t,c,1144+(i*9),1135+(i*7),1122+(i*5),1114+(i*3),2024);}
ps("\n");rea_report();rea_state();ps("\n=== Demo Complete ===\n");return 0;}
