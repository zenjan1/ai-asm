/* dendromecon_admin: Dendromecon management technology administration (v1.0)
 * Dendromecon planning, dendromecon execution, dendromecon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dend_t;
typedef struct{int n_dendp,n_dend,n_dend2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dend_state_t;
static dend_t dends[N],dende[N-2],dend2[N-4],dendac[N-6],dendam[N-6]; static dend_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dend_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dend_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DEND] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dend_init(void){if(init)return -1;st.n_dendp=0;st.n_dend=0;st.n_dend2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dends[i].active=0;for(int i=0;i<N-2;i++)dende[i].active=0;for(int i=0;i<N-4;i++)dend2[i].active=0;for(int i=0;i<N-6;i++)dendac[i].active=0;for(int i=0;i<N-6;i++)dendam[i].active=0;init=1;ps("[DEND] Dendromecon initialized\n");return 0;}
int dend_planning(int t,int c,int a,int b,int d,int e,int y){return add(dends,&st.n_dendp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dend_execution(int t,int c,int a,int b,int d,int e,int y){return add(dende,&st.n_dend,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dend_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dend2,&st.n_dend2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dend_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dendac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dend_market(int t,int c,int a,int b,int d,int e,int y){return add(dendam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dend_report(void){ps("[DEND] Ndp: ");pi(st.n_dendp);ps(" PCS=");pi(st.t_f1);ps("\nNde: ");pi(st.n_dend);ps(" PCS=");pi(st.t_f2);ps("\nNd2: ");pi(st.n_dend2);ps(" PCS=");pi(st.t_f3);ps("\nDac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dend_state(void){ps("[DEND] Ndp=");pi(st.n_dendp);ps(" Nd=");pi(st.n_dend);ps(" Nd2=");pi(st.n_dend2);ps(" Dac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dendromecon Admin Demo ===\n\n");dend_init();
ps("Dendromecon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dend_planning(t,c,1494+(i*17),1483+(i*14),1463+(i*10),1445+(i*6),2020+(i%5));}
ps("\nDendromecon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dend_execution(t,c,1483+(i*15),1472+(i*12),1454+(i*8),1441+(i*5),2021+(i%4));}
ps("\nDendromecon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dend_evaluation(t,c,1475+(i*13),1464+(i*10),1448+(i*7),1437+(i*4),2022+(i%3));}
ps("\nDendromecon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dend_accessory(t,c,1467+(i*11),1458+(i*9),1444+(i*6),1434+(i*3),2023+(i%2));}
ps("\nDendromecon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dend_market(t,c,1461+(i*9),1452+(i*7),1439+(i*5),1431+(i*3),2024);}
ps("\n");dend_report();dend_state();ps("\n=== Demo Complete ===\n");return 0;}
