/* crupina_admin: Crupina management technology administration (v1.0)
 * Crupina planning, crupina execution, crupina evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cru_t;
typedef struct{int n_crup,n_crue,n_cru2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cru_state_t;
static cru_t crups[N],cruss[N-2],cruvss[N-4],cruas[N-6],crumks[N-6]; static cru_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cru_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cru_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cru_init(void){if(init)return -1;st.n_crup=0;st.n_crue=0;st.n_cru2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)crups[i].active=0;for(int i=0;i<N-2;i++)cruss[i].active=0;for(int i=0;i<N-4;i++)cruvss[i].active=0;for(int i=0;i<N-6;i++)cruas[i].active=0;for(int i=0;i<N-6;i++)crumks[i].active=0;init=1;ps("[CRU] Crupina initialized\n");return 0;}
int cru_planning(int t,int c,int a,int b,int d,int e,int y){return add(crups,&st.n_crup,&st.t_f1,N,t,c,a,b,d,e,y);}
int cru_execution(int t,int c,int a,int b,int d,int e,int y){return add(cruss,&st.n_crue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cru_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cruvss,&st.n_cru2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cru_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cruas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cru_market(int t,int c,int a,int b,int d,int e,int y){return add(crumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cru_report(void){ps("[CRU] Cruup: ");pi(st.n_crup);ps(" PCS=");pi(st.t_f1);ps("\nCrue: ");pi(st.n_crue);ps(" PCS=");pi(st.t_f2);ps("\nCruv: ");pi(st.n_cru2);ps(" PCS=");pi(st.t_f3);ps("\nCruc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cru_state(void){ps("[CRU] Cruup=");pi(st.n_crup);ps(" Crue=");pi(st.n_crue);ps(" Ruv=");pi(st.n_cru2);ps(" Cruc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Crupina Admin Demo ===\n\n");cru_init();
ps("Crupina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cru_planning(t,c,989+(i*17),978+(i*14),958+(i*10),940+(i*6),2020+(i%5));}
ps("\nCrupina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cru_execution(t,c,978+(i*15),967+(i*12),949+(i*8),936+(i*5),2021+(i%4));}
ps("\nCrupina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cru_evaluation(t,c,970+(i*13),959+(i*10),943+(i*7),932+(i*4),2022+(i%3));}
ps("\nCrupina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cru_accessory(t,c,962+(i*11),953+(i*9),939+(i*6),929+(i*3),2023+(i%2));}
ps("\nCrupina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cru_market(t,c,956+(i*9),947+(i*7),934+(i*5),926+(i*3),2024);}
ps("\n");cru_report();cru_state();ps("\n=== Demo Complete ===\n");return 0;}
