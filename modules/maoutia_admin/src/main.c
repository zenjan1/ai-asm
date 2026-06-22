/* maoutia_admin: Maoutia management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mao_t;
typedef struct{int n_maoop,n_mao,n_mao2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mao_state_t;
static mao_t maos[N],maoe[N-2],mao2[N-4],maoac[N-6],maoam[N-6]; static mao_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mao_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mao_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MAO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mao_init(void){if(init)return -1;st.n_maoop=0;st.n_mao=0;st.n_mao2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)maos[i].active=0;for(int i=0;i<N-2;i++)maoe[i].active=0;for(int i=0;i<N-4;i++)mao2[i].active=0;for(int i=0;i<N-6;i++)maoac[i].active=0;for(int i=0;i<N-6;i++)maoam[i].active=0;init=1;ps("[MAO] Maoutia initialized\n");return 0;}
int mao_planning(int t,int c,int a,int b,int d,int e,int y){return add(maos,&st.n_maoop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mao_execution(int t,int c,int a,int b,int d,int e,int y){return add(maoe,&st.n_mao,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mao_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mao2,&st.n_mao2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mao_accessory(int t,int c,int a,int b,int d,int e,int y){return add(maoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mao_market(int t,int c,int a,int b,int d,int e,int y){return add(maoam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mao_report(void){ps("[MAO] Maop: ");pi(st.n_maoop);ps(" PCS=");pi(st.t_f1);ps("\nMaoe: ");pi(st.n_mao);ps(" PCS=");pi(st.t_f2);ps("\nMao2: ");pi(st.n_mao2);ps(" PCS=");pi(st.t_f3);ps("\nMao: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mao_state(void){ps("[MAO] Maop=");pi(st.n_maoop);ps(" Mao=");pi(st.n_mao);ps(" Mao2=");pi(st.n_mao2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Maoutia Admin Demo ===\n\n");mao_init();
ps("Maoutia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mao_planning(t,c,1563-(i*17),1552-(i*14),1532-(i*10),1514-(i*6),2020+(i%5));}
ps("\nMaoutia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mao_execution(t,c,1552-(i*15),1541-(i*12),1523-(i*8),1510-(i*5),2021+(i%4));}
ps("\nMaoutia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mao_evaluation(t,c,1544-(i*13),1533-(i*10),1517-(i*7),1506-(i*4),2022+(i%3));}
ps("\nMaoutia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mao_accessory(t,c,1536-(i*11),1527-(i*9),1513-(i*6),1503-(i*3),2023+(i%2));}
ps("\nMaoutia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mao_market(t,c,1530-(i*9),1521-(i*7),1508-(i*5),1500-(i*3),2024);}
ps("\n");mao_report();mao_state();ps("\n=== Demo Complete ===\n");return 0;}
