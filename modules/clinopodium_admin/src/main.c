/* clinopodium_admin: Clinopodium management technology administration (v1.0)
 * Clinopodium planning, clinopodium execution, clinopodium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nod_t;
typedef struct{int n_nodp,n_node,n_nod2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nod_state_t;
static nod_t nodps[N],nodss[N-2],nodvss[N-4],nodas[N-6],nodmks[N-6]; static nod_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(nod_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nod_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NOD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nod_init(void){if(init)return -1;st.n_nodp=0;st.n_node=0;st.n_nod2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nodps[i].active=0;for(int i=0;i<N-2;i++)nodss[i].active=0;for(int i=0;i<N-4;i++)nodvss[i].active=0;for(int i=0;i<N-6;i++)nodas[i].active=0;for(int i=0;i<N-6;i++)nodmks[i].active=0;init=1;ps("[NOD] Clinopodium initialized\n");return 0;}
int nod_planning(int t,int c,int a,int b,int d,int e,int y){return add(nodps,&st.n_nodp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nod_execution(int t,int c,int a,int b,int d,int e,int y){return add(nodss,&st.n_node,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nod_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nodvss,&st.n_nod2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nod_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nodas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nod_market(int t,int c,int a,int b,int d,int e,int y){return add(nodmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nod_report(void){ps("[NOD] Clipp: ");pi(st.n_nodp);ps(" PCS=");pi(st.t_f1);ps("\nClipe: ");pi(st.n_node);ps(" PCS=");pi(st.t_f2);ps("\nCliv: ");pi(st.n_nod2);ps(" PCS=");pi(st.t_f3);ps("\nClic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nod_state(void){ps("[NOD] Clipp=");pi(st.n_nodp);ps(" Clipe=");pi(st.n_node);ps(" Cliv=");pi(st.n_nod2);ps(" Clic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Clinopodium Admin Demo ===\n\n");nod_init();
ps("Clinopodium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nod_planning(t,c,1238+(i*17),1227+(i*14),1207+(i*10),1189+(i*6),2020+(i%5));}
ps("\nClinopodium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nod_execution(t,c,1227+(i*15),1216+(i*12),1198+(i*8),1185+(i*5),2021+(i%4));}
ps("\nClinopodium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nod_evaluation(t,c,1219+(i*13),1208+(i*10),1192+(i*7),1181+(i*4),2022+(i%3));}
ps("\nClinopodium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nod_accessory(t,c,1211+(i*11),1202+(i*9),1188+(i*6),1178+(i*3),2023+(i%2));}
ps("\nClinopodium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nod_market(t,c,1205+(i*9),1196+(i*7),1183+(i*5),1175+(i*3),2024);}
ps("\n");nod_report();nod_state();ps("\n=== Demo Complete ===\n");return 0;}
