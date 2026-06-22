/* grevillea_admin: Grevillea management technology administration (v1.0)
 * Grevillea planning, grevillea execution, grevillea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} grev_t;
typedef struct{int n_grevp,n_grev,n_grev2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} grev_state_t;
static grev_t grevs[N],greve[N-2],grev2[N-4],grevac[N-6],grevam[N-6]; static grev_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(grev_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;grev_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GREV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int grev_init(void){if(init)return -1;st.n_grevp=0;st.n_grev=0;st.n_grev2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)grevs[i].active=0;for(int i=0;i<N-2;i++)greve[i].active=0;for(int i=0;i<N-4;i++)grev2[i].active=0;for(int i=0;i<N-6;i++)grevac[i].active=0;for(int i=0;i<N-6;i++)grevam[i].active=0;init=1;ps("[GREV] Grevillea initialized\n");return 0;}
int grev_planning(int t,int c,int a,int b,int d,int e,int y){return add(grevs,&st.n_grevp,&st.t_f1,N,t,c,a,b,d,e,y);}
int grev_execution(int t,int c,int a,int b,int d,int e,int y){return add(greve,&st.n_grev,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int grev_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(grev2,&st.n_grev2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int grev_accessory(int t,int c,int a,int b,int d,int e,int y){return add(grevac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int grev_market(int t,int c,int a,int b,int d,int e,int y){return add(grevam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void grev_report(void){ps("[GREV] Grevp: ");pi(st.n_grevp);ps(" PCS=");pi(st.t_f1);ps("\nGreve: ");pi(st.n_grev);ps(" PCS=");pi(st.t_f2);ps("\nGrev2: ");pi(st.n_grev2);ps(" PCS=");pi(st.t_f3);ps("\nGrevac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void grev_state(void){ps("[GREV] Grevp=");pi(st.n_grevp);ps(" Grev=");pi(st.n_grev);ps(" Grev2=");pi(st.n_grev2);ps(" Grevac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Grevillea Admin Demo ===\n\n");grev_init();
ps("Grevillea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;grev_planning(t,c,1521+(i*17),1510+(i*14),1490+(i*10),1472+(i*6),2020+(i%5));}
ps("\nGrevillea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;grev_execution(t,c,1510+(i*15),1499+(i*12),1481+(i*8),1468+(i*5),2021+(i%4));}
ps("\nGrevillea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;grev_evaluation(t,c,1502+(i*13),1491+(i*10),1475+(i*7),1464+(i*4),2022+(i%3));}
ps("\nGrevillea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grev_accessory(t,c,1494+(i*11),1485+(i*9),1471+(i*6),1461+(i*3),2023+(i%2));}
ps("\nGrevillea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grev_market(t,c,1488+(i*9),1479+(i*7),1466+(i*5),1458+(i*3),2024);}
ps("\n");grev_report();grev_state();ps("\n=== Demo Complete ===\n");return 0;}
