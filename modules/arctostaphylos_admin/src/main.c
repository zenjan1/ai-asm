/* arctostaphylos_admin: Arctostaphylos management technology administration (v1.0)
 * Arctostaphylos planning, arctostaphylos execution, arctostaphylos evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} arcs_t;
typedef struct{int n_arcsp,n_arcs,n_arcs2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} arcs_state_t;
static arcs_t arcss[N],arcses[N-2],arcs2[N-4],arcsac[N-6],arcsam[N-6]; static arcs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(arcs_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;arcs_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARCS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int arcs_init(void){if(init)return -1;st.n_arcsp=0;st.n_arcs=0;st.n_arcs2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)arcss[i].active=0;for(int i=0;i<N-2;i++)arcses[i].active=0;for(int i=0;i<N-4;i++)arcs2[i].active=0;for(int i=0;i<N-6;i++)arcsac[i].active=0;for(int i=0;i<N-6;i++)arcsam[i].active=0;init=1;ps("[ARCS] Arctostaphylos initialized\n");return 0;}
int arcs_planning(int t,int c,int a,int b,int d,int e,int y){return add(arcss,&st.n_arcsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int arcs_execution(int t,int c,int a,int b,int d,int e,int y){return add(arcses,&st.n_arcs,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int arcs_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(arcs2,&st.n_arcs2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int arcs_accessory(int t,int c,int a,int b,int d,int e,int y){return add(arcsac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int arcs_market(int t,int c,int a,int b,int d,int e,int y){return add(arcsam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void arcs_report(void){ps("[ARCS] Csp: ");pi(st.n_arcsp);ps(" PCS=");pi(st.t_f1);ps("\nCses: ");pi(st.n_arcs);ps(" PCS=");pi(st.t_f2);ps("\nCs2: ");pi(st.n_arcs2);ps(" PCS=");pi(st.t_f3);ps("\nCsac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void arcs_state(void){ps("[ARCS] Csp=");pi(st.n_arcsp);ps(" Cses=");pi(st.n_arcs);ps(" Cs2=");pi(st.n_arcs2);ps(" Csac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Arctostaphylos Admin Demo ===\n\n");arcs_init();
ps("Arctostaphylos planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;arcs_planning(t,c,1414+(i*17),1403+(i*14),1383+(i*10),1365+(i*6),2020+(i%5));}
ps("\nArctostaphylos execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;arcs_execution(t,c,1403+(i*15),1392+(i*12),1374+(i*8),1361+(i*5),2021+(i%4));}
ps("\nArctostaphylos evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;arcs_evaluation(t,c,1395+(i*13),1384+(i*10),1368+(i*7),1357+(i*4),2022+(i%3));}
ps("\nArctostaphylos accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arcs_accessory(t,c,1387+(i*11),1378+(i*9),1364+(i*6),1354+(i*3),2023+(i%2));}
ps("\nArctostaphylos marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arcs_market(t,c,1381+(i*9),1372+(i*7),1359+(i*5),1351+(i*3),2024);}
ps("\n");arcs_report();arcs_state();ps("\n=== Demo Complete ===\n");return 0;}
