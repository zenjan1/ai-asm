/* agrostis_admin: Agrostis management technology administration (v1.0)
 * Agrostis planning, agrostis execution, agrostis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} agro_t;
typedef struct{int n_agrop,n_agro,n_agro2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} agro_state_t;
static agro_t agros[N],agroe[N-2],agrov[N-4],agroac[N-6],agrom[N-6]; static agro_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(agro_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;agro_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AGRO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int agro_init(void){if(init)return -1;st.n_agrop=0;st.n_agro=0;st.n_agro2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)agros[i].active=0;for(int i=0;i<N-2;i++)agroe[i].active=0;for(int i=0;i<N-4;i++)agrov[i].active=0;for(int i=0;i<N-6;i++)agroac[i].active=0;for(int i=0;i<N-6;i++)agrom[i].active=0;init=1;ps("[AGRO] Agrostis initialized\n");return 0;}
int agro_planning(int t,int c,int a,int b,int d,int e,int y){return add(agros,&st.n_agrop,&st.t_f1,N,t,c,a,b,d,e,y);}
int agro_execution(int t,int c,int a,int b,int d,int e,int y){return add(agroe,&st.n_agro,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int agro_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(agrov,&st.n_agro2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int agro_accessory(int t,int c,int a,int b,int d,int e,int y){return add(agroac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int agro_market(int t,int c,int a,int b,int d,int e,int y){return add(agrom,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void agro_report(void){ps("[AGRO] Grop: ");pi(st.n_agrop);ps(" PCS=");pi(st.t_f1);ps("\nGroe: ");pi(st.n_agro);ps(" PCS=");pi(st.t_f2);ps("\nGrov: ");pi(st.n_agro2);ps(" PCS=");pi(st.t_f3);ps("\nGroac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void agro_state(void){ps("[AGRO] Grop=");pi(st.n_agrop);ps(" Groe=");pi(st.n_agro);ps(" Grov=");pi(st.n_agro2);ps(" Groac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Agrostis Admin Demo ===\n\n");agro_init();
ps("Agrostis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;agro_planning(t,c,1395+(i*17),1384+(i*14),1364+(i*10),1346+(i*6),2020+(i%5));}
ps("\nAgrostis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;agro_execution(t,c,1384+(i*15),1373+(i*12),1355+(i*8),1342+(i*5),2021+(i%4));}
ps("\nAgrostis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;agro_evaluation(t,c,1376+(i*13),1365+(i*10),1349+(i*7),1338+(i*4),2022+(i%3));}
ps("\nAgrostis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;agro_accessory(t,c,1368+(i*11),1359+(i*9),1345+(i*6),1335+(i*3),2023+(i%2));}
ps("\nAgrostis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;agro_market(t,c,1362+(i*9),1353+(i*7),1340+(i*5),1332+(i*3),2024);}
ps("\n");agro_report();agro_state();ps("\n=== Demo Complete ===\n");return 0;}
