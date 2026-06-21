/* sansevieria_admin: Sansevieria management technology administration (v1.0)
 * Sansevieria planning, sansevieria execution, sansevieria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sv_t;
typedef struct{int n_svp,n_sve,n_svv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sv_state_t;
static sv_t svps[N],svss[N-2],svvs[N-4],svcs[N-6],svmks[N-6]; static sv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SSV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sv_init(void){if(init)return -1;st.n_svp=0;st.n_sve=0;st.n_svv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)svps[i].active=0;for(int i=0;i<N-2;i++)svss[i].active=0;for(int i=0;i<N-4;i++)svvs[i].active=0;for(int i=0;i<N-6;i++)svcs[i].active=0;for(int i=0;i<N-6;i++)svmks[i].active=0;init=1;ps("[SSV] Sansevieria initialized\n");return 0;}
int sv_planning(int t,int c,int a,int b,int d,int e,int y){return add(svps,&st.n_svp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sv_execution(int t,int c,int a,int b,int d,int e,int y){return add(svss,&st.n_sve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sv_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(svvs,&st.n_svv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(svcs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sv_market(int t,int c,int a,int b,int d,int e,int y){return add(svmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sv_report(void){ps("[SSV] Svp: ");pi(st.n_svp);ps(" PCS=");pi(st.t_f1);ps("\nSve: ");pi(st.n_sve);ps(" PCS=");pi(st.t_f2);ps("\nSvv: ");pi(st.n_svv);ps(" PCS=");pi(st.t_f3);ps("\nSvc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sv_state(void){ps("[SSV] Svp=");pi(st.n_svp);ps(" Sve=");pi(st.n_sve);ps(" Svv=");pi(st.n_svv);ps(" Svc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sansevieria Admin Demo ===\n\n");sv_init();
ps("Sansevieria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sv_planning(t,c,640+(i*17),629+(i*14),609+(i*10),591+(i*6),2020+(i%5));}
ps("\nSansevieria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sv_execution(t,c,629+(i*15),618+(i*12),600+(i*8),587+(i*5),2021+(i%4));}
ps("\nSansevieria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sv_evaluation(t,c,621+(i*13),610+(i*10),594+(i*7),583+(i*4),2022+(i%3));}
ps("\nSansevieria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sv_accessory(t,c,613+(i*11),604+(i*9),590+(i*6),580+(i*3),2023+(i%2));}
ps("\nSansevieria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sv_market(t,c,607+(i*9),598+(i*7),585+(i*5),577+(i*3),2024);}
ps("\n");sv_report();sv_state();ps("\n=== Demo Complete ===\n");return 0;}
