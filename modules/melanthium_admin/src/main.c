/* melanthium_admin: Melanthium management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} meln_t;
typedef struct{int n_melnop,n_meln,n_meln2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} meln_state_t;
static meln_t melnos[N],melne[N-2],meln2[N-4],melnac[N-6],melnam[N-6]; static meln_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(meln_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;meln_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MELN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int meln_init(void){if(init)return -1;st.n_melnop=0;st.n_meln=0;st.n_meln2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)melnos[i].active=0;for(int i=0;i<N-2;i++)melne[i].active=0;for(int i=0;i<N-4;i++)meln2[i].active=0;for(int i=0;i<N-6;i++)melnac[i].active=0;for(int i=0;i<N-6;i++)melnam[i].active=0;init=1;ps("[MELN] Melanthium initialized\n");return 0;}
int meln_planning(int t,int c,int a,int b,int d,int e,int y){return add(melnos,&st.n_melnop,&st.t_f1,N,t,c,a,b,d,e,y);}
int meln_execution(int t,int c,int a,int b,int d,int e,int y){return add(melne,&st.n_meln,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int meln_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(meln2,&st.n_meln2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int meln_accessory(int t,int c,int a,int b,int d,int e,int y){return add(melnac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int meln_market(int t,int c,int a,int b,int d,int e,int y){return add(melnam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void meln_report(void){ps("[MELN] Melnp: ");pi(st.n_melnop);ps(" PCS=");pi(st.t_f1);ps("\nMelne: ");pi(st.n_meln);ps(" PCS=");pi(st.t_f2);ps("\nMeln2: ");pi(st.n_meln2);ps(" PCS=");pi(st.t_f3);ps("\nMln: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void meln_state(void){ps("[MELN] Melnp=");pi(st.n_melnop);ps(" Meln=");pi(st.n_meln);ps(" Lnn2=");pi(st.n_meln2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Melanthium Admin Demo ===\n\n");meln_init();
ps("Melanthium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;meln_planning(t,c,1568-(i*17),1557-(i*14),1537-(i*10),1519-(i*6),2020+(i%5));}
ps("\nMelanthium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;meln_execution(t,c,1557-(i*15),1546-(i*12),1528-(i*8),1515-(i*5),2021+(i%4));}
ps("\nMelanthium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;meln_evaluation(t,c,1549-(i*13),1538-(i*10),1522-(i*7),1511-(i*4),2022+(i%3));}
ps("\nMelanthium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;meln_accessory(t,c,1541-(i*11),1532-(i*9),1518-(i*6),1508-(i*3),2023+(i%2));}
ps("\nMelanthium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;meln_market(t,c,1535-(i*9),1526-(i*7),1513-(i*5),1505-(i*3),2024);}
ps("\n");meln_report();meln_state();ps("\n=== Demo Complete ===\n");return 0;}
