/* rumex_admin: Rumex management technology administration (v1.0)
 * Rumex planning, rumex execution, rumex evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rum_t;
typedef struct{int n_rump,n_rume,n_rum2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rum_state_t;
static rum_t rumps[N],rumss[N-2],rumvss[N-4],rumas[N-6],rummks[N-6]; static rum_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rum_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rum_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RUM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rum_init(void){if(init)return -1;st.n_rump=0;st.n_rume=0;st.n_rum2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rumps[i].active=0;for(int i=0;i<N-2;i++)rumss[i].active=0;for(int i=0;i<N-4;i++)rumvss[i].active=0;for(int i=0;i<N-6;i++)rumas[i].active=0;for(int i=0;i<N-6;i++)rummks[i].active=0;init=1;ps("[RUM] Rumex initialized\n");return 0;}
int rum_planning(int t,int c,int a,int b,int d,int e,int y){return add(rumps,&st.n_rump,&st.t_f1,N,t,c,a,b,d,e,y);}
int rum_execution(int t,int c,int a,int b,int d,int e,int y){return add(rumss,&st.n_rume,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rum_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rumvss,&st.n_rum2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rum_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rumas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rum_market(int t,int c,int a,int b,int d,int e,int y){return add(rummks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rum_report(void){ps("[RUM] Rumpp: ");pi(st.n_rump);ps(" PCS=");pi(st.t_f1);ps("\nRume: ");pi(st.n_rume);ps(" PCS=");pi(st.t_f2);ps("\nRumv: ");pi(st.n_rum2);ps(" PCS=");pi(st.t_f3);ps("\nRumc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rum_state(void){ps("[RUM] Rumpp=");pi(st.n_rump);ps(" Rume=");pi(st.n_rume);ps(" Rumv=");pi(st.n_rum2);ps(" Rumc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rumex Admin Demo ===\n\n");rum_init();
ps("Rumex planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rum_planning(t,c,1184+(i*17),1173+(i*14),1153+(i*10),1135+(i*6),2020+(i%5));}
ps("\nRumex execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rum_execution(t,c,1173+(i*15),1162+(i*12),1144+(i*8),1131+(i*5),2021+(i%4));}
ps("\nRumex evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rum_evaluation(t,c,1165+(i*13),1154+(i*10),1138+(i*7),1127+(i*4),2022+(i%3));}
ps("\nRumex accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rum_accessory(t,c,1157+(i*11),1148+(i*9),1134+(i*6),1124+(i*3),2023+(i%2));}
ps("\nRumex marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rum_market(t,c,1151+(i*9),1142+(i*7),1129+(i*5),1121+(i*3),2024);}
ps("\n");rum_report();rum_state();ps("\n=== Demo Complete ===\n");return 0;}
