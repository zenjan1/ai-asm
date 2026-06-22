/* heptacodium_admin: Heptacodium management technology administration (v1.0)
 * Heptacodium planning, heptacodium execution, heptacodium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hept_t;
typedef struct{int n_heptp,n_hept,n_hept2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hept_state_t;
static hept_t hepts[N],hepte[N-2],hept2[N-4],heptac[N-6],heptam[N-6]; static hept_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hept_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hept_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HEPT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hept_init(void){if(init)return -1;st.n_heptp=0;st.n_hept=0;st.n_hept2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hepts[i].active=0;for(int i=0;i<N-2;i++)hepte[i].active=0;for(int i=0;i<N-4;i++)hept2[i].active=0;for(int i=0;i<N-6;i++)heptac[i].active=0;for(int i=0;i<N-6;i++)heptam[i].active=0;init=1;ps("[HEPT] Heptacodium initialized\n");return 0;}
int hept_planning(int t,int c,int a,int b,int d,int e,int y){return add(hepts,&st.n_heptp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hept_execution(int t,int c,int a,int b,int d,int e,int y){return add(hepte,&st.n_hept,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hept_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hept2,&st.n_hept2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hept_accessory(int t,int c,int a,int b,int d,int e,int y){return add(heptac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hept_market(int t,int c,int a,int b,int d,int e,int y){return add(heptam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hept_report(void){ps("[HEPT] Heptp: ");pi(st.n_heptp);ps(" PCS=");pi(st.t_f1);ps("\nHepte: ");pi(st.n_hept);ps(" PCS=");pi(st.t_f2);ps("\nHept2: ");pi(st.n_hept2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hept_state(void){ps("[HEPT] Heptp=");pi(st.n_heptp);ps(" Hept=");pi(st.n_hept);ps(" Hept2=");pi(st.n_hept2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heptacodium Admin Demo ===\n\n");hept_init();
ps("Heptacodium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hept_planning(t,c,1528+(i*17),1517+(i*14),1497+(i*10),1479+(i*6),2020+(i%5));}
ps("\nHeptacodium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hept_execution(t,c,1517+(i*15),1506+(i*12),1488+(i*8),1475+(i*5),2021+(i%4));}
ps("\nHeptacodium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hept_evaluation(t,c,1509+(i*13),1498+(i*10),1482+(i*7),1471+(i*4),2022+(i%3));}
ps("\nHeptacodium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hept_accessory(t,c,1501+(i*11),1492+(i*9),1478+(i*6),1468+(i*3),2023+(i%2));}
ps("\nHeptacodium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hept_market(t,c,1495+(i*9),1486+(i*7),1473+(i*5),1465+(i*3),2024);}
ps("\n");hept_report();hept_state();ps("\n=== Demo Complete ===\n");return 0;}
