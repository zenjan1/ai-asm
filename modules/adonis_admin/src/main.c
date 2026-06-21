/* adonis_admin: Adonis management technology administration (v1.0)
 * Adonis planning, adonis execution, adonis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ado_t;
typedef struct{int n_adop,n_adoe,n_ado2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ado_state_t;
static ado_t adops[N],adoss[N-2],adovss[N-4],adoas[N-6],adomks[N-6]; static ado_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ado_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ado_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ADO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ado_init(void){if(init)return -1;st.n_adop=0;st.n_adoe=0;st.n_ado2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)adops[i].active=0;for(int i=0;i<N-2;i++)adoss[i].active=0;for(int i=0;i<N-4;i++)adovss[i].active=0;for(int i=0;i<N-6;i++)adoas[i].active=0;for(int i=0;i<N-6;i++)adomks[i].active=0;init=1;ps("[ADO] Adonis initialized\n");return 0;}
int ado_planning(int t,int c,int a,int b,int d,int e,int y){return add(adops,&st.n_adop,&st.t_f1,N,t,c,a,b,d,e,y);}
int ado_execution(int t,int c,int a,int b,int d,int e,int y){return add(adoss,&st.n_adoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ado_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(adovss,&st.n_ado2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ado_accessory(int t,int c,int a,int b,int d,int e,int y){return add(adoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ado_market(int t,int c,int a,int b,int d,int e,int y){return add(adomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ado_report(void){ps("[ADO] Adop: ");pi(st.n_adop);ps(" PCS=");pi(st.t_f1);ps("\nAdoe: ");pi(st.n_adoe);ps(" PCS=");pi(st.t_f2);ps("\nAdov: ");pi(st.n_ado2);ps(" PCS=");pi(st.t_f3);ps("\nAdoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ado_state(void){ps("[ADO] Adop=");pi(st.n_adop);ps(" Adoe=");pi(st.n_adoe);ps(" Adov=");pi(st.n_ado2);ps(" Adoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Adonis Admin Demo ===\n\n");ado_init();
ps("Adonis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ado_planning(t,c,967+(i*17),956+(i*14),936+(i*10),918+(i*6),2020+(i%5));}
ps("\nAdonis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ado_execution(t,c,956+(i*15),945+(i*12),927+(i*8),914+(i*5),2021+(i%4));}
ps("\nAdonis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ado_evaluation(t,c,948+(i*13),937+(i*10),921+(i*7),910+(i*4),2022+(i%3));}
ps("\nAdonis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ado_accessory(t,c,940+(i*11),931+(i*9),917+(i*6),907+(i*3),2023+(i%2));}
ps("\nAdonis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ado_market(t,c,934+(i*9),925+(i*7),912+(i*5),904+(i*3),2024);}
ps("\n");ado_report();ado_state();ps("\n=== Demo Complete ===\n");return 0;}
