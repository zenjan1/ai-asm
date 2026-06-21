/* leonurus_admin: Leonurus management technology administration (v1.0)
 * Leonurus planning, leonurus execution, leonurus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} leo_t;
typedef struct{int n_leop,n_leoe,n_leo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} leo_state_t;
static leo_t leops[N],leoss[N-2],leovss[N-4],leas[N-6],leomks[N-6]; static leo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(leo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;leo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int leo_init(void){if(init)return -1;st.n_leop=0;st.n_leoe=0;st.n_leo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)leops[i].active=0;for(int i=0;i<N-2;i++)leoss[i].active=0;for(int i=0;i<N-4;i++)leovss[i].active=0;for(int i=0;i<N-6;i++)leas[i].active=0;for(int i=0;i<N-6;i++)leomks[i].active=0;init=1;ps("[LEO] Leonurus initialized\n");return 0;}
int leo_planning(int t,int c,int a,int b,int d,int e,int y){return add(leops,&st.n_leop,&st.t_f1,N,t,c,a,b,d,e,y);}
int leo_execution(int t,int c,int a,int b,int d,int e,int y){return add(leoss,&st.n_leoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int leo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(leovss,&st.n_leo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int leo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(leas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int leo_market(int t,int c,int a,int b,int d,int e,int y){return add(leomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void leo_report(void){ps("[LEO] Leopp: ");pi(st.n_leop);ps(" PCS=");pi(st.t_f1);ps("\nLeoe: ");pi(st.n_leoe);ps(" PCS=");pi(st.t_f2);ps("\nLeov: ");pi(st.n_leo2);ps(" PCS=");pi(st.t_f3);ps("\nLeoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void leo_state(void){ps("[LEO] Leop=");pi(st.n_leop);ps(" Leoe=");pi(st.n_leoe);ps(" Leov=");pi(st.n_leo2);ps(" Leoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Leonurus Admin Demo ===\n\n");leo_init();
ps("Leonurus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;leo_planning(t,c,1051+(i*17),1040+(i*14),1020+(i*10),1002+(i*6),2020+(i%5));}
ps("\nLeonurus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;leo_execution(t,c,1040+(i*15),1029+(i*12),1011+(i*8),998+(i*5),2021+(i%4));}
ps("\nLeonurus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;leo_evaluation(t,c,1032+(i*13),1021+(i*10),1005+(i*7),994+(i*4),2022+(i%3));}
ps("\nLeonurus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;leo_accessory(t,c,1024+(i*11),1015+(i*9),1001+(i*6),991+(i*3),2023+(i%2));}
ps("\nLeonurus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;leo_market(t,c,1018+(i*9),1009+(i*7),996+(i*5),988+(i*3),2024);}
ps("\n");leo_report();leo_state();ps("\n=== Demo Complete ===\n");return 0;}
