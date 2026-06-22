/* castanea_admin: Castanea management technology administration (v1.0)
 * Castanea planning, castanea execution, castanea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cast_t;
typedef struct{int n_castp,n_cast,n_cast2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cast_state_t;
static cast_t casts[N],caste[N-2],cast2[N-4],castac[N-6],castam[N-6]; static cast_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cast_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cast_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cast_init(void){if(init)return -1;st.n_castp=0;st.n_cast=0;st.n_cast2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)casts[i].active=0;for(int i=0;i<N-2;i++)caste[i].active=0;for(int i=0;i<N-4;i++)cast2[i].active=0;for(int i=0;i<N-6;i++)castac[i].active=0;for(int i=0;i<N-6;i++)castam[i].active=0;init=1;ps("[CAST] Castanea initialized\n");return 0;}
int cast_planning(int t,int c,int a,int b,int d,int e,int y){return add(casts,&st.n_castp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cast_execution(int t,int c,int a,int b,int d,int e,int y){return add(caste,&st.n_cast,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cast_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cast2,&st.n_cast2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cast_accessory(int t,int c,int a,int b,int d,int e,int y){return add(castac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cast_market(int t,int c,int a,int b,int d,int e,int y){return add(castam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cast_report(void){ps("[CAST] Astp: ");pi(st.n_castp);ps(" PCS=");pi(st.t_f1);ps("\nAste: ");pi(st.n_cast);ps(" PCS=");pi(st.t_f2);ps("\nAst2: ");pi(st.n_cast2);ps(" PCS=");pi(st.t_f3);ps("\nAstac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cast_state(void){ps("[CAST] Astp=");pi(st.n_castp);ps(" Ast=");pi(st.n_cast);ps(" Ast2=");pi(st.n_cast2);ps(" Astac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Castanea Admin Demo ===\n\n");cast_init();
ps("Castanea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cast_planning(t,c,1464+(i*17),1453+(i*14),1433+(i*10),1415+(i*6),2020+(i%5));}
ps("\nCastanea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cast_execution(t,c,1453+(i*15),1442+(i*12),1424+(i*8),1411+(i*5),2021+(i%4));}
ps("\nCastanea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cast_evaluation(t,c,1445+(i*13),1434+(i*10),1418+(i*7),1407+(i*4),2022+(i%3));}
ps("\nCastanea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cast_accessory(t,c,1437+(i*11),1428+(i*9),1414+(i*6),1404+(i*3),2023+(i%2));}
ps("\nCastanea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cast_market(t,c,1431+(i*9),1422+(i*7),1409+(i*5),1401+(i*3),2024);}
ps("\n");cast_report();cast_state();ps("\n=== Demo Complete ===\n");return 0;}
