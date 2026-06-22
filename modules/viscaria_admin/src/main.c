/* viscaria_admin: Viscaria management technology administration (v1.0)
 * Viscaria planning, viscaria execution, viscaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} visc_t;
typedef struct{int n_viscp,n_visc,n_visc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} visc_state_t;
static visc_t viscs[N],viscess[N-2],viscvss[N-4],viscas[N-6],viscmks[N-6]; static visc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(visc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;visc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VISC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int visc_init(void){if(init)return -1;st.n_viscp=0;st.n_visc=0;st.n_visc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)viscs[i].active=0;for(int i=0;i<N-2;i++)viscess[i].active=0;for(int i=0;i<N-4;i++)viscvss[i].active=0;for(int i=0;i<N-6;i++)viscas[i].active=0;for(int i=0;i<N-6;i++)viscmks[i].active=0;init=1;ps("[VISC] Viscaria initialized\n");return 0;}
int visc_planning(int t,int c,int a,int b,int d,int e,int y){return add(viscs,&st.n_viscp,&st.t_f1,N,t,c,a,b,d,e,y);}
int visc_execution(int t,int c,int a,int b,int d,int e,int y){return add(viscess,&st.n_visc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int visc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(viscvss,&st.n_visc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int visc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(viscas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int visc_market(int t,int c,int a,int b,int d,int e,int y){return add(viscmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void visc_report(void){ps("[VISC] Viscp: ");pi(st.n_viscp);ps(" PCS=");pi(st.t_f1);ps("\nVisce: ");pi(st.n_visc);ps(" PCS=");pi(st.t_f2);ps("\nViscv: ");pi(st.n_visc2);ps(" PCS=");pi(st.t_f3);ps("\nViscc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void visc_state(void){ps("[VISC] Viscp=");pi(st.n_viscp);ps(" Visce=");pi(st.n_visc);ps(" Viscv=");pi(st.n_visc2);ps(" Viscc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Viscaria Admin Demo ===\n\n");visc_init();
ps("Viscaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;visc_planning(t,c,1364+(i*17),1353+(i*14),1333+(i*10),1315+(i*6),2020+(i%5));}
ps("\nViscaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;visc_execution(t,c,1353+(i*15),1342+(i*12),1324+(i*8),1311+(i*5),2021+(i%4));}
ps("\nViscaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;visc_evaluation(t,c,1345+(i*13),1334+(i*10),1318+(i*7),1307+(i*4),2022+(i%3));}
ps("\nViscaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;visc_accessory(t,c,1337+(i*11),1328+(i*9),1314+(i*6),1304+(i*3),2023+(i%2));}
ps("\nViscaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;visc_market(t,c,1331+(i*9),1322+(i*7),1309+(i*5),1301+(i*3),2024);}
ps("\n");visc_report();visc_state();ps("\n=== Demo Complete ===\n");return 0;}
