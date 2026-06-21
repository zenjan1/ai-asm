/* panicum_admin: Panicum management technology administration (v1.0)
 * Panicum planning, panicum execution, panicum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pnc_t;
typedef struct{int n_pnp,n_pne,n_pnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pnc_state_t;
static pnc_t pncps[N],pncss[N-2],pncvss[N-4],pncas[N-6],pncmks[N-6]; static pnc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pnc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pnc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PNC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pnc_init(void){if(init)return -1;st.n_pnp=0;st.n_pne=0;st.n_pnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pncps[i].active=0;for(int i=0;i<N-2;i++)pncss[i].active=0;for(int i=0;i<N-4;i++)pncvss[i].active=0;for(int i=0;i<N-6;i++)pncas[i].active=0;for(int i=0;i<N-6;i++)pncmks[i].active=0;init=1;ps("[PNC] Panicum initialized\n");return 0;}
int pnc_planning(int t,int c,int a,int b,int d,int e,int y){return add(pncps,&st.n_pnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pnc_execution(int t,int c,int a,int b,int d,int e,int y){return add(pncss,&st.n_pne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pnc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pncvss,&st.n_pnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pnc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pncas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pnc_market(int t,int c,int a,int b,int d,int e,int y){return add(pncmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pnc_report(void){ps("[PNC] Pnp: ");pi(st.n_pnp);ps(" PCS=");pi(st.t_f1);ps("\nPne: ");pi(st.n_pne);ps(" PCS=");pi(st.t_f2);ps("\nPnv: ");pi(st.n_pnv);ps(" PCS=");pi(st.t_f3);ps("\nPnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pnc_state(void){ps("[PNC] Pnp=");pi(st.n_pnp);ps(" Pne=");pi(st.n_pne);ps(" Pnv=");pi(st.n_pnv);ps(" Pnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Panicum Admin Demo ===\n\n");pnc_init();
ps("Panicum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pnc_planning(t,c,850+(i*17),839+(i*14),819+(i*10),801+(i*6),2020+(i%5));}
ps("\nPanicum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pnc_execution(t,c,839+(i*15),828+(i*12),810+(i*8),797+(i*5),2021+(i%4));}
ps("\nPanicum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pnc_evaluation(t,c,831+(i*13),820+(i*10),804+(i*7),793+(i*4),2022+(i%3));}
ps("\nPanicum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnc_accessory(t,c,823+(i*11),814+(i*9),800+(i*6),790+(i*3),2023+(i%2));}
ps("\nPanicum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnc_market(t,c,817+(i*9),808+(i*7),795+(i*5),787+(i*3),2024);}
ps("\n");pnc_report();pnc_state();ps("\n=== Demo Complete ===\n");return 0;}
