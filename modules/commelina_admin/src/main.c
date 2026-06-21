/* commelina_admin: Commelina management technology administration (v1.0)
 * Commelina planning, commelina execution, commelina evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} com_t;
typedef struct{int n_comp,n_comce,n_com2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} com_state_t;
static com_t comps[N],comss[N-2],comvss[N-4],comas[N-6],commks[N-6]; static com_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(com_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;com_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[COM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int com_init(void){if(init)return -1;st.n_comp=0;st.n_comce=0;st.n_com2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)comps[i].active=0;for(int i=0;i<N-2;i++)comss[i].active=0;for(int i=0;i<N-4;i++)comvss[i].active=0;for(int i=0;i<N-6;i++)comas[i].active=0;for(int i=0;i<N-6;i++)commks[i].active=0;init=1;ps("[COM] Commelina initialized\n");return 0;}
int com_planning(int t,int c,int a,int b,int d,int e,int y){return add(comps,&st.n_comp,&st.t_f1,N,t,c,a,b,d,e,y);}
int com_execution(int t,int c,int a,int b,int d,int e,int y){return add(comss,&st.n_comce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int com_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(comvss,&st.n_com2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int com_accessory(int t,int c,int a,int b,int d,int e,int y){return add(comas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int com_market(int t,int c,int a,int b,int d,int e,int y){return add(commks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void com_report(void){ps("[COM] Comp: ");pi(st.n_comp);ps(" PCS=");pi(st.t_f1);ps("\nComc: ");pi(st.n_comce);ps(" PCS=");pi(st.t_f2);ps("\nComv: ");pi(st.n_com2);ps(" PCS=");pi(st.t_f3);ps("\nComc2: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void com_state(void){ps("[COM] Comp=");pi(st.n_comp);ps(" Comc=");pi(st.n_comce);ps(" Comv=");pi(st.n_com2);ps(" Comc2=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Commelina Admin Demo ===\n\n");com_init();
ps("Commelina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;com_planning(t,c,911+(i*17),900+(i*14),880+(i*10),862+(i*6),2020+(i%5));}
ps("\nCommelina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;com_execution(t,c,900+(i*15),889+(i*12),871+(i*8),858+(i*5),2021+(i%4));}
ps("\nCommelina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;com_evaluation(t,c,892+(i*13),881+(i*10),865+(i*7),854+(i*4),2022+(i%3));}
ps("\nCommelina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;com_accessory(t,c,884+(i*11),875+(i*9),861+(i*6),851+(i*3),2023+(i%2));}
ps("\nCommelina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;com_market(t,c,878+(i*9),869+(i*7),856+(i*5),848+(i*3),2024);}
ps("\n");com_report();com_state();ps("\n=== Demo Complete ===\n");return 0;}
