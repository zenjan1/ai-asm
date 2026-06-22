/* leucocoryne_admin: Leucocoryne management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} leuc_t;
typedef struct{int n_leucop,n_leuc,n_leuc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} leuc_state_t;
static leuc_t leucos[N],leuce[N-2],leuc2[N-4],leucac[N-6],leucam[N-6]; static leuc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(leuc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;leuc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEUC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int leuc_init(void){if(init)return -1;st.n_leucop=0;st.n_leuc=0;st.n_leuc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)leucos[i].active=0;for(int i=0;i<N-2;i++)leuce[i].active=0;for(int i=0;i<N-4;i++)leuc2[i].active=0;for(int i=0;i<N-6;i++)leucac[i].active=0;for(int i=0;i<N-6;i++)leucam[i].active=0;init=1;ps("[LEUC] Leucocoryne initialized\n");return 0;}
int leuc_planning(int t,int c,int a,int b,int d,int e,int y){return add(leucos,&st.n_leucop,&st.t_f1,N,t,c,a,b,d,e,y);}
int leuc_execution(int t,int c,int a,int b,int d,int e,int y){return add(leuce,&st.n_leuc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int leuc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(leuc2,&st.n_leuc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int leuc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(leucac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int leuc_market(int t,int c,int a,int b,int d,int e,int y){return add(leucam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void leuc_report(void){ps("[LEUC] Leucp: ");pi(st.n_leucop);ps(" PCS=");pi(st.t_f1);ps("\nLeuce: ");pi(st.n_leuc);ps(" PCS=");pi(st.t_f2);ps("\nLeuc2: ");pi(st.n_leuc2);ps(" PCS=");pi(st.t_f3);ps("\nLuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void leuc_state(void){ps("[LEUC] Leucp=");pi(st.n_leucop);ps(" Leuc=");pi(st.n_leuc);ps(" Luc2=");pi(st.n_leuc2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Leucocoryne Admin Demo ===\n\n");leuc_init();
ps("Leucocoryne planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;leuc_planning(t,c,1556-(i*17),1545-(i*14),1525-(i*10),1507-(i*6),2020+(i%5));}
ps("\nLeucocoryne execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;leuc_execution(t,c,1545-(i*15),1534-(i*12),1516-(i*8),1503-(i*5),2021+(i%4));}
ps("\nLeucocoryne evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;leuc_evaluation(t,c,1537-(i*13),1526-(i*10),1510-(i*7),1499-(i*4),2022+(i%3));}
ps("\nLeucocoryne accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;leuc_accessory(t,c,1529-(i*11),1520-(i*9),1506-(i*6),1496-(i*3),2023+(i%2));}
ps("\nLeucocoryne marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;leuc_market(t,c,1523-(i*9),1514-(i*7),1501-(i*5),1493-(i*3),2024);}
ps("\n");leuc_report();leuc_state();ps("\n=== Demo Complete ===\n");return 0;}
