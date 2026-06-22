/* mahernia_admin: Mahernia management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mahr_t;
typedef struct{int n_mahrop,n_mahr,n_mahr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mahr_state_t;
static mahr_t mahros[N],mahre[N-2],mahr2[N-4],mhrac[N-6],mhram[N-6]; static mahr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mahr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mahr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MAHR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mahr_init(void){if(init)return -1;st.n_mahrop=0;st.n_mahr=0;st.n_mahr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mahros[i].active=0;for(int i=0;i<N-2;i++)mahre[i].active=0;for(int i=0;i<N-4;i++)mahr2[i].active=0;for(int i=0;i<N-6;i++)mhrac[i].active=0;for(int i=0;i<N-6;i++)mhram[i].active=0;init=1;ps("[MAHR] Mahernia initialized\n");return 0;}
int mahr_planning(int t,int c,int a,int b,int d,int e,int y){return add(mahros,&st.n_mahrop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mahr_execution(int t,int c,int a,int b,int d,int e,int y){return add(mahre,&st.n_mahr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mahr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mahr2,&st.n_mahr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mahr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mhrac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mahr_market(int t,int c,int a,int b,int d,int e,int y){return add(mhram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mahr_report(void){ps("[MAHR] Mhrp: ");pi(st.n_mahrop);ps(" PCS=");pi(st.t_f1);ps("\nMhre: ");pi(st.n_mahr);ps(" PCS=");pi(st.t_f2);ps("\nMhr2: ");pi(st.n_mahr2);ps(" PCS=");pi(st.t_f3);ps("\nMhr: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mahr_state(void){ps("[MAHR] Mhrp=");pi(st.n_mahrop);ps(" Mhr=");pi(st.n_mahr);ps(" Mhr2=");pi(st.n_mahr2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mahernia Admin Demo ===\n\n");mahr_init();
ps("Mahernia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mahr_planning(t,c,1559-(i*17),1548-(i*14),1528-(i*10),1510-(i*6),2020+(i%5));}
ps("\nMahernia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mahr_execution(t,c,1548-(i*15),1537-(i*12),1519-(i*8),1506-(i*5),2021+(i%4));}
ps("\nMahernia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mahr_evaluation(t,c,1540-(i*13),1529-(i*10),1513-(i*7),1502-(i*4),2022+(i%3));}
ps("\nMahernia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mahr_accessory(t,c,1532-(i*11),1523-(i*9),1509-(i*6),1499-(i*3),2023+(i%2));}
ps("\nMahernia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mahr_market(t,c,1526-(i*9),1517-(i*7),1504-(i*5),1496-(i*3),2024);}
ps("\n");mahr_report();mahr_state();ps("\n=== Demo Complete ===\n");return 0;}
