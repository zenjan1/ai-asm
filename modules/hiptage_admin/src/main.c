/* hiptage_admin: Hiptage management technology administration (v1.0)
 * Hiptage planning, hiptage execution, hiptage evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hip_t;
typedef struct{int n_hipp,n_hipe,n_hip2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hip_state_t;
static hip_t hipps[N],hipss[N-2],hipvss[N-4],hipas[N-6],hipmks[N-6]; static hip_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hip_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hip_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HIP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hip_init(void){if(init)return -1;st.n_hipp=0;st.n_hipe=0;st.n_hip2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hipps[i].active=0;for(int i=0;i<N-2;i++)hipss[i].active=0;for(int i=0;i<N-4;i++)hipvss[i].active=0;for(int i=0;i<N-6;i++)hipas[i].active=0;for(int i=0;i<N-6;i++)hipmks[i].active=0;init=1;ps("[HIP] Hiptage initialized\n");return 0;}
int hip_planning(int t,int c,int a,int b,int d,int e,int y){return add(hipps,&st.n_hipp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hip_execution(int t,int c,int a,int b,int d,int e,int y){return add(hipss,&st.n_hipe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hip_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hipvss,&st.n_hip2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hip_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hipas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hip_market(int t,int c,int a,int b,int d,int e,int y){return add(hipmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hip_report(void){ps("[HIP] Hipp: ");pi(st.n_hipp);ps(" PCS=");pi(st.t_f1);ps("\nHipe: ");pi(st.n_hipe);ps(" PCS=");pi(st.t_f2);ps("\nHipv: ");pi(st.n_hip2);ps(" PCS=");pi(st.t_f3);ps("\nHipc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hip_state(void){ps("[HIP] Hipp=");pi(st.n_hipp);ps(" Hipe=");pi(st.n_hipe);ps(" Hipv=");pi(st.n_hip2);ps(" Hipc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hiptage Admin Demo ===\n\n");hip_init();
ps("Hiptage planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hip_planning(t,c,1292+(i*17),1281+(i*14),1261+(i*10),1243+(i*6),2020+(i%5));}
ps("\nHiptage execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hip_execution(t,c,1281+(i*15),1270+(i*12),1252+(i*8),1239+(i*5),2021+(i%4));}
ps("\nHiptage evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hip_evaluation(t,c,1273+(i*13),1262+(i*10),1246+(i*7),1235+(i*4),2022+(i%3));}
ps("\nHiptage accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hip_accessory(t,c,1265+(i*11),1256+(i*9),1242+(i*6),1232+(i*3),2023+(i%2));}
ps("\nHiptage marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hip_market(t,c,1259+(i*9),1250+(i*7),1237+(i*5),1229+(i*3),2024);}
ps("\n");hip_report();hip_state();ps("\n=== Demo Complete ===\n");return 0;}
