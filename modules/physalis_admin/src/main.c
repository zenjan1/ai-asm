/* physalis_admin: Physalis management technology administration (v1.0)
 * Physalis planning, physalis execution, physalis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} phys_t;
typedef struct{int n_physp,n_physe,n_phys2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} phys_state_t;
static phys_t physjs[N],physses[N-2],physvss[N-4],physas[N-6],physmks[N-6]; static phys_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(phys_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;phys_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PHYS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int phys_init(void){if(init)return -1;st.n_physp=0;st.n_physe=0;st.n_phys2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)physjs[i].active=0;for(int i=0;i<N-2;i++)physses[i].active=0;for(int i=0;i<N-4;i++)physvss[i].active=0;for(int i=0;i<N-6;i++)physas[i].active=0;for(int i=0;i<N-6;i++)physmks[i].active=0;init=1;ps("[PHYS] Physalis initialized\n");return 0;}
int phys_planning(int t,int c,int a,int b,int d,int e,int y){return add(physjs,&st.n_physp,&st.t_f1,N,t,c,a,b,d,e,y);}
int phys_execution(int t,int c,int a,int b,int d,int e,int y){return add(physses,&st.n_physe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int phys_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(physvss,&st.n_phys2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int phys_accessory(int t,int c,int a,int b,int d,int e,int y){return add(physas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int phys_market(int t,int c,int a,int b,int d,int e,int y){return add(physmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void phys_report(void){ps("[PHYS] Physp: ");pi(st.n_physp);ps(" PCS=");pi(st.t_f1);ps("\nPhyse: ");pi(st.n_physe);ps(" PCS=");pi(st.t_f2);ps("\nPhysv: ");pi(st.n_phys2);ps(" PCS=");pi(st.t_f3);ps("\nPhysc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void phys_state(void){ps("[PHYS] Physp=");pi(st.n_physp);ps(" Physe=");pi(st.n_physe);ps(" Physv=");pi(st.n_phys2);ps(" Physc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Physalis Admin Demo ===\n\n");phys_init();
ps("Physalis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;phys_planning(t,c,1328+(i*17),1317+(i*14),1297+(i*10),1279+(i*6),2020+(i%5));}
ps("\nPhysalis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;phys_execution(t,c,1317+(i*15),1306+(i*12),1288+(i*8),1275+(i*5),2021+(i%4));}
ps("\nPhysalis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;phys_evaluation(t,c,1309+(i*13),1298+(i*10),1282+(i*7),1271+(i*4),2022+(i%3));}
ps("\nPhysalis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phys_accessory(t,c,1301+(i*11),1292+(i*9),1278+(i*6),1268+(i*3),2023+(i%2));}
ps("\nPhysalis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phys_market(t,c,1295+(i*9),1286+(i*7),1273+(i*5),1265+(i*3),2024);}
ps("\n");phys_report();phys_state();ps("\n=== Demo Complete ===\n");return 0;}
