/* machaerina_admin: Machaerina management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mach_t;
typedef struct{int n_machop,n_mach,n_mach2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mach_state_t;
static mach_t machos[N],mache[N-2],mach2[N-4],machac[N-6],macham[N-6]; static mach_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mach_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mach_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MACH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mach_init(void){if(init)return -1;st.n_machop=0;st.n_mach=0;st.n_mach2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)machos[i].active=0;for(int i=0;i<N-2;i++)mache[i].active=0;for(int i=0;i<N-4;i++)mach2[i].active=0;for(int i=0;i<N-6;i++)machac[i].active=0;for(int i=0;i<N-6;i++)macham[i].active=0;init=1;ps("[MACH] Machaerina initialized\n");return 0;}
int mach_planning(int t,int c,int a,int b,int d,int e,int y){return add(machos,&st.n_machop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mach_execution(int t,int c,int a,int b,int d,int e,int y){return add(mache,&st.n_mach,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mach_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mach2,&st.n_mach2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mach_accessory(int t,int c,int a,int b,int d,int e,int y){return add(machac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mach_market(int t,int c,int a,int b,int d,int e,int y){return add(macham,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mach_report(void){ps("[MACH] Machp: ");pi(st.n_machop);ps(" PCS=");pi(st.t_f1);ps("\nMache: ");pi(st.n_mach);ps(" PCS=");pi(st.t_f2);ps("\nMach2: ");pi(st.n_mach2);ps(" PCS=");pi(st.t_f3);ps("\nMch: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mach_state(void){ps("[MACH] Machp=");pi(st.n_machop);ps(" Mach=");pi(st.n_mach);ps(" Mch2=");pi(st.n_mach2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Machaerina Admin Demo ===\n\n");mach_init();
ps("Machaerina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mach_planning(t,c,1558-(i*17),1547-(i*14),1527-(i*10),1509-(i*6),2020+(i%5));}
ps("\nMachaerina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mach_execution(t,c,1547-(i*15),1536-(i*12),1518-(i*8),1505-(i*5),2021+(i%4));}
ps("\nMachaerina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mach_evaluation(t,c,1539-(i*13),1528-(i*10),1512-(i*7),1501-(i*4),2022+(i%3));}
ps("\nMachaerina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mach_accessory(t,c,1531-(i*11),1522-(i*9),1508-(i*6),1498-(i*3),2023+(i%2));}
ps("\nMachaerina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mach_market(t,c,1525-(i*9),1516-(i*7),1503-(i*5),1495-(i*3),2024);}
ps("\n");mach_report();mach_state();ps("\n=== Demo Complete ===\n");return 0;}
