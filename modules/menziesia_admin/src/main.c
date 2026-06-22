/* menziesia_admin: Menziesia management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} menz_t;
typedef struct{int n_menzop,n_menz,n_menz2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} menz_state_t;
static menz_t menzos[N],menze[N-2],menz2[N-4],menzac[N-6],menzam[N-6]; static menz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(menz_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;menz_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MENZ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int menz_init(void){if(init)return -1;st.n_menzop=0;st.n_menz=0;st.n_menz2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)menzos[i].active=0;for(int i=0;i<N-2;i++)menze[i].active=0;for(int i=0;i<N-4;i++)menz2[i].active=0;for(int i=0;i<N-6;i++)menzac[i].active=0;for(int i=0;i<N-6;i++)menzam[i].active=0;init=1;ps("[MENZ] Menziesia initialized\n");return 0;}
int menz_planning(int t,int c,int a,int b,int d,int e,int y){return add(menzos,&st.n_menzop,&st.t_f1,N,t,c,a,b,d,e,y);}
int menz_execution(int t,int c,int a,int b,int d,int e,int y){return add(menze,&st.n_menz,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int menz_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(menz2,&st.n_menz2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int menz_accessory(int t,int c,int a,int b,int d,int e,int y){return add(menzac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int menz_market(int t,int c,int a,int b,int d,int e,int y){return add(menzam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void menz_report(void){ps("[MENZ] Menzp: ");pi(st.n_menzop);ps(" PCS=");pi(st.t_f1);ps("\nMenze: ");pi(st.n_menz);ps(" PCS=");pi(st.t_f2);ps("\nMenz2: ");pi(st.n_menz2);ps(" PCS=");pi(st.t_f3);ps("\nMnz: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void menz_state(void){ps("[MENZ] Menzp=");pi(st.n_menzop);ps(" Menz=");pi(st.n_menz);ps(" Nz2=");pi(st.n_menz2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Menziesia Admin Demo ===\n\n");menz_init();
ps("Menziesia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;menz_planning(t,c,1570-(i*17),1559-(i*14),1539-(i*10),1521-(i*6),2020+(i%5));}
ps("\nMenziesia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;menz_execution(t,c,1559-(i*15),1548-(i*12),1530-(i*8),1517-(i*5),2021+(i%4));}
ps("\nMenziesia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;menz_evaluation(t,c,1551-(i*13),1540-(i*10),1524-(i*7),1513-(i*4),2022+(i%3));}
ps("\nMenziesia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;menz_accessory(t,c,1543-(i*11),1534-(i*9),1520-(i*6),1510-(i*3),2023+(i%2));}
ps("\nMenziesia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;menz_market(t,c,1537-(i*9),1528-(i*7),1515-(i*5),1507-(i*3),2024);}
ps("\n");menz_report();menz_state();ps("\n=== Demo Complete ===\n");return 0;}
