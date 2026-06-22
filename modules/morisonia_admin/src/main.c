/* morisonia_admin: Morisonia management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mori_t;
typedef struct{int n_moriop,n_mori,n_mori2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mori_state_t;
static mori_t morios[N],morie[N-2],mori2[N-4],moriac[N-6],moriam[N-6]; static mori_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mori_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mori_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MORI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mori_init(void){if(init)return -1;st.n_moriop=0;st.n_mori=0;st.n_mori2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)morios[i].active=0;for(int i=0;i<N-2;i++)morie[i].active=0;for(int i=0;i<N-4;i++)mori2[i].active=0;for(int i=0;i<N-6;i++)moriac[i].active=0;for(int i=0;i<N-6;i++)moriam[i].active=0;init=1;ps("[MORI] Morisonia initialized\n");return 0;}
int mori_planning(int t,int c,int a,int b,int d,int e,int y){return add(morios,&st.n_moriop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mori_execution(int t,int c,int a,int b,int d,int e,int y){return add(morie,&st.n_mori,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mori_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mori2,&st.n_mori2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mori_accessory(int t,int c,int a,int b,int d,int e,int y){return add(moriac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mori_market(int t,int c,int a,int b,int d,int e,int y){return add(moriam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mori_report(void){ps("[MORI] Moriop: ");pi(st.n_moriop);ps(" PCS=");pi(st.t_f1);ps("\nMorie: ");pi(st.n_mori);ps(" PCS=");pi(st.t_f2);ps("\nMori2: ");pi(st.n_mori2);ps(" PCS=");pi(st.t_f3);ps("\nMri: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mori_state(void){ps("[MORI] Moriop=");pi(st.n_moriop);ps(" Mori=");pi(st.n_mori);ps(" Ri2=");pi(st.n_mori2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Morisonia Admin Demo ===\n\n");mori_init();
ps("Morisonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mori_planning(t,c,1579-(i*17),1568-(i*14),1548-(i*10),1530-(i*6),2020+(i%5));}
ps("\nMorisonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mori_execution(t,c,1568-(i*15),1557-(i*12),1539-(i*8),1526-(i*5),2021+(i%4));}
ps("\nMorisonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mori_evaluation(t,c,1560-(i*13),1549-(i*10),1533-(i*7),1522-(i*4),2022+(i%3));}
ps("\nMorisonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mori_accessory(t,c,1552-(i*11),1543-(i*9),1529-(i*6),1519-(i*3),2023+(i%2));}
ps("\nMorisonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mori_market(t,c,1546-(i*9),1537-(i*7),1524-(i*5),1516-(i*3),2024);}
ps("\n");mori_report();mori_state();ps("\n=== Demo Complete ===\n");return 0;}
