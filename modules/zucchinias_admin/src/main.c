/* zucchinias_admin: Zucchinias management technology administration (v1.0)
 * Zucchinias planning, zucchinias execution, zucchinias evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zucc_t;
typedef struct{int n_zuccp,n_zucc,n_zucc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zucc_state_t;
static zucc_t zucs[N],zuce[N-2],zuccv[N-4],zuccac[N-6],zuccm[N-6]; static zucc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zucc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zucc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZUCC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zucc_init(void){if(init)return -1;st.n_zuccp=0;st.n_zucc=0;st.n_zucc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zucs[i].active=0;for(int i=0;i<N-2;i++)zuce[i].active=0;for(int i=0;i<N-4;i++)zuccv[i].active=0;for(int i=0;i<N-6;i++)zuccac[i].active=0;for(int i=0;i<N-6;i++)zuccm[i].active=0;init=1;ps("[ZUCC] Zucchinias initialized\n");return 0;}
int zucc_planning(int t,int c,int a,int b,int d,int e,int y){return add(zucs,&st.n_zuccp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zucc_execution(int t,int c,int a,int b,int d,int e,int y){return add(zuce,&st.n_zucc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zucc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zuccv,&st.n_zucc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zucc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zuccac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zucc_market(int t,int c,int a,int b,int d,int e,int y){return add(zuccm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zucc_report(void){ps("[ZUCC] Zuccp: ");pi(st.n_zuccp);ps(" PCS=");pi(st.t_f1);ps("\nZuce: ");pi(st.n_zucc);ps(" PCS=");pi(st.t_f2);ps("\nZuccv: ");pi(st.n_zucc2);ps(" PCS=");pi(st.t_f3);ps("\nZuccac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zucc_state(void){ps("[ZUCC] Zuccp=");pi(st.n_zuccp);ps(" Zuce=");pi(st.n_zucc);ps(" Zuccv=");pi(st.n_zucc2);ps(" Zuccac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zucchinias Admin Demo ===\n\n");zucc_init();
ps("Zucchinias planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zucc_planning(t,c,1383+(i*17),1372+(i*14),1352+(i*10),1334+(i*6),2020+(i%5));}
ps("\nZucchinias execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zucc_execution(t,c,1372+(i*15),1361+(i*12),1343+(i*8),1330+(i*5),2021+(i%4));}
ps("\nZucchinias evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zucc_evaluation(t,c,1364+(i*13),1353+(i*10),1337+(i*7),1326+(i*4),2022+(i%3));}
ps("\nZucchinias accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zucc_accessory(t,c,1356+(i*11),1347+(i*9),1333+(i*6),1323+(i*3),2023+(i%2));}
ps("\nZucchinias marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zucc_market(t,c,1350+(i*9),1341+(i*7),1328+(i*5),1320+(i*3),2024);}
ps("\n");zucc_report();zucc_state();ps("\n=== Demo Complete ===\n");return 0;}
