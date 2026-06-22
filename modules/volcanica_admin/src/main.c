/* volcanica_admin: Volcanica management technology administration (v1.0)
 * Volcanica planning, volcanica execution, volcanica evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} volc_t;
typedef struct{int n_volcp,n_volc,n_volc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} volc_state_t;
static volc_t volcs[N],volce[N-2],volcv[N-4],volcac[N-6],volcm[N-6]; static volc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(volc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;volc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VOLC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int volc_init(void){if(init)return -1;st.n_volcp=0;st.n_volc=0;st.n_volc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)volcs[i].active=0;for(int i=0;i<N-2;i++)volce[i].active=0;for(int i=0;i<N-4;i++)volcv[i].active=0;for(int i=0;i<N-6;i++)volcac[i].active=0;for(int i=0;i<N-6;i++)volcm[i].active=0;init=1;ps("[VOLC] Volcanica initialized\n");return 0;}
int volc_planning(int t,int c,int a,int b,int d,int e,int y){return add(volcs,&st.n_volcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int volc_execution(int t,int c,int a,int b,int d,int e,int y){return add(volce,&st.n_volc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int volc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(volcv,&st.n_volc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int volc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(volcac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int volc_market(int t,int c,int a,int b,int d,int e,int y){return add(volcm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void volc_report(void){ps("[VOLC] Volcp: ");pi(st.n_volcp);ps(" PCS=");pi(st.t_f1);ps("\nVolce: ");pi(st.n_volc);ps(" PCS=");pi(st.t_f2);ps("\nVolcv: ");pi(st.n_volc2);ps(" PCS=");pi(st.t_f3);ps("\nVolcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void volc_state(void){ps("[VOLC] Volcp=");pi(st.n_volcp);ps(" Volce=");pi(st.n_volc);ps(" Volcv=");pi(st.n_volc2);ps(" Volcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Volcanica Admin Demo ===\n\n");volc_init();
ps("Volcanica planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;volc_planning(t,c,1370+(i*17),1359+(i*14),1339+(i*10),1321+(i*6),2020+(i%5));}
ps("\nVolcanica execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;volc_execution(t,c,1359+(i*15),1348+(i*12),1330+(i*8),1317+(i*5),2021+(i%4));}
ps("\nVolcanica evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;volc_evaluation(t,c,1351+(i*13),1340+(i*10),1324+(i*7),1313+(i*4),2022+(i%3));}
ps("\nVolcanica accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;volc_accessory(t,c,1343+(i*11),1334+(i*9),1320+(i*6),1310+(i*3),2023+(i%2));}
ps("\nVolcanica marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;volc_market(t,c,1337+(i*9),1328+(i*7),1315+(i*5),1307+(i*3),2024);}
ps("\n");volc_report();volc_state();ps("\n=== Demo Complete ===\n");return 0;}
