/* pteridium_admin: Pteridium management technology administration (v1.0)
 * Pteridium planning, pteridium execution, pteridium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pter_t;
typedef struct{int n_pterp,n_ptere,n_pter2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pter_state_t;
static pter_t pters[N],pteress[N-2],ptervss[N-4],pteras[N-6],ptermks[N-6]; static pter_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(pter_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pter_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PTER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pter_init(void){if(init)return -1;st.n_pterp=0;st.n_ptere=0;st.n_pter2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pters[i].active=0;for(int i=0;i<N-2;i++)pteress[i].active=0;for(int i=0;i<N-4;i++)ptervss[i].active=0;for(int i=0;i<N-6;i++)pteras[i].active=0;for(int i=0;i<N-6;i++)ptermks[i].active=0;init=1;ps("[PTER] Pteridium initialized\n");return 0;}
int pter_planning(int t,int c,int a,int b,int d,int e,int y){return add(pters,&st.n_pterp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pter_execution(int t,int c,int a,int b,int d,int e,int y){return add(pteress,&st.n_ptere,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pter_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ptervss,&st.n_pter2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pter_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pteras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pter_market(int t,int c,int a,int b,int d,int e,int y){return add(ptermks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pter_report(void){ps("[PTER] Pterp: ");pi(st.n_pterp);ps(" PCS=");pi(st.t_f1);ps("\nPtere: ");pi(st.n_ptere);ps(" PCS=");pi(st.t_f2);ps("\nPterv: ");pi(st.n_pter2);ps(" PCS=");pi(st.t_f3);ps("\nPterc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pter_state(void){ps("[PTER] Pterp=");pi(st.n_pterp);ps(" Ptere=");pi(st.n_ptere);ps(" Pterv=");pi(st.n_pter2);ps(" Pterc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pteridium Admin Demo ===\n\n");pter_init();
ps("Pteridium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pter_planning(t,c,1332+(i*17),1321+(i*14),1301+(i*10),1283+(i*6),2020+(i%5));}
ps("\nPteridium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pter_execution(t,c,1321+(i*15),1310+(i*12),1292+(i*8),1279+(i*5),2021+(i%4));}
ps("\nPteridium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pter_evaluation(t,c,1313+(i*13),1302+(i*10),1286+(i*7),1275+(i*4),2022+(i%3));}
ps("\nPteridium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pter_accessory(t,c,1305+(i*11),1296+(i*9),1282+(i*6),1272+(i*3),2023+(i%2));}
ps("\nPteridium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pter_market(t,c,1299+(i*9),1290+(i*7),1277+(i*5),1269+(i*3),2024);}
ps("\n");pter_report();pter_state();ps("\n=== Demo Complete ===\n");return 0;}
