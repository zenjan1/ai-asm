/* pennyroyal_admin: Pennyroyal management technology administration (v1.0)
 * Pennyroyal planning, pennyroyal execution, pennyroyal evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} py_t;
typedef struct{int n_pnp,n_pne,n_pnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} py_state_t;
static py_t pnps[N],pnss[N-2],pnvss[N-4],pnas[N-6],pnmks[N-6]; static py_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(py_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;py_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PNR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pn_init(void){if(init)return -1;st.n_pnp=0;st.n_pne=0;st.n_pnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pnps[i].active=0;for(int i=0;i<N-2;i++)pnss[i].active=0;for(int i=0;i<N-4;i++)pnvss[i].active=0;for(int i=0;i<N-6;i++)pnas[i].active=0;for(int i=0;i<N-6;i++)pnmks[i].active=0;init=1;ps("[PNR] Pennyroyal initialized\n");return 0;}
int pn_planning(int t,int c,int a,int b,int d,int e,int y){return add(pnps,&st.n_pnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pn_execution(int t,int c,int a,int b,int d,int e,int y){return add(pnss,&st.n_pne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pnvss,&st.n_pnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pn_market(int t,int c,int a,int b,int d,int e,int y){return add(pnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pn_report(void){ps("[PNR] Pnp: ");pi(st.n_pnp);ps(" PCS=");pi(st.t_f1);ps("\nPne: ");pi(st.n_pne);ps(" PCS=");pi(st.t_f2);ps("\nPnv: ");pi(st.n_pnv);ps(" PCS=");pi(st.t_f3);ps("\nPnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pn_state(void){ps("[PNR] Pnp=");pi(st.n_pnp);ps(" Pne=");pi(st.n_pne);ps(" Pnv=");pi(st.n_pnv);ps(" Pnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pennyroyal Admin Demo ===\n\n");pn_init();
ps("Pennyroyal planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pn_planning(t,c,701+(i*17),690+(i*14),670+(i*10),652+(i*6),2020+(i%5));}
ps("\nPennyroyal execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pn_execution(t,c,690+(i*15),679+(i*12),661+(i*8),648+(i*5),2021+(i%4));}
ps("\nPennyroyal evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pn_evaluation(t,c,682+(i*13),671+(i*10),655+(i*7),644+(i*4),2022+(i%3));}
ps("\nPennyroyal accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pn_accessory(t,c,674+(i*11),665+(i*9),651+(i*6),641+(i*3),2023+(i%2));}
ps("\nPennyroyal marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pn_market(t,c,668+(i*9),659+(i*7),646+(i*5),638+(i*3),2024);}
ps("\n");pn_report();pn_state();ps("\n=== Demo Complete ===\n");return 0;}
