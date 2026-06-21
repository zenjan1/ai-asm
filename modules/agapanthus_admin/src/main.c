/* agapanthus_admin: Agapanthus management technology administration (v1.0)
 * Agapanthus planning, agapanthus execution, agapanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aga_t;
typedef struct{int n_agp,n_age,n_agv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aga_state_t;
static aga_t agaps[N],agae[N-2],agav[N-4],acs[N-6],mks[N-6]; static aga_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aga_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aga_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AGA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aga_init(void){if(init)return -1;st.n_agp=0;st.n_age=0;st.n_agv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)agaps[i].active=0;for(int i=0;i<N-2;i++)agae[i].active=0;for(int i=0;i<N-4;i++)agav[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[AGA] Agapanthus initialized\n");return 0;}
int aga_planning(int t,int c,int a,int b,int d,int e,int y){return add(agaps,&st.n_agp,&st.t_f1,N,t,c,a,b,d,e,y);}
int aga_execution(int t,int c,int a,int b,int d,int e,int y){return add(agae,&st.n_age,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aga_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(agav,&st.n_agv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aga_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aga_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aga_report(void){ps("[AGA] Agp: ");pi(st.n_agp);ps(" PCS=");pi(st.t_f1);ps("\nAge: ");pi(st.n_age);ps(" PCS=");pi(st.t_f2);ps("\nAgv: ");pi(st.n_agv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aga_state(void){ps("[AGA] Agp=");pi(st.n_agp);ps(" Age=");pi(st.n_age);ps(" Agv=");pi(st.n_agv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Agapanthus Admin Demo ===\n\n");aga_init();
ps("Agapanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aga_planning(t,c,622+(i*17),611+(i*14),591+(i*10),573+(i*6),2020+(i%5));}
ps("\nAgapanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aga_execution(t,c,611+(i*15),600+(i*12),582+(i*8),569+(i*5),2021+(i%4));}
ps("\nAgapanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aga_evaluation(t,c,603+(i*13),592+(i*10),576+(i*7),565+(i*4),2022+(i%3));}
ps("\nAgapanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aga_accessory(t,c,595+(i*11),586+(i*9),572+(i*6),562+(i*3),2023+(i%2));}
ps("\nAgapanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aga_market(t,c,589+(i*9),580+(i*7),567+(i*5),559+(i*3),2024);}
ps("\n");aga_report();aga_state();ps("\n=== Demo Complete ===\n");return 0;}
