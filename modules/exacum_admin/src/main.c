/* exacum_admin: Exacum management technology administration (v1.0)
 * Exacum planning, exacum execution, exacum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} exac_t;
typedef struct{int n_exacp,n_exac,n_exac2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} exac_state_t;
static exac_t exacs[N],exace[N-2],exac2[N-4],exacac[N-6],exacam[N-6]; static exac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(exac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;exac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EXAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int exac_init(void){if(init)return -1;st.n_exacp=0;st.n_exac=0;st.n_exac2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)exacs[i].active=0;for(int i=0;i<N-2;i++)exace[i].active=0;for(int i=0;i<N-4;i++)exac2[i].active=0;for(int i=0;i<N-6;i++)exacac[i].active=0;for(int i=0;i<N-6;i++)exacam[i].active=0;init=1;ps("[EXAC] Exacum initialized\n");return 0;}
int exac_planning(int t,int c,int a,int b,int d,int e,int y){return add(exacs,&st.n_exacp,&st.t_f1,N,t,c,a,b,d,e,y);}
int exac_execution(int t,int c,int a,int b,int d,int e,int y){return add(exace,&st.n_exac,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int exac_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(exac2,&st.n_exac2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int exac_accessory(int t,int c,int a,int b,int d,int e,int y){return add(exacac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int exac_market(int t,int c,int a,int b,int d,int e,int y){return add(exacam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void exac_report(void){ps("[EXAC] Acp: ");pi(st.n_exacp);ps(" PCS=");pi(st.t_f1);ps("\nAce: ");pi(st.n_exac);ps(" PCS=");pi(st.t_f2);ps("\nAc2: ");pi(st.n_exac2);ps(" PCS=");pi(st.t_f3);ps("\nAac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void exac_state(void){ps("[EXAC] Acp=");pi(st.n_exacp);ps(" Ac=");pi(st.n_exac);ps(" Ac2=");pi(st.n_exac2);ps(" Aac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Exacum Admin Demo ===\n\n");exac_init();
ps("Exacum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;exac_planning(t,c,1509+(i*17),1498+(i*14),1478+(i*10),1460+(i*6),2020+(i%5));}
ps("\nExacum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;exac_execution(t,c,1498+(i*15),1487+(i*12),1469+(i*8),1456+(i*5),2021+(i%4));}
ps("\nExacum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;exac_evaluation(t,c,1490+(i*13),1479+(i*10),1463+(i*7),1452+(i*4),2022+(i%3));}
ps("\nExacum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;exac_accessory(t,c,1482+(i*11),1473+(i*9),1459+(i*6),1449+(i*3),2023+(i%2));}
ps("\nExacum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;exac_market(t,c,1476+(i*9),1467+(i*7),1454+(i*5),1446+(i*3),2024);}
ps("\n");exac_report();exac_state();ps("\n=== Demo Complete ===\n");return 0;}
