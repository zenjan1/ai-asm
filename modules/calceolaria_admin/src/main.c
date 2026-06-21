/* calceolaria_admin: Calceolaria management technology administration (v1.0)
 * Calceolaria planning, calceolaria execution, calceolaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cl_t;
typedef struct{int n_clp,n_cle,n_clv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cl_state_t;
static cl_t clps[N],clss[N-2],clvss[N-4],clas[N-6],clmks[N-6]; static cl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int clc_init(void){if(init)return -1;st.n_clp=0;st.n_cle=0;st.n_clv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)clps[i].active=0;for(int i=0;i<N-2;i++)clss[i].active=0;for(int i=0;i<N-4;i++)clvss[i].active=0;for(int i=0;i<N-6;i++)clas[i].active=0;for(int i=0;i<N-6;i++)clmks[i].active=0;init=1;ps("[CLC] Calceolaria initialized\n");return 0;}
int clc_planning(int t,int c,int a,int b,int d,int e,int y){return add(clps,&st.n_clp,&st.t_f1,N,t,c,a,b,d,e,y);}
int clc_execution(int t,int c,int a,int b,int d,int e,int y){return add(clss,&st.n_cle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int clc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(clvss,&st.n_clv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int clc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(clas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int clc_market(int t,int c,int a,int b,int d,int e,int y){return add(clmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void clc_report(void){ps("[CLC] Clp: ");pi(st.n_clp);ps(" PCS=");pi(st.t_f1);ps("\nCle: ");pi(st.n_cle);ps(" PCS=");pi(st.t_f2);ps("\nClv: ");pi(st.n_clv);ps(" PCS=");pi(st.t_f3);ps("\nClc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void clc_state(void){ps("[CLC] Clp=");pi(st.n_clp);ps(" Cle=");pi(st.n_cle);ps(" Clv=");pi(st.n_clv);ps(" Clc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Calceolaria Admin Demo ===\n\n");clc_init();
ps("Calceolaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;clc_planning(t,c,762+(i*17),751+(i*14),731+(i*10),713+(i*6),2020+(i%5));}
ps("\nCalceolaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;clc_execution(t,c,751+(i*15),740+(i*12),722+(i*8),709+(i*5),2021+(i%4));}
ps("\nCalceolaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;clc_evaluation(t,c,743+(i*13),732+(i*10),718+(i*7),707+(i*4),2022+(i%3));}
ps("\nCalceolaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clc_accessory(t,c,735+(i*11),726+(i*9),712+(i*6),702+(i*3),2023+(i%2));}
ps("\nCalceolaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clc_market(t,c,729+(i*9),720+(i*7),707+(i*5),699+(i*3),2024);}
ps("\n");clc_report();clc_state();ps("\n=== Demo Complete ===\n");return 0;}
