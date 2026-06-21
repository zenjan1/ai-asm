/* plumeria_admin: Plumeria management technology administration (v1.0)
 * Plumeria planning, plumeria execution, plumeria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} plu_t;
typedef struct{int n_plp,n_ple,n_plv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} plu_state_t;
static plu_t plps[N],ples[N-2],plvs[N-4],pcs[N-6],pmks[N-6]; static plu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(plu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;plu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PLU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int plu_init(void){if(init)return -1;st.n_plp=0;st.n_ple=0;st.n_plv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)plps[i].active=0;for(int i=0;i<N-2;i++)ples[i].active=0;for(int i=0;i<N-4;i++)plvs[i].active=0;for(int i=0;i<N-6;i++)pcs[i].active=0;for(int i=0;i<N-6;i++)pmks[i].active=0;init=1;ps("[PLU] Plumeria initialized\n");return 0;}
int plu_planning(int t,int c,int a,int b,int d,int e,int y){return add(plps,&st.n_plp,&st.t_f1,N,t,c,a,b,d,e,y);}
int plu_execution(int t,int c,int a,int b,int d,int e,int y){return add(ples,&st.n_ple,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int plu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(plvs,&st.n_plv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int plu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pcs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int plu_market(int t,int c,int a,int b,int d,int e,int y){return add(pmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void plu_report(void){ps("[PLU] Plp: ");pi(st.n_plp);ps(" PCS=");pi(st.t_f1);ps("\nPle: ");pi(st.n_ple);ps(" PCS=");pi(st.t_f2);ps("\nPlv: ");pi(st.n_plv);ps(" PCS=");pi(st.t_f3);ps("\nPc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void plu_state(void){ps("[PLU] Plp=");pi(st.n_plp);ps(" Ple=");pi(st.n_ple);ps(" Plv=");pi(st.n_plv);ps(" Pc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Plumeria Admin Demo ===\n\n");plu_init();
ps("Plumeria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;plu_planning(t,c,631+(i*17),620+(i*14),600+(i*10),582+(i*6),2020+(i%5));}
ps("\nPlumeria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;plu_execution(t,c,620+(i*15),609+(i*12),591+(i*8),578+(i*5),2021+(i%4));}
ps("\nPlumeria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;plu_evaluation(t,c,612+(i*13),601+(i*10),585+(i*7),574+(i*4),2022+(i%3));}
ps("\nPlumeria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plu_accessory(t,c,604+(i*11),595+(i*9),581+(i*6),571+(i*3),2023+(i%2));}
ps("\nPlumeria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plu_market(t,c,598+(i*9),589+(i*7),576+(i*5),568+(i*3),2024);}
ps("\n");plu_report();plu_state();ps("\n=== Demo Complete ===\n");return 0;}
