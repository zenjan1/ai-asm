/* nutmeg_admin: Nutmeg management technology administration (v1.0)
 * Nutmeg planning, nutmeg execution, nutmeg evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nm_t;
typedef struct{int n_nmp,n_nme,n_nmv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nm_state_t;
static nm_t nmps[N],nmss[N-2],nmvss[N-4],nmas[N-6],nmmks[N-6]; static nm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NTM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nm_init(void){if(init)return -1;st.n_nmp=0;st.n_nme=0;st.n_nmv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nmps[i].active=0;for(int i=0;i<N-2;i++)nmss[i].active=0;for(int i=0;i<N-4;i++)nmvss[i].active=0;for(int i=0;i<N-6;i++)nmas[i].active=0;for(int i=0;i<N-6;i++)nmmks[i].active=0;init=1;ps("[NTM] Nutmeg initialized\n");return 0;}
int nm_planning(int t,int c,int a,int b,int d,int e,int y){return add(nmps,&st.n_nmp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nm_execution(int t,int c,int a,int b,int d,int e,int y){return add(nmss,&st.n_nme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nmvss,&st.n_nmv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nmas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nm_market(int t,int c,int a,int b,int d,int e,int y){return add(nmmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nm_report(void){ps("[NTM] Nmp: ");pi(st.n_nmp);ps(" PCS=");pi(st.t_f1);ps("\nNme: ");pi(st.n_nme);ps(" PCS=");pi(st.t_f2);ps("\nNmv: ");pi(st.n_nmv);ps(" PCS=");pi(st.t_f3);ps("\nNmc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nm_state(void){ps("[NTM] Nmp=");pi(st.n_nmp);ps(" Nme=");pi(st.n_nme);ps(" Nmv=");pi(st.n_nmv);ps(" Nmc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nutmeg Admin Demo ===\n\n");nm_init();
ps("Nutmeg planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nm_planning(t,c,672+(i*17),661+(i*14),641+(i*10),623+(i*6),2020+(i%5));}
ps("\nNutmeg execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nm_execution(t,c,661+(i*15),650+(i*12),632+(i*8),619+(i*5),2021+(i%4));}
ps("\nNutmeg evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nm_evaluation(t,c,653+(i*13),642+(i*10),626+(i*7),615+(i*4),2022+(i%3));}
ps("\nNutmeg accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nm_accessory(t,c,645+(i*11),636+(i*9),622+(i*6),612+(i*3),2023+(i%2));}
ps("\nNutmeg marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nm_market(t,c,639+(i*9),630+(i*7),617+(i*5),609+(i*3),2024);}
ps("\n");nm_report();nm_state();ps("\n=== Demo Complete ===\n");return 0;}
