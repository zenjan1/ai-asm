/* vitis_admin: Vitis management technology administration (v1.0)
 * Vitis planning, vitis execution, vitis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} viti_t;
typedef struct{int n_vitip,n_viti,n_viti2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} viti_state_t;
static viti_t vitis[N],vitie[N-2],vitiv[N-4],vitiac[N-6],vitim[N-6]; static viti_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(viti_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;viti_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VITI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int viti_init(void){if(init)return -1;st.n_vitip=0;st.n_viti=0;st.n_viti2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vitis[i].active=0;for(int i=0;i<N-2;i++)vitie[i].active=0;for(int i=0;i<N-4;i++)vitiv[i].active=0;for(int i=0;i<N-6;i++)vitiac[i].active=0;for(int i=0;i<N-6;i++)vitim[i].active=0;init=1;ps("[VITI] Vitis initialized\n");return 0;}
int viti_planning(int t,int c,int a,int b,int d,int e,int y){return add(vitis,&st.n_vitip,&st.t_f1,N,t,c,a,b,d,e,y);}
int viti_execution(int t,int c,int a,int b,int d,int e,int y){return add(vitie,&st.n_viti,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int viti_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vitiv,&st.n_viti2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int viti_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vitiac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int viti_market(int t,int c,int a,int b,int d,int e,int y){return add(vitim,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void viti_report(void){ps("[VITI] Vitip: ");pi(st.n_vitip);ps(" PCS=");pi(st.t_f1);ps("\nVitie: ");pi(st.n_viti);ps(" PCS=");pi(st.t_f2);ps("\nVitiv: ");pi(st.n_viti2);ps(" PCS=");pi(st.t_f3);ps("\nVitac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void viti_state(void){ps("[VITI] Vitip=");pi(st.n_vitip);ps(" Vitie=");pi(st.n_viti);ps(" Vitiv=");pi(st.n_viti2);ps(" Vitac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vitis Admin Demo ===\n\n");viti_init();
ps("Vitis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;viti_planning(t,c,1366+(i*17),1355+(i*14),1335+(i*10),1317+(i*6),2020+(i%5));}
ps("\nVitis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;viti_execution(t,c,1355+(i*15),1344+(i*12),1326+(i*8),1313+(i*5),2021+(i%4));}
ps("\nVitis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;viti_evaluation(t,c,1347+(i*13),1336+(i*10),1320+(i*7),1309+(i*4),2022+(i%3));}
ps("\nVitis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;viti_accessory(t,c,1339+(i*11),1330+(i*9),1316+(i*6),1306+(i*3),2023+(i%2));}
ps("\nVitis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;viti_market(t,c,1333+(i*9),1324+(i*7),1311+(i*5),1303+(i*3),2024);}
ps("\n");viti_report();viti_state();ps("\n=== Demo Complete ===\n");return 0;}
