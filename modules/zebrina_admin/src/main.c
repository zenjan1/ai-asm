/* zebrina_admin: Zebrina management technology administration (v1.0)
 * Zebrina planning, zebrina execution, zebrina evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zeb_t;
typedef struct{int n_zebp,n_zebe,n_zeb2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zeb_state_t;
static zeb_t zebps[N],zebss[N-2],zebvss[N-4],zebas[N-6],zebmks[N-6]; static zeb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zeb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zeb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZEB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zeb_init(void){if(init)return -1;st.n_zebp=0;st.n_zebe=0;st.n_zeb2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zebps[i].active=0;for(int i=0;i<N-2;i++)zebss[i].active=0;for(int i=0;i<N-4;i++)zebvss[i].active=0;for(int i=0;i<N-6;i++)zebas[i].active=0;for(int i=0;i<N-6;i++)zebmks[i].active=0;init=1;ps("[ZEB] Zebrina initialized\n");return 0;}
int zeb_planning(int t,int c,int a,int b,int d,int e,int y){return add(zebps,&st.n_zebp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zeb_execution(int t,int c,int a,int b,int d,int e,int y){return add(zebss,&st.n_zebe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zeb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zebvss,&st.n_zeb2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zeb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zebas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zeb_market(int t,int c,int a,int b,int d,int e,int y){return add(zebmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zeb_report(void){ps("[ZEB] Zebpp: ");pi(st.n_zebp);ps(" PCS=");pi(st.t_f1);ps("\nZebpe: ");pi(st.n_zebe);ps(" PCS=");pi(st.t_f2);ps("\nZebv: ");pi(st.n_zeb2);ps(" PCS=");pi(st.t_f3);ps("\nZebc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zeb_state(void){ps("[ZEB] Zebpp=");pi(st.n_zebp);ps(" Zebpe=");pi(st.n_zebe);ps(" Zebv=");pi(st.n_zeb2);ps(" Zebc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zebrina Admin Demo ===\n\n");zeb_init();
ps("Zebrina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zeb_planning(t,c,1205+(i*17),1194+(i*14),1174+(i*10),1156+(i*6),2020+(i%5));}
ps("\nZebrina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zeb_execution(t,c,1194+(i*15),1183+(i*12),1165+(i*8),1152+(i*5),2021+(i%4));}
ps("\nZebrina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zeb_evaluation(t,c,1186+(i*13),1175+(i*10),1159+(i*7),1148+(i*4),2022+(i%3));}
ps("\nZebrina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zeb_accessory(t,c,1178+(i*11),1169+(i*9),1155+(i*6),1145+(i*3),2023+(i%2));}
ps("\nZebrina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zeb_market(t,c,1172+(i*9),1163+(i*7),1150+(i*5),1142+(i*3),2024);}
ps("\n");zeb_report();zeb_state();ps("\n=== Demo Complete ===\n");return 0;}
