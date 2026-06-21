/* alstroemeria_admin: Alstroemeria management technology administration (v1.0)
 * Alstroemeria planning, alstroemeria execution, alstroemeria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} als_t;
typedef struct{int n_alp,n_ale,n_alv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} als_state_t;
static als_t alps[N],ales[N-2],alvs[N-4],acs[N-6],mks[N-6]; static als_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(als_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;als_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ALS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int als_init(void){if(init)return -1;st.n_alp=0;st.n_ale=0;st.n_alv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)alps[i].active=0;for(int i=0;i<N-2;i++)ales[i].active=0;for(int i=0;i<N-4;i++)alvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ALS] Alstroemeria initialized\n");return 0;}
int als_planning(int t,int c,int a,int b,int d,int e,int y){return add(alps,&st.n_alp,&st.t_f1,N,t,c,a,b,d,e,y);}
int als_execution(int t,int c,int a,int b,int d,int e,int y){return add(ales,&st.n_ale,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int als_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(alvs,&st.n_alv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int als_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int als_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void als_report(void){ps("[ALS] Alp: ");pi(st.n_alp);ps(" PCS=");pi(st.t_f1);ps("\nAle: ");pi(st.n_ale);ps(" PCS=");pi(st.t_f2);ps("\nAlv: ");pi(st.n_alv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void als_state(void){ps("[ALS] Alp=");pi(st.n_alp);ps(" Ale=");pi(st.n_ale);ps(" Alv=");pi(st.n_alv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Alstroemeria Admin Demo ===\n\n");als_init();
ps("Alstroemeria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;als_planning(t,c,621+(i*17),610+(i*14),590+(i*10),572+(i*6),2020+(i%5));}
ps("\nAlstroemeria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;als_execution(t,c,610+(i*15),599+(i*12),581+(i*8),568+(i*5),2021+(i%4));}
ps("\nAlstroemeria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;als_evaluation(t,c,602+(i*13),591+(i*10),575+(i*7),564+(i*4),2022+(i%3));}
ps("\nAlstroemeria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;als_accessory(t,c,594+(i*11),585+(i*9),571+(i*6),561+(i*3),2023+(i%2));}
ps("\nAlstroemeria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;als_market(t,c,588+(i*9),579+(i*7),566+(i*5),558+(i*3),2024);}
ps("\n");als_report();als_state();ps("\n=== Demo Complete ===\n");return 0;}
