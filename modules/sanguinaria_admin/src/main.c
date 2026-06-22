/* sanguinaria_admin: Sanguinaria management technology administration (v1.0)
 * Sanguinaria planning, sanguinaria execution, sanguinaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} san_t;
typedef struct{int n_sanp,n_sane,n_san2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} san_state_t;
static san_t sanps[N],sanss[N-2],sanvss[N-4],sanas[N-6],sanmks[N-6]; static san_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(san_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;san_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int san_init(void){if(init)return -1;st.n_sanp=0;st.n_sane=0;st.n_san2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sanps[i].active=0;for(int i=0;i<N-2;i++)sanss[i].active=0;for(int i=0;i<N-4;i++)sanvss[i].active=0;for(int i=0;i<N-6;i++)sanas[i].active=0;for(int i=0;i<N-6;i++)sanmks[i].active=0;init=1;ps("[SAN] Sanguinaria initialized\n");return 0;}
int san_planning(int t,int c,int a,int b,int d,int e,int y){return add(sanps,&st.n_sanp,&st.t_f1,N,t,c,a,b,d,e,y);}
int san_execution(int t,int c,int a,int b,int d,int e,int y){return add(sanss,&st.n_sane,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int san_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sanvss,&st.n_san2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int san_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sanas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int san_market(int t,int c,int a,int b,int d,int e,int y){return add(sanmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void san_report(void){ps("[SAN] Sanpp: ");pi(st.n_sanp);ps(" PCS=");pi(st.t_f1);ps("\nSane: ");pi(st.n_sane);ps(" PCS=");pi(st.t_f2);ps("\nSanv: ");pi(st.n_san2);ps(" PCS=");pi(st.t_f3);ps("\nSanc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void san_state(void){ps("[SAN] Sanpp=");pi(st.n_sanp);ps(" Sane=");pi(st.n_sane);ps(" Sanv=");pi(st.n_san2);ps(" Sanc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sanguinaria Admin Demo ===\n\n");san_init();
ps("Sanguinaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;san_planning(t,c,1186+(i*17),1175+(i*14),1155+(i*10),1137+(i*6),2020+(i%5));}
ps("\nSanguinaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;san_execution(t,c,1175+(i*15),1164+(i*12),1146+(i*8),1133+(i*5),2021+(i%4));}
ps("\nSanguinaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;san_evaluation(t,c,1167+(i*13),1156+(i*10),1140+(i*7),1129+(i*4),2022+(i%3));}
ps("\nSanguinaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;san_accessory(t,c,1159+(i*11),1150+(i*9),1136+(i*6),1126+(i*3),2023+(i%2));}
ps("\nSanguinaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;san_market(t,c,1153+(i*9),1144+(i*7),1131+(i*5),1123+(i*3),2024);}
ps("\n");san_report();san_state();ps("\n=== Demo Complete ===\n");return 0;}
