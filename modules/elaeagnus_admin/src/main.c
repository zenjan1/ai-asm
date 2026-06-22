/* elaeagnus_admin: Elaeagnus management technology administration (v1.0)
 * Elaeagnus planning, elaeagnus execution, elaeagnus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ela_t;
typedef struct{int n_elap,n_elae,n_ela2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ela_state_t;
static ela_t elaps[N],elass[N-2],elavss[N-4],elaas[N-6],elamks[N-6]; static ela_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ela_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ela_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ELA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ela_init(void){if(init)return -1;st.n_elap=0;st.n_elae=0;st.n_ela2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)elaps[i].active=0;for(int i=0;i<N-2;i++)elass[i].active=0;for(int i=0;i<N-4;i++)elavss[i].active=0;for(int i=0;i<N-6;i++)elaas[i].active=0;for(int i=0;i<N-6;i++)elamks[i].active=0;init=1;ps("[ELA] Elaeagnus initialized\n");return 0;}
int ela_planning(int t,int c,int a,int b,int d,int e,int y){return add(elaps,&st.n_elap,&st.t_f1,N,t,c,a,b,d,e,y);}
int ela_execution(int t,int c,int a,int b,int d,int e,int y){return add(elass,&st.n_elae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ela_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(elavss,&st.n_ela2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ela_accessory(int t,int c,int a,int b,int d,int e,int y){return add(elaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ela_market(int t,int c,int a,int b,int d,int e,int y){return add(elamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ela_report(void){ps("[ELA] Elap: ");pi(st.n_elap);ps(" PCS=");pi(st.t_f1);ps("\nElae: ");pi(st.n_elae);ps(" PCS=");pi(st.t_f2);ps("\nElav: ");pi(st.n_ela2);ps(" PCS=");pi(st.t_f3);ps("\nElac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ela_state(void){ps("[ELA] Elap=");pi(st.n_elap);ps(" Elae=");pi(st.n_elae);ps(" Elav=");pi(st.n_ela2);ps(" Elac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Elaeagnus Admin Demo ===\n\n");ela_init();
ps("Elaeagnus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ela_planning(t,c,1274+(i*17),1263+(i*14),1243+(i*10),1225+(i*6),2020+(i%5));}
ps("\nElaeagnus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ela_execution(t,c,1263+(i*15),1252+(i*12),1234+(i*8),1221+(i*5),2021+(i%4));}
ps("\nElaeagnus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ela_evaluation(t,c,1255+(i*13),1244+(i*10),1228+(i*7),1217+(i*4),2022+(i%3));}
ps("\nElaeagnus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ela_accessory(t,c,1247+(i*11),1238+(i*9),1224+(i*6),1214+(i*3),2023+(i%2));}
ps("\nElaeagnus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ela_market(t,c,1241+(i*9),1232+(i*7),1219+(i*5),1211+(i*3),2024);}
ps("\n");ela_report();ela_state();ps("\n=== Demo Complete ===\n");return 0;}
