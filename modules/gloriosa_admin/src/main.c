/* gloriosa_admin: Gloriosa management technology administration (v1.0)
 * Gloriosa planning, gloriosa execution, gloriosa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} glor_t;
typedef struct{int n_glorp,n_glor,n_glor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} glor_state_t;
static glor_t glors[N],glore[N-2],glor2[N-4],glorac[N-6],gloram[N-6]; static glor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(glor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;glor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GLOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int glor_init(void){if(init)return -1;st.n_glorp=0;st.n_glor=0;st.n_glor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)glors[i].active=0;for(int i=0;i<N-2;i++)glore[i].active=0;for(int i=0;i<N-4;i++)glor2[i].active=0;for(int i=0;i<N-6;i++)glorac[i].active=0;for(int i=0;i<N-6;i++)gloram[i].active=0;init=1;ps("[GLOR] Gloriosa initialized\n");return 0;}
int glor_planning(int t,int c,int a,int b,int d,int e,int y){return add(glors,&st.n_glorp,&st.t_f1,N,t,c,a,b,d,e,y);}
int glor_execution(int t,int c,int a,int b,int d,int e,int y){return add(glore,&st.n_glor,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int glor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(glor2,&st.n_glor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int glor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(glorac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int glor_market(int t,int c,int a,int b,int d,int e,int y){return add(gloram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void glor_report(void){ps("[GLOR] Glorp: ");pi(st.n_glorp);ps(" PCS=");pi(st.t_f1);ps("\nGlore: ");pi(st.n_glor);ps(" PCS=");pi(st.t_f2);ps("\nGlor2: ");pi(st.n_glor2);ps(" PCS=");pi(st.t_f3);ps("\nGloac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void glor_state(void){ps("[GLOR] Glorp=");pi(st.n_glorp);ps(" Glor=");pi(st.n_glor);ps(" Glor2=");pi(st.n_glor2);ps(" Gloac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gloriosa Admin Demo ===\n\n");glor_init();
ps("Gloriosa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;glor_planning(t,c,1517+(i*17),1506+(i*14),1486+(i*10),1468+(i*6),2020+(i%5));}
ps("\nGloriosa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;glor_execution(t,c,1506+(i*15),1495+(i*12),1477+(i*8),1464+(i*5),2021+(i%4));}
ps("\nGloriosa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;glor_evaluation(t,c,1498+(i*13),1487+(i*10),1471+(i*7),1460+(i*4),2022+(i%3));}
ps("\nGloriosa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;glor_accessory(t,c,1490+(i*11),1481+(i*9),1467+(i*6),1457+(i*3),2023+(i%2));}
ps("\nGloriosa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;glor_market(t,c,1484+(i*9),1475+(i*7),1462+(i*5),1454+(i*3),2024);}
ps("\n");glor_report();glor_state();ps("\n=== Demo Complete ===\n");return 0;}
