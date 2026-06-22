/* anathallis_admin: Anathallis management technology administration (v1.0)
 * Anathallis planning, anathallis execution, anathallis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} anat_t;
typedef struct{int n_anatp,n_anat,n_anat2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} anat_state_t;
static anat_t anats[N],anate[N-2],anat2[N-4],anatac[N-6],anatam[N-6]; static anat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(anat_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;anat_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANAT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int anat_init(void){if(init)return -1;st.n_anatp=0;st.n_anat=0;st.n_anat2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)anats[i].active=0;for(int i=0;i<N-2;i++)anate[i].active=0;for(int i=0;i<N-4;i++)anat2[i].active=0;for(int i=0;i<N-6;i++)anatac[i].active=0;for(int i=0;i<N-6;i++)anatam[i].active=0;init=1;ps("[ANAT] Anathallis initialized\n");return 0;}
int anat_planning(int t,int c,int a,int b,int d,int e,int y){return add(anats,&st.n_anatp,&st.t_f1,N,t,c,a,b,d,e,y);}
int anat_execution(int t,int c,int a,int b,int d,int e,int y){return add(anate,&st.n_anat,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int anat_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(anat2,&st.n_anat2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int anat_accessory(int t,int c,int a,int b,int d,int e,int y){return add(anatac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int anat_market(int t,int c,int a,int b,int d,int e,int y){return add(anatam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void anat_report(void){ps("[ANAT] Natp: ");pi(st.n_anatp);ps(" PCS=");pi(st.t_f1);ps("\nNate: ");pi(st.n_anat);ps(" PCS=");pi(st.t_f2);ps("\nNat2: ");pi(st.n_anat2);ps(" PCS=");pi(st.t_f3);ps("\nNatac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void anat_state(void){ps("[ANAT] Natp=");pi(st.n_anatp);ps(" Nate=");pi(st.n_anat);ps(" Nat2=");pi(st.n_anat2);ps(" Natac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Anathallis Admin Demo ===\n\n");anat_init();
ps("Anathallis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;anat_planning(t,c,1408+(i*17),1397+(i*14),1377+(i*10),1359+(i*6),2020+(i%5));}
ps("\nAnathallis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;anat_execution(t,c,1397+(i*15),1386+(i*12),1368+(i*8),1355+(i*5),2021+(i%4));}
ps("\nAnathallis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;anat_evaluation(t,c,1389+(i*13),1378+(i*10),1362+(i*7),1351+(i*4),2022+(i%3));}
ps("\nAnathallis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anat_accessory(t,c,1381+(i*11),1372+(i*9),1358+(i*6),1348+(i*3),2023+(i%2));}
ps("\nAnathallis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anat_market(t,c,1375+(i*9),1366+(i*7),1353+(i*5),1345+(i*3),2024);}
ps("\n");anat_report();anat_state();ps("\n=== Demo Complete ===\n");return 0;}
