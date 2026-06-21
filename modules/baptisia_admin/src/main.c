/* baptisia_admin: Baptisia management technology administration (v1.0)
 * Baptisia planning, baptisia execution, baptisia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bap_t;
typedef struct{int n_bapp,n_bape,n_bap2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bap_state_t;
static bap_t bapps[N],bapss[N-2],bapvss[N-4],bapas[N-6],bapmks[N-6]; static bap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bap_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bap_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BAP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bap_init(void){if(init)return -1;st.n_bapp=0;st.n_bape=0;st.n_bap2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bapps[i].active=0;for(int i=0;i<N-2;i++)bapss[i].active=0;for(int i=0;i<N-4;i++)bapvss[i].active=0;for(int i=0;i<N-6;i++)bapas[i].active=0;for(int i=0;i<N-6;i++)bapmks[i].active=0;init=1;ps("[BAP] Baptisia initialized\n");return 0;}
int bap_planning(int t,int c,int a,int b,int d,int e,int y){return add(bapps,&st.n_bapp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bap_execution(int t,int c,int a,int b,int d,int e,int y){return add(bapss,&st.n_bape,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bap_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bapvss,&st.n_bap2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bap_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bapas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bap_market(int t,int c,int a,int b,int d,int e,int y){return add(bapmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bap_report(void){ps("[BAP] Bapp: ");pi(st.n_bapp);ps(" PCS=");pi(st.t_f1);ps("\nBape: ");pi(st.n_bape);ps(" PCS=");pi(st.t_f2);ps("\nBapv: ");pi(st.n_bap2);ps(" PCS=");pi(st.t_f3);ps("\nBapc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bap_state(void){ps("[BAP] Bapp=");pi(st.n_bapp);ps(" Bape=");pi(st.n_bape);ps(" Bapv=");pi(st.n_bap2);ps(" Bapc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Baptisia Admin Demo ===\n\n");bap_init();
ps("Baptisia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bap_planning(t,c,978+(i*17),967+(i*14),947+(i*10),929+(i*6),2020+(i%5));}
ps("\nBaptisia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bap_execution(t,c,967+(i*15),956+(i*12),938+(i*8),925+(i*5),2021+(i%4));}
ps("\nBaptisia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bap_evaluation(t,c,959+(i*13),948+(i*10),932+(i*7),921+(i*4),2022+(i%3));}
ps("\nBaptisia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bap_accessory(t,c,951+(i*11),942+(i*9),928+(i*6),918+(i*3),2023+(i%2));}
ps("\nBaptisia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bap_market(t,c,945+(i*9),936+(i*7),923+(i*5),915+(i*3),2024);}
ps("\n");bap_report();bap_state();ps("\n=== Demo Complete ===\n");return 0;}
