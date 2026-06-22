/* isatis_admin: Isatis management technology administration (v1.0)
 * Isatis planning, isatis execution, isatis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} isa_t;
typedef struct{int n_isap,n_isae,n_isa2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} isa_state_t;
static isa_t isaps[N],isaess[N-2],isavss[N-4],isaas[N-6],isamks[N-6]; static isa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(isa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;isa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ISA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int isa_init(void){if(init)return -1;st.n_isap=0;st.n_isae=0;st.n_isa2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)isaps[i].active=0;for(int i=0;i<N-2;i++)isaess[i].active=0;for(int i=0;i<N-4;i++)isavss[i].active=0;for(int i=0;i<N-6;i++)isaas[i].active=0;for(int i=0;i<N-6;i++)isamks[i].active=0;init=1;ps("[ISA] Isatis initialized\n");return 0;}
int isa_planning(int t,int c,int a,int b,int d,int e,int y){return add(isaps,&st.n_isap,&st.t_f1,N,t,c,a,b,d,e,y);}
int isa_execution(int t,int c,int a,int b,int d,int e,int y){return add(isaess,&st.n_isae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int isa_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(isavss,&st.n_isa2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int isa_accessory(int t,int c,int a,int b,int d,int e,int y){return add(isaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int isa_market(int t,int c,int a,int b,int d,int e,int y){return add(isamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void isa_report(void){ps("[ISA] Isap: ");pi(st.n_isap);ps(" PCS=");pi(st.t_f1);ps("\nIsae: ");pi(st.n_isae);ps(" PCS=");pi(st.t_f2);ps("\nIsav: ");pi(st.n_isa2);ps(" PCS=");pi(st.t_f3);ps("\nIsac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void isa_state(void){ps("[ISA] Isap=");pi(st.n_isap);ps(" Isae=");pi(st.n_isae);ps(" Isav=");pi(st.n_isa2);ps(" Isac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Isatis Admin Demo ===\n\n");isa_init();
ps("Isatis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;isa_planning(t,c,1300+(i*17),1289+(i*14),1269+(i*10),1251+(i*6),2020+(i%5));}
ps("\nIsatis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;isa_execution(t,c,1289+(i*15),1278+(i*12),1260+(i*8),1247+(i*5),2021+(i%4));}
ps("\nIsatis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;isa_evaluation(t,c,1281+(i*13),1270+(i*10),1254+(i*7),1243+(i*4),2022+(i%3));}
ps("\nIsatis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;isa_accessory(t,c,1273+(i*11),1264+(i*9),1250+(i*6),1240+(i*3),2023+(i%2));}
ps("\nIsatis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;isa_market(t,c,1267+(i*9),1258+(i*7),1245+(i*5),1237+(i*3),2024);}
ps("\n");isa_report();isa_state();ps("\n=== Demo Complete ===\n");return 0;}
