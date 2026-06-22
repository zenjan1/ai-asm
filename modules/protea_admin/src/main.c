/* protea_admin: Protea management technology administration (v1.0)
 * Protea planning, protea execution, protea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} prot_t;
typedef struct{int n_protp,n_prote,n_prot2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} prot_state_t;
static prot_t prots[N],protss[N-2],protvss[N-4],protas[N-6],protmks[N-6]; static prot_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(prot_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;prot_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PROT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int prot_init(void){if(init)return -1;st.n_protp=0;st.n_prote=0;st.n_prot2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)prots[i].active=0;for(int i=0;i<N-2;i++)protss[i].active=0;for(int i=0;i<N-4;i++)protvss[i].active=0;for(int i=0;i<N-6;i++)protas[i].active=0;for(int i=0;i<N-6;i++)protmks[i].active=0;init=1;ps("[PROT] Protea initialized\n");return 0;}
int prot_planning(int t,int c,int a,int b,int d,int e,int y){return add(prots,&st.n_protp,&st.t_f1,N,t,c,a,b,d,e,y);}
int prot_execution(int t,int c,int a,int b,int d,int e,int y){return add(protss,&st.n_prote,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int prot_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(protvss,&st.n_prot2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int prot_accessory(int t,int c,int a,int b,int d,int e,int y){return add(protas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int prot_market(int t,int c,int a,int b,int d,int e,int y){return add(protmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void prot_report(void){ps("[PROT] Protp: ");pi(st.n_protp);ps(" PCS=");pi(st.t_f1);ps("\nProte: ");pi(st.n_prote);ps(" PCS=");pi(st.t_f2);ps("\nProtv: ");pi(st.n_prot2);ps(" PCS=");pi(st.t_f3);ps("\nProtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void prot_state(void){ps("[PROT] Protp=");pi(st.n_protp);ps(" Prote=");pi(st.n_prote);ps(" Protv=");pi(st.n_prot2);ps(" Protc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Protea Admin Demo ===\n\n");prot_init();
ps("Protea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;prot_planning(t,c,1331+(i*17),1320+(i*14),1300+(i*10),1282+(i*6),2020+(i%5));}
ps("\nProtea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;prot_execution(t,c,1320+(i*15),1309+(i*12),1291+(i*8),1278+(i*5),2021+(i%4));}
ps("\nProtea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;prot_evaluation(t,c,1312+(i*13),1301+(i*10),1285+(i*7),1274+(i*4),2022+(i%3));}
ps("\nProtea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prot_accessory(t,c,1304+(i*11),1295+(i*9),1281+(i*6),1271+(i*3),2023+(i%2));}
ps("\nProtea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prot_market(t,c,1298+(i*9),1289+(i*7),1276+(i*5),1268+(i*3),2024);}
ps("\n");prot_report();prot_state();ps("\n=== Demo Complete ===\n");return 0;}
