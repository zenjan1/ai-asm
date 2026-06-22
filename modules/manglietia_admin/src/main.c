/* manglietia_admin: Manglietia management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mang_t;
typedef struct{int n_mangop,n_mang,n_mang2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mang_state_t;
static mang_t mangos[N],mange[N-2],mang2[N-4],mangac[N-6],mangam[N-6]; static mang_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mang_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mang_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MANG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mang_init(void){if(init)return -1;st.n_mangop=0;st.n_mang=0;st.n_mang2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mangos[i].active=0;for(int i=0;i<N-2;i++)mange[i].active=0;for(int i=0;i<N-4;i++)mang2[i].active=0;for(int i=0;i<N-6;i++)mangac[i].active=0;for(int i=0;i<N-6;i++)mangam[i].active=0;init=1;ps("[MANG] Manglietia initialized\n");return 0;}
int mang_planning(int t,int c,int a,int b,int d,int e,int y){return add(mangos,&st.n_mangop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mang_execution(int t,int c,int a,int b,int d,int e,int y){return add(mange,&st.n_mang,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mang_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mang2,&st.n_mang2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mang_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mangac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mang_market(int t,int c,int a,int b,int d,int e,int y){return add(mangam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mang_report(void){ps("[MANG] Mangp: ");pi(st.n_mangop);ps(" PCS=");pi(st.t_f1);ps("\nMange: ");pi(st.n_mang);ps(" PCS=");pi(st.t_f2);ps("\nMang2: ");pi(st.n_mang2);ps(" PCS=");pi(st.t_f3);ps("\nMng: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mang_state(void){ps("[MANG] Mangp=");pi(st.n_mangop);ps(" Mang=");pi(st.n_mang);ps(" Mng2=");pi(st.n_mang2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Manglietia Admin Demo ===\n\n");mang_init();
ps("Manglietia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mang_planning(t,c,1562-(i*17),1551-(i*14),1531-(i*10),1513-(i*6),2020+(i%5));}
ps("\nManglietia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mang_execution(t,c,1551-(i*15),1540-(i*12),1522-(i*8),1509-(i*5),2021+(i%4));}
ps("\nManglietia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mang_evaluation(t,c,1543-(i*13),1532-(i*10),1516-(i*7),1505-(i*4),2022+(i%3));}
ps("\nManglietia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mang_accessory(t,c,1535-(i*11),1526-(i*9),1512-(i*6),1502-(i*3),2023+(i%2));}
ps("\nManglietia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mang_market(t,c,1529-(i*9),1520-(i*7),1507-(i*5),1499-(i*3),2024);}
ps("\n");mang_report();mang_state();ps("\n=== Demo Complete ===\n");return 0;}
