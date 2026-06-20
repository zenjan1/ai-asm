/* teatech_admin: Tea technology administration (v1.0)
 * Tea planting, tea processing, tea culture, tea marketing, tea equipment
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tet_t;
typedef struct{int n_gp,n_pc,n_cl,n_mk,n_eq,t_f1,t_f2,t_f3,t_f4,t_f5;} tet_state_t;
static tet_t gps[N],pcs[N-2],cls[N-4],mks[N-6],eqs[N-6]; static tet_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tet_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tet_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TET] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tet_init(void){if(init)return -1;st.n_gp=0;st.n_pc=0;st.n_cl=0;st.n_mk=0;st.n_eq=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gps[i].active=0;for(int i=0;i<N-2;i++)pcs[i].active=0;for(int i=0;i<N-4;i++)cls[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;for(int i=0;i<N-6;i++)eqs[i].active=0;init=1;ps("[TET] Teatech initialized\n");return 0;}
int tet_plant(int t,int c,int a,int b,int d,int e,int y){return add(gps,&st.n_gp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tet_process(int t,int c,int a,int b,int d,int e,int y){return add(pcs,&st.n_pc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tet_culture(int t,int c,int a,int b,int d,int e,int y){return add(cls,&st.n_cl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tet_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tet_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tet_report(void){ps("[TET] Plant: ");pi(st.n_gp);ps(" ha=");pi(st.t_f1);ps("\nProc: ");pi(st.n_pc);ps(" Ton=");pi(st.t_f2);ps("\nCulture: ");pi(st.n_cl);ps(" Cer=");pi(st.t_f3);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f4);ps("\nEquip: ");pi(st.n_eq);ps(" Mach=");pi(st.t_f5);ps("\n");}
void tet_state(void){ps("[TET] Gp=");pi(st.n_gp);ps(" Pc=");pi(st.n_pc);ps(" Cl=");pi(st.n_cl);ps(" Mk=");pi(st.n_mk);ps(" Eq=");pi(st.n_eq);ps("\n");}
int main(void){
ps("=== Tea Tech Admin Demo ===\n\n");tet_init();
ps("Tea planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tet_plant(t,c,173+(i*17),158+(i*14),138+(i*10),121+(i*6),2020+(i%5));}
ps("\nTea processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tet_process(t,c,162+(i*15),148+(i*12),130+(i*8),117+(i*5),2021+(i%4));}
ps("\nTea culture...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tet_culture(t,c,154+(i*13),140+(i*10),124+(i*7),113+(i*4),2022+(i%3));}
ps("\nTea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tet_market(t,c,146+(i*11),134+(i*9),120+(i*6),110+(i*3),2023+(i%2));}
ps("\nTea equipment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tet_equipment(t,c,140+(i*9),129+(i*7),116+(i*5),108+(i*3),2024);}
ps("\n");tet_report();tet_state();ps("\n=== Demo Complete ===\n");return 0;}
