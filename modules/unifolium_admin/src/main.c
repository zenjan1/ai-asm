/* unifolium_admin: Unifolium management technology administration (v1.0)
 * Unifolium planning, unifolium execution, unifolium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} unif_t;
typedef struct{int n_unifp,n_unife,n_unif2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} unif_state_t;
static unif_t unifs[N],unifess[N-2],unifvss[N-4],unifas[N-6],unifmks[N-6]; static unif_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(unif_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;unif_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[UNIF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int unif_init(void){if(init)return -1;st.n_unifp=0;st.n_unife=0;st.n_unif2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)unifs[i].active=0;for(int i=0;i<N-2;i++)unifess[i].active=0;for(int i=0;i<N-4;i++)unifvss[i].active=0;for(int i=0;i<N-6;i++)unifas[i].active=0;for(int i=0;i<N-6;i++)unifmks[i].active=0;init=1;ps("[UNIF] Unifolium initialized\n");return 0;}
int unif_planning(int t,int c,int a,int b,int d,int e,int y){return add(unifs,&st.n_unifp,&st.t_f1,N,t,c,a,b,d,e,y);}
int unif_execution(int t,int c,int a,int b,int d,int e,int y){return add(unifess,&st.n_unife,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int unif_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(unifvss,&st.n_unif2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int unif_accessory(int t,int c,int a,int b,int d,int e,int y){return add(unifas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int unif_market(int t,int c,int a,int b,int d,int e,int y){return add(unifmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void unif_report(void){ps("[UNIF] Unifp: ");pi(st.n_unifp);ps(" PCS=");pi(st.t_f1);ps("\nUnife: ");pi(st.n_unife);ps(" PCS=");pi(st.t_f2);ps("\nUnifv: ");pi(st.n_unif2);ps(" PCS=");pi(st.t_f3);ps("\nUnifc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void unif_state(void){ps("[UNIF] Unifp=");pi(st.n_unifp);ps(" Unife=");pi(st.n_unife);ps(" Unifv=");pi(st.n_unif2);ps(" Unifc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Unifolium Admin Demo ===\n\n");unif_init();
ps("Unifolium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;unif_planning(t,c,1353+(i*17),1342+(i*14),1322+(i*10),1304+(i*6),2020+(i%5));}
ps("\nUnifolium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;unif_execution(t,c,1342+(i*15),1331+(i*12),1313+(i*8),1300+(i*5),2021+(i%4));}
ps("\nUnifolium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;unif_evaluation(t,c,1334+(i*13),1323+(i*10),1307+(i*7),1296+(i*4),2022+(i%3));}
ps("\nUnifolium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;unif_accessory(t,c,1326+(i*11),1317+(i*9),1303+(i*6),1293+(i*3),2023+(i%2));}
ps("\nUnifolium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;unif_market(t,c,1320+(i*9),1311+(i*7),1298+(i*5),1290+(i*3),2024);}
ps("\n");unif_report();unif_state();ps("\n=== Demo Complete ===\n");return 0;}
