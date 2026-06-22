/* dysoxylon_admin: Dysoxylon management technology administration (v1.0)
 * Dysoxylon planning, dysoxylon execution, dysoxylon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dyso_t;
typedef struct{int n_dysop,n_dyso,n_dyso2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dyso_state_t;
static dyso_t dyson[N],dysoe[N-2],dyso2[N-4],dysoac[N-6],dysoam[N-6]; static dyso_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dyso_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dyso_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DYSO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dyso_init(void){if(init)return -1;st.n_dysop=0;st.n_dyso=0;st.n_dyso2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dyson[i].active=0;for(int i=0;i<N-2;i++)dysoe[i].active=0;for(int i=0;i<N-4;i++)dyso2[i].active=0;for(int i=0;i<N-6;i++)dysoac[i].active=0;for(int i=0;i<N-6;i++)dysoam[i].active=0;init=1;ps("[DYSO] Dysoxylon initialized\n");return 0;}
int dyso_planning(int t,int c,int a,int b,int d,int e,int y){return add(dyson,&st.n_dysop,&st.t_f1,N,t,c,a,b,d,e,y);}
int dyso_execution(int t,int c,int a,int b,int d,int e,int y){return add(dysoe,&st.n_dyso,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dyso_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dyso2,&st.n_dyso2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dyso_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dysoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dyso_market(int t,int c,int a,int b,int d,int e,int y){return add(dysoam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dyso_report(void){ps("[DYSO] Sop: ");pi(st.n_dysop);ps(" PCS=");pi(st.t_f1);ps("\nSoe: ");pi(st.n_dyso);ps(" PCS=");pi(st.t_f2);ps("\nSo2: ");pi(st.n_dyso2);ps(" PCS=");pi(st.t_f3);ps("\nSac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dyso_state(void){ps("[DYSO] Sop=");pi(st.n_dysop);ps(" So=");pi(st.n_dyso);ps(" So2=");pi(st.n_dyso2);ps(" Sac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dysoxylon Admin Demo ===\n\n");dyso_init();
ps("Dysoxylon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dyso_planning(t,c,1504+(i*17),1493+(i*14),1473+(i*10),1455+(i*6),2020+(i%5));}
ps("\nDysoxylon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dyso_execution(t,c,1493+(i*15),1482+(i*12),1464+(i*8),1451+(i*5),2021+(i%4));}
ps("\nDysoxylon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dyso_evaluation(t,c,1485+(i*13),1474+(i*10),1458+(i*7),1447+(i*4),2022+(i%3));}
ps("\nDysoxylon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dyso_accessory(t,c,1477+(i*11),1468+(i*9),1454+(i*6),1444+(i*3),2023+(i%2));}
ps("\nDysoxylon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dyso_market(t,c,1471+(i*9),1462+(i*7),1449+(i*5),1441+(i*3),2024);}
ps("\n");dyso_report();dyso_state();ps("\n=== Demo Complete ===\n");return 0;}
