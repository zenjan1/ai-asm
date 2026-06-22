/* acalypha_admin: Acalypha management technology administration (v1.0)
 * Acalypha planning, acalypha execution, acalypha evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acl_t;
typedef struct{int n_aclp,n_acle,n_acl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acl_state_t;
static acl_t aclps[N],aclss[N-2],aclvss[N-4],aclas[N-6],aclmks[N-6]; static acl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(acl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acl_init(void){if(init)return -1;st.n_aclp=0;st.n_acle=0;st.n_acl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aclps[i].active=0;for(int i=0;i<N-2;i++)aclss[i].active=0;for(int i=0;i<N-4;i++)aclvss[i].active=0;for(int i=0;i<N-6;i++)aclas[i].active=0;for(int i=0;i<N-6;i++)aclmks[i].active=0;init=1;ps("[ACL] Acalypha initialized\n");return 0;}
int acl_planning(int t,int c,int a,int b,int d,int e,int y){return add(aclps,&st.n_aclp,&st.t_f1,N,t,c,a,b,d,e,y);}
int acl_execution(int t,int c,int a,int b,int d,int e,int y){return add(aclss,&st.n_acle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aclvss,&st.n_acl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aclas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acl_market(int t,int c,int a,int b,int d,int e,int y){return add(aclmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acl_report(void){ps("[ACL] Acapp: ");pi(st.n_aclp);ps(" PCS=");pi(st.t_f1);ps("\nAcape: ");pi(st.n_acle);ps(" PCS=");pi(st.t_f2);ps("\nAcav: ");pi(st.n_acl2);ps(" PCS=");pi(st.t_f3);ps("\nAcac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acl_state(void){ps("[ACL] Acapp=");pi(st.n_aclp);ps(" Acape=");pi(st.n_acle);ps(" Acav=");pi(st.n_acl2);ps(" Acac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Acalypha Admin Demo ===\n\n");acl_init();
ps("Acalypha planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acl_planning(t,c,1210+(i*17),1199+(i*14),1179+(i*10),1161+(i*6),2020+(i%5));}
ps("\nAcalypha execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acl_execution(t,c,1199+(i*15),1188+(i*12),1170+(i*8),1157+(i*5),2021+(i%4));}
ps("\nAcalypha evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acl_evaluation(t,c,1191+(i*13),1180+(i*10),1164+(i*7),1153+(i*4),2022+(i%3));}
ps("\nAcalypha accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acl_accessory(t,c,1183+(i*11),1174+(i*9),1160+(i*6),1150+(i*3),2023+(i%2));}
ps("\nAcalypha marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acl_market(t,c,1177+(i*9),1168+(i*7),1155+(i*5),1147+(i*3),2024);}
ps("\n");acl_report();acl_state();ps("\n=== Demo Complete ===\n");return 0;}
