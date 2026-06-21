/* prunus_admin: Prunus management technology administration (v1.0)
 * Prunus planning, prunus execution, prunus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pru_t;
typedef struct{int n_prup,n_prue,n_pru2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pru_state_t;
static pru_t prups[N],pruss[N-2],pruvss[N-4],pruas[N-6],prumks[N-6]; static pru_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pru_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pru_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PRU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pru_init(void){if(init)return -1;st.n_prup=0;st.n_prue=0;st.n_pru2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)prups[i].active=0;for(int i=0;i<N-2;i++)pruss[i].active=0;for(int i=0;i<N-4;i++)pruvss[i].active=0;for(int i=0;i<N-6;i++)pruas[i].active=0;for(int i=0;i<N-6;i++)prumks[i].active=0;init=1;ps("[PRU] Prunus initialized\n");return 0;}
int pru_planning(int t,int c,int a,int b,int d,int e,int y){return add(prups,&st.n_prup,&st.t_f1,N,t,c,a,b,d,e,y);}
int pru_execution(int t,int c,int a,int b,int d,int e,int y){return add(pruss,&st.n_prue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pru_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pruvss,&st.n_pru2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pru_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pruas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pru_market(int t,int c,int a,int b,int d,int e,int y){return add(prumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pru_report(void){ps("[PRU] prup: ");pi(st.n_prup);ps(" PCS=");pi(st.t_f1);ps("\nprue: ");pi(st.n_prue);ps(" PCS=");pi(st.t_f2);ps("\npruv: ");pi(st.n_pru2);ps(" PCS=");pi(st.t_f3);ps("\npruc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pru_state(void){ps("[PRU] prup=");pi(st.n_prup);ps(" prue=");pi(st.n_prue);ps(" pruv=");pi(st.n_pru2);ps(" pruc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Prunus Admin Demo ===\n\n");pru_init();
ps("Prunus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pru_planning(t,c,1087+(i*17),1076+(i*14),1056+(i*10),1038+(i*6),2020+(i%5));}
ps("\nPrunus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pru_execution(t,c,1076+(i*15),1065+(i*12),1047+(i*8),1034+(i*5),2021+(i%4));}
ps("\nPrunus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pru_evaluation(t,c,1068+(i*13),1057+(i*10),1041+(i*7),1030+(i*4),2022+(i%3));}
ps("\nPrunus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pru_accessory(t,c,1060+(i*11),1051+(i*9),1037+(i*6),1027+(i*3),2023+(i%2));}
ps("\nPrunus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pru_market(t,c,1054+(i*9),1045+(i*7),1032+(i*5),1024+(i*3),2024);}
ps("\n");pru_report();pru_state();ps("\n=== Demo Complete ===\n");return 0;}
