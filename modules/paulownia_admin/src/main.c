/* paulownia_admin: Paulownia management technology administration (v1.0)
 * Paulownia planning, paulownia execution, paulownia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} plw_t;
typedef struct{int n_plp,n_ple,n_plv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} plw_state_t;
static plw_t plwps[N],plwss[N-2],plwvss[N-4],plwas[N-6],plwmks[N-6]; static plw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(plw_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;plw_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PLW] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int plw_init(void){if(init)return -1;st.n_plp=0;st.n_ple=0;st.n_plv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)plwps[i].active=0;for(int i=0;i<N-2;i++)plwss[i].active=0;for(int i=0;i<N-4;i++)plwvss[i].active=0;for(int i=0;i<N-6;i++)plwas[i].active=0;for(int i=0;i<N-6;i++)plwmks[i].active=0;init=1;ps("[PLW] Paulownia initialized\n");return 0;}
int plw_planning(int t,int c,int a,int b,int d,int e,int y){return add(plwps,&st.n_plp,&st.t_f1,N,t,c,a,b,d,e,y);}
int plw_execution(int t,int c,int a,int b,int d,int e,int y){return add(plwss,&st.n_ple,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int plw_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(plwvss,&st.n_plv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int plw_accessory(int t,int c,int a,int b,int d,int e,int y){return add(plwas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int plw_market(int t,int c,int a,int b,int d,int e,int y){return add(plwmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void plw_report(void){ps("[PLW] Plp: ");pi(st.n_plp);ps(" PCS=");pi(st.t_f1);ps("\nPle: ");pi(st.n_ple);ps(" PCS=");pi(st.t_f2);ps("\nPlv: ");pi(st.n_plv);ps(" PCS=");pi(st.t_f3);ps("\nPlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void plw_state(void){ps("[PLW] Plp=");pi(st.n_plp);ps(" Ple=");pi(st.n_ple);ps(" Plv=");pi(st.n_plv);ps(" Plc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Paulownia Admin Demo ===\n\n");plw_init();
ps("Paulownia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;plw_planning(t,c,852+(i*17),841+(i*14),821+(i*10),803+(i*6),2020+(i%5));}
ps("\nPaulownia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;plw_execution(t,c,841+(i*15),830+(i*12),812+(i*8),799+(i*5),2021+(i%4));}
ps("\nPaulownia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;plw_evaluation(t,c,833+(i*13),822+(i*10),806+(i*7),795+(i*4),2022+(i%3));}
ps("\nPaulownia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plw_accessory(t,c,825+(i*11),816+(i*9),802+(i*6),792+(i*3),2023+(i%2));}
ps("\nPaulownia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plw_market(t,c,819+(i*9),810+(i*7),797+(i*5),789+(i*3),2024);}
ps("\n");plw_report();plw_state();ps("\n=== Demo Complete ===\n");return 0;}
