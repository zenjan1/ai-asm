/* mimosa_admin: Mimosa management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mimi_t;
typedef struct{int n_mimiop,n_mimi,n_mimi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mimi_state_t;
static mimi_t mimos[N],mimie[N-2],mimi2[N-4],mimiac[N-6],mimiam[N-6]; static mimi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mimi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mimi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MIMI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mimi_init(void){if(init)return -1;st.n_mimiop=0;st.n_mimi=0;st.n_mimi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mimos[i].active=0;for(int i=0;i<N-2;i++)mimie[i].active=0;for(int i=0;i<N-4;i++)mimi2[i].active=0;for(int i=0;i<N-6;i++)mimiac[i].active=0;for(int i=0;i<N-6;i++)mimiam[i].active=0;init=1;ps("[MIMI] Mimosa initialized\n");return 0;}
int mimi_planning(int t,int c,int a,int b,int d,int e,int y){return add(mimos,&st.n_mimiop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mimi_execution(int t,int c,int a,int b,int d,int e,int y){return add(mimie,&st.n_mimi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mimi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mimi2,&st.n_mimi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mimi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mimiac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mimi_market(int t,int c,int a,int b,int d,int e,int y){return add(mimiam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mimi_report(void){ps("[MIMI] Mimop: ");pi(st.n_mimiop);ps(" PCS=");pi(st.t_f1);ps("\nMimie: ");pi(st.n_mimi);ps(" PCS=");pi(st.t_f2);ps("\nMimi2: ");pi(st.n_mimi2);ps(" PCS=");pi(st.t_f3);ps("\nMmi: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mimi_state(void){ps("[MIMI] Mimop=");pi(st.n_mimiop);ps(" Mimi=");pi(st.n_mimi);ps(" Mi2=");pi(st.n_mimi2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mimosa Admin Demo ===\n\n");mimi_init();
ps("Mimosa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mimi_planning(t,c,1573-(i*17),1562-(i*14),1542-(i*10),1524-(i*6),2020+(i%5));}
ps("\nMimosa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mimi_execution(t,c,1562-(i*15),1551-(i*12),1533-(i*8),1520-(i*5),2021+(i%4));}
ps("\nMimosa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mimi_evaluation(t,c,1554-(i*13),1543-(i*10),1527-(i*7),1516-(i*4),2022+(i%3));}
ps("\nMimosa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mimi_accessory(t,c,1546-(i*11),1537-(i*9),1523-(i*6),1513-(i*3),2023+(i%2));}
ps("\nMimosa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mimi_market(t,c,1540-(i*9),1531-(i*7),1518-(i*5),1510-(i*3),2024);}
ps("\n");mimi_report();mimi_state();ps("\n=== Demo Complete ===\n");return 0;}
