/* luzuriopsis_admin: Luzuriopsis management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} luzu_t;
typedef struct{int n_luzuop,n_luzu,n_luzu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} luzu_state_t;
static luzu_t luzuos[N],luzue[N-2],luzu2[N-4],luzuac[N-6],luzuam[N-6]; static luzu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(luzu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;luzu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LUZU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int luzu_init(void){if(init)return -1;st.n_luzuop=0;st.n_luzu=0;st.n_luzu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)luzuos[i].active=0;for(int i=0;i<N-2;i++)luzue[i].active=0;for(int i=0;i<N-4;i++)luzu2[i].active=0;for(int i=0;i<N-6;i++)luzuac[i].active=0;for(int i=0;i<N-6;i++)luzuam[i].active=0;init=1;ps("[LUZU] Luzuriopsis initialized\n");return 0;}
int luzu_planning(int t,int c,int a,int b,int d,int e,int y){return add(luzuos,&st.n_luzuop,&st.t_f1,N,t,c,a,b,d,e,y);}
int luzu_execution(int t,int c,int a,int b,int d,int e,int y){return add(luzue,&st.n_luzu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int luzu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(luzu2,&st.n_luzu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int luzu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(luzuac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int luzu_market(int t,int c,int a,int b,int d,int e,int y){return add(luzuam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void luzu_report(void){ps("[LUZU] Luzup: ");pi(st.n_luzuop);ps(" PCS=");pi(st.t_f1);ps("\nLuzue: ");pi(st.n_luzu);ps(" PCS=");pi(st.t_f2);ps("\nLuzu2: ");pi(st.n_luzu2);ps(" PCS=");pi(st.t_f3);ps("\nLzu: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void luzu_state(void){ps("[LUZU] Luzup=");pi(st.n_luzuop);ps(" Lzu=");pi(st.n_luzu);ps(" Lzu2=");pi(st.n_luzu2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Luzuriopsis Admin Demo ===\n\n");luzu_init();
ps("Luzuriopsis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;luzu_planning(t,c,1557-(i*17),1546-(i*14),1526-(i*10),1508-(i*6),2020+(i%5));}
ps("\nLuzuriopsis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;luzu_execution(t,c,1546-(i*15),1535-(i*12),1517-(i*8),1504-(i*5),2021+(i%4));}
ps("\nLuzuriopsis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;luzu_evaluation(t,c,1538-(i*13),1527-(i*10),1511-(i*7),1500-(i*4),2022+(i%3));}
ps("\nLuzuriopsis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;luzu_accessory(t,c,1530-(i*11),1521-(i*9),1507-(i*6),1497-(i*3),2023+(i%2));}
ps("\nLuzuriopsis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;luzu_market(t,c,1524-(i*9),1515-(i*7),1502-(i*5),1494-(i*3),2024);}
ps("\n");luzu_report();luzu_state();ps("\n=== Demo Complete ===\n");return 0;}
