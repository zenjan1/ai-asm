/* molinaea_admin: Molinaea management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} moli_t;
typedef struct{int n_moliop,n_moli,n_moli2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} moli_state_t;
static moli_t molos[N],molie[N-2],moli2[N-4],moliac[N-6],moliam[N-6]; static moli_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(moli_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;moli_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MOLI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int moli_init(void){if(init)return -1;st.n_moliop=0;st.n_moli=0;st.n_moli2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)molos[i].active=0;for(int i=0;i<N-2;i++)molie[i].active=0;for(int i=0;i<N-4;i++)moli2[i].active=0;for(int i=0;i<N-6;i++)moliac[i].active=0;for(int i=0;i<N-6;i++)moliam[i].active=0;init=1;ps("[MOLI] Molinaea initialized\n");return 0;}
int moli_planning(int t,int c,int a,int b,int d,int e,int y){return add(molos,&st.n_moliop,&st.t_f1,N,t,c,a,b,d,e,y);}
int moli_execution(int t,int c,int a,int b,int d,int e,int y){return add(molie,&st.n_moli,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int moli_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(moli2,&st.n_moli2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int moli_accessory(int t,int c,int a,int b,int d,int e,int y){return add(moliac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int moli_market(int t,int c,int a,int b,int d,int e,int y){return add(moliam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void moli_report(void){ps("[MOLI] Molp: ");pi(st.n_moliop);ps(" PCS=");pi(st.t_f1);ps("\nMolie: ");pi(st.n_moli);ps(" PCS=");pi(st.t_f2);ps("\nMoli2: ");pi(st.n_moli2);ps(" PCS=");pi(st.t_f3);ps("\nMli: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void moli_state(void){ps("[MOLI] Molp=");pi(st.n_moliop);ps(" Moli=");pi(st.n_moli);ps(" Li2=");pi(st.n_moli2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Molinaea Admin Demo ===\n\n");moli_init();
ps("Molinaea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;moli_planning(t,c,1575-(i*17),1564-(i*14),1544-(i*10),1526-(i*6),2020+(i%5));}
ps("\nMolinaea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;moli_execution(t,c,1564-(i*15),1553-(i*12),1535-(i*8),1522-(i*5),2021+(i%4));}
ps("\nMolinaea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;moli_evaluation(t,c,1556-(i*13),1545-(i*10),1529-(i*7),1518-(i*4),2022+(i%3));}
ps("\nMolinaea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;moli_accessory(t,c,1548-(i*11),1539-(i*9),1525-(i*6),1515-(i*3),2023+(i%2));}
ps("\nMolinaea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;moli_market(t,c,1542-(i*9),1533-(i*7),1520-(i*5),1512-(i*3),2024);}
ps("\n");moli_report();moli_state();ps("\n=== Demo Complete ===\n");return 0;}
