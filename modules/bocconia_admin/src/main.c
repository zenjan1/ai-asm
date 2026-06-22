/* bocconia_admin: Bocconia management technology administration (v1.0)
 * Bocconia planning, bocconia execution, bocconia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} boc_t;
typedef struct{int n_bocp,n_boce,n_boc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} boc_state_t;
static boc_t bocps[N],bocss[N-2],bocvss[N-4],bocas[N-6],bocmks[N-6]; static boc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(boc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;boc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BOC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int boc_init(void){if(init)return -1;st.n_bocp=0;st.n_boce=0;st.n_boc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bocps[i].active=0;for(int i=0;i<N-2;i++)bocss[i].active=0;for(int i=0;i<N-4;i++)bocvss[i].active=0;for(int i=0;i<N-6;i++)bocas[i].active=0;for(int i=0;i<N-6;i++)bocmks[i].active=0;init=1;ps("[BOC] Bocconia initialized\n");return 0;}
int boc_planning(int t,int c,int a,int b,int d,int e,int y){return add(bocps,&st.n_bocp,&st.t_f1,N,t,c,a,b,d,e,y);}
int boc_execution(int t,int c,int a,int b,int d,int e,int y){return add(bocss,&st.n_boce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int boc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bocvss,&st.n_boc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int boc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bocas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int boc_market(int t,int c,int a,int b,int d,int e,int y){return add(bocmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void boc_report(void){ps("[BOC] Bocpp: ");pi(st.n_bocp);ps(" PCS=");pi(st.t_f1);ps("\nBoce: ");pi(st.n_boce);ps(" PCS=");pi(st.t_f2);ps("\nBocv: ");pi(st.n_boc2);ps(" PCS=");pi(st.t_f3);ps("\nBocc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void boc_state(void){ps("[BOC] Bocpp=");pi(st.n_bocp);ps(" Boce=");pi(st.n_boce);ps(" Bocv=");pi(st.n_boc2);ps(" Bocc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bocconia Admin Demo ===\n\n");boc_init();
ps("Bocconia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;boc_planning(t,c,1124+(i*17),1113+(i*14),1093+(i*10),1075+(i*6),2020+(i%5));}
ps("\nBocconia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;boc_execution(t,c,1113+(i*15),1102+(i*12),1084+(i*8),1071+(i*5),2021+(i%4));}
ps("\nBocconia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;boc_evaluation(t,c,1105+(i*13),1094+(i*10),1078+(i*7),1067+(i*4),2022+(i%3));}
ps("\nBocconia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;boc_accessory(t,c,1097+(i*11),1088+(i*9),1074+(i*6),1064+(i*3),2023+(i%2));}
ps("\nBocconia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;boc_market(t,c,1091+(i*9),1082+(i*7),1069+(i*5),1061+(i*3),2024);}
ps("\n");boc_report();boc_state();ps("\n=== Demo Complete ===\n");return 0;}
