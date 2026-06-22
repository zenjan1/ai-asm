/* hylocereus_admin: Hylocereus management technology administration (v1.0)
 * Hylocereus planning, hylocereus execution, hylocereus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hylo_t;
typedef struct{int n_hylop,n_hylo,n_hylo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hylo_state_t;
static hylo_t hylos[N],hyloe[N-2],hylo2[N-4],hyloac[N-6],hyloam[N-6]; static hylo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hylo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hylo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYLO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hylo_init(void){if(init)return -1;st.n_hylop=0;st.n_hylo=0;st.n_hylo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hylos[i].active=0;for(int i=0;i<N-2;i++)hyloe[i].active=0;for(int i=0;i<N-4;i++)hylo2[i].active=0;for(int i=0;i<N-6;i++)hyloac[i].active=0;for(int i=0;i<N-6;i++)hyloam[i].active=0;init=1;ps("[HYLO] Hylocereus initialized\n");return 0;}
int hylo_planning(int t,int c,int a,int b,int d,int e,int y){return add(hylos,&st.n_hylop,&st.t_f1,N,t,c,a,b,d,e,y);}
int hylo_execution(int t,int c,int a,int b,int d,int e,int y){return add(hyloe,&st.n_hylo,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hylo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hylo2,&st.n_hylo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hylo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hyloac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hylo_market(int t,int c,int a,int b,int d,int e,int y){return add(hyloam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hylo_report(void){ps("[HYLO] Hylop: ");pi(st.n_hylop);ps(" PCS=");pi(st.t_f1);ps("\nHyloe: ");pi(st.n_hylo);ps(" PCS=");pi(st.t_f2);ps("\nHylo2: ");pi(st.n_hylo2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hylo_state(void){ps("[HYLO] Hylop=");pi(st.n_hylop);ps(" Hylo=");pi(st.n_hylo);ps(" Hylo2=");pi(st.n_hylo2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hylocereus Admin Demo ===\n\n");hylo_init();
ps("Hylocereus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hylo_planning(t,c,1539+(i*17),1528+(i*14),1508+(i*10),1490+(i*6),2020+(i%5));}
ps("\nHylocereus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hylo_execution(t,c,1528+(i*15),1517+(i*12),1499+(i*8),1486+(i*5),2021+(i%4));}
ps("\nHylocereus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hylo_evaluation(t,c,1520+(i*13),1509+(i*10),1493+(i*7),1482+(i*4),2022+(i%3));}
ps("\nHylocereus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hylo_accessory(t,c,1512+(i*11),1503+(i*9),1489+(i*6),1479+(i*3),2023+(i%2));}
ps("\nHylocereus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hylo_market(t,c,1506+(i*9),1497+(i*7),1484+(i*5),1476+(i*3),2024);}
ps("\n");hylo_report();hylo_state();ps("\n=== Demo Complete ===\n");return 0;}
