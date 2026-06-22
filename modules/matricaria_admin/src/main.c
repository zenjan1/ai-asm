/* matricaria_admin: Matricaria management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} matri_t;
typedef struct{int n_matrop,n_matri,n_matri2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} matri_state_t;
static matri_t matros[N],matrie[N-2],matri2[N-4],matriac[N-6],matriam[N-6]; static matri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(matri_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;matri_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MATRI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int matri_init(void){if(init)return -1;st.n_matrop=0;st.n_matri=0;st.n_matri2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)matros[i].active=0;for(int i=0;i<N-2;i++)matrie[i].active=0;for(int i=0;i<N-4;i++)matri2[i].active=0;for(int i=0;i<N-6;i++)matriac[i].active=0;for(int i=0;i<N-6;i++)matriam[i].active=0;init=1;ps("[MATRI] Matricaria initialized\n");return 0;}
int matri_planning(int t,int c,int a,int b,int d,int e,int y){return add(matros,&st.n_matrop,&st.t_f1,N,t,c,a,b,d,e,y);}
int matri_execution(int t,int c,int a,int b,int d,int e,int y){return add(matrie,&st.n_matri,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int matri_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(matri2,&st.n_matri2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int matri_accessory(int t,int c,int a,int b,int d,int e,int y){return add(matriac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int matri_market(int t,int c,int a,int b,int d,int e,int y){return add(matriam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void matri_report(void){ps("[MATRI] Matrip: ");pi(st.n_matrop);ps(" PCS=");pi(st.t_f1);ps("\nMatrie: ");pi(st.n_matri);ps(" PCS=");pi(st.t_f2);ps("\nMatri2: ");pi(st.n_matri2);ps(" PCS=");pi(st.t_f3);ps("\nMtri: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void matri_state(void){ps("[MATRI] Matrip=");pi(st.n_matrop);ps(" Matri=");pi(st.n_matri);ps(" Tri2=");pi(st.n_matri2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Matricaria Admin Demo ===\n\n");matri_init();
ps("Matricaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;matri_planning(t,c,1566-(i*17),1555-(i*14),1535-(i*10),1517-(i*6),2020+(i%5));}
ps("\nMatricaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;matri_execution(t,c,1555-(i*15),1544-(i*12),1526-(i*8),1513-(i*5),2021+(i%4));}
ps("\nMatricaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;matri_evaluation(t,c,1547-(i*13),1536-(i*10),1520-(i*7),1509-(i*4),2022+(i%3));}
ps("\nMatricaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;matri_accessory(t,c,1539-(i*11),1530-(i*9),1516-(i*6),1506-(i*3),2023+(i%2));}
ps("\nMatricaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;matri_market(t,c,1533-(i*9),1524-(i*7),1511-(i*5),1503-(i*3),2024);}
ps("\n");matri_report();matri_state();ps("\n=== Demo Complete ===\n");return 0;}
