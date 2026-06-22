/* mossia_admin: Mossia management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} moss_t;
typedef struct{int n_mossop,n_moss,n_moss2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} moss_state_t;
static moss_t mossos[N],mosse[N-2],moss2[N-4],mossac[N-6],mossam[N-6]; static moss_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(moss_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;moss_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MOSS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int moss_init(void){if(init)return -1;st.n_mossop=0;st.n_moss=0;st.n_moss2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mossos[i].active=0;for(int i=0;i<N-2;i++)mosse[i].active=0;for(int i=0;i<N-4;i++)moss2[i].active=0;for(int i=0;i<N-6;i++)mossac[i].active=0;for(int i=0;i<N-6;i++)mossam[i].active=0;init=1;ps("[MOSS] Mossia initialized\n");return 0;}
int moss_planning(int t,int c,int a,int b,int d,int e,int y){return add(mossos,&st.n_mossop,&st.t_f1,N,t,c,a,b,d,e,y);}
int moss_execution(int t,int c,int a,int b,int d,int e,int y){return add(mosse,&st.n_moss,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int moss_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(moss2,&st.n_moss2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int moss_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mossac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int moss_market(int t,int c,int a,int b,int d,int e,int y){return add(mossam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void moss_report(void){ps("[MOSS] Mossp: ");pi(st.n_mossop);ps(" PCS=");pi(st.t_f1);ps("\nMosse: ");pi(st.n_moss);ps(" PCS=");pi(st.t_f2);ps("\nMoss2: ");pi(st.n_moss2);ps(" PCS=");pi(st.t_f3);ps("\nMss: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void moss_state(void){ps("[MOSS] Mossp=");pi(st.n_mossop);ps(" Moss=");pi(st.n_moss);ps(" Ss2=");pi(st.n_moss2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mossia Admin Demo ===\n\n");moss_init();
ps("Mossia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;moss_planning(t,c,1581-(i*17),1570-(i*14),1550-(i*10),1532-(i*6),2020+(i%5));}
ps("\nMossia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;moss_execution(t,c,1570-(i*15),1559-(i*12),1541-(i*8),1528-(i*5),2021+(i%4));}
ps("\nMossia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;moss_evaluation(t,c,1562-(i*13),1551-(i*10),1535-(i*7),1524-(i*4),2022+(i%3));}
ps("\nMossia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;moss_accessory(t,c,1554-(i*11),1545-(i*9),1531-(i*6),1521-(i*3),2023+(i%2));}
ps("\nMossia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;moss_market(t,c,1548-(i*9),1539-(i*7),1526-(i*5),1518-(i*3),2024);}
ps("\n");moss_report();moss_state();ps("\n=== Demo Complete ===\n");return 0;}
