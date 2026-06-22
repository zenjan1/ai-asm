/* aconiti_admin: Aconiti management technology administration (v1.0)
 * Aconiti planning, aconiti execution, aconiti evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acon_t;
typedef struct{int n_aconp,n_acon,n_acon2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acon_state_t;
static acon_t acons[N],acone[N-2],aconv[N-4],aconac[N-6],aconm[N-6]; static acon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(acon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acon_init(void){if(init)return -1;st.n_aconp=0;st.n_acon=0;st.n_acon2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acons[i].active=0;for(int i=0;i<N-2;i++)acone[i].active=0;for(int i=0;i<N-4;i++)aconv[i].active=0;for(int i=0;i<N-6;i++)aconac[i].active=0;for(int i=0;i<N-6;i++)aconm[i].active=0;init=1;ps("[ACON] Aconiti initialized\n");return 0;}
int acon_planning(int t,int c,int a,int b,int d,int e,int y){return add(acons,&st.n_aconp,&st.t_f1,N,t,c,a,b,d,e,y);}
int acon_execution(int t,int c,int a,int b,int d,int e,int y){return add(acone,&st.n_acon,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acon_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aconv,&st.n_acon2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acon_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aconac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acon_market(int t,int c,int a,int b,int d,int e,int y){return add(aconm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acon_report(void){ps("[ACON] Conp: ");pi(st.n_aconp);ps(" PCS=");pi(st.t_f1);ps("\nCone: ");pi(st.n_acon);ps(" PCS=");pi(st.t_f2);ps("\nConv: ");pi(st.n_acon2);ps(" PCS=");pi(st.t_f3);ps("\nConac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acon_state(void){ps("[ACON] Conp=");pi(st.n_aconp);ps(" Cone=");pi(st.n_acon);ps(" Conv=");pi(st.n_acon2);ps(" Conac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aconiti Admin Demo ===\n\n");acon_init();
ps("Aconiti planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acon_planning(t,c,1390+(i*17),1379+(i*14),1359+(i*10),1341+(i*6),2020+(i%5));}
ps("\nAconiti execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acon_execution(t,c,1379+(i*15),1368+(i*12),1350+(i*8),1337+(i*5),2021+(i%4));}
ps("\nAconiti evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acon_evaluation(t,c,1371+(i*13),1360+(i*10),1344+(i*7),1333+(i*4),2022+(i%3));}
ps("\nAconiti accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acon_accessory(t,c,1363+(i*11),1354+(i*9),1340+(i*6),1330+(i*3),2023+(i%2));}
ps("\nAconiti marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acon_market(t,c,1357+(i*9),1348+(i*7),1335+(i*5),1327+(i*3),2024);}
ps("\n");acon_report();acon_state();ps("\n=== Demo Complete ===\n");return 0;}
