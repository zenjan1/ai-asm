/* daffodil_admin: Daffodil management technology administration (v1.0)
 * Daffodil planning, daffodil execution, daffodil evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} daf_t;
typedef struct{int n_dp,n_de,n_dv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} daf_state_t;
static daf_t dps[N],des[N-2],dvs[N-4],acs[N-6],mks[N-6]; static daf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(daf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;daf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DAF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int daf_init(void){if(init)return -1;st.n_dp=0;st.n_de=0;st.n_dv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dps[i].active=0;for(int i=0;i<N-2;i++)des[i].active=0;for(int i=0;i<N-4;i++)dvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DAF] Daffodil initialized\n");return 0;}
int daf_planning(int t,int c,int a,int b,int d,int e,int y){return add(dps,&st.n_dp,&st.t_f1,N,t,c,a,b,d,e,y);}
int daf_execution(int t,int c,int a,int b,int d,int e,int y){return add(des,&st.n_de,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int daf_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dvs,&st.n_dv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int daf_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int daf_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void daf_report(void){ps("[DAF] Dp: ");pi(st.n_dp);ps(" PCS=");pi(st.t_f1);ps("\nDe: ");pi(st.n_de);ps(" PCS=");pi(st.t_f2);ps("\nDv: ");pi(st.n_dv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void daf_state(void){ps("[DAF] Dp=");pi(st.n_dp);ps(" De=");pi(st.n_de);ps(" Dv=");pi(st.n_dv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Daffodil Admin Demo ===\n\n");daf_init();
ps("Daffodil planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;daf_planning(t,c,563+(i*17),552+(i*14),532+(i*10),514+(i*6),2020+(i%5));}
ps("\nDaffodil execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;daf_execution(t,c,552+(i*15),541+(i*12),523+(i*8),510+(i*5),2021+(i%4));}
ps("\nDaffodil evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;daf_evaluation(t,c,544+(i*13),533+(i*10),517+(i*7),506+(i*4),2022+(i%3));}
ps("\nDaffodil accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;daf_accessory(t,c,536+(i*11),527+(i*9),513+(i*6),503+(i*3),2023+(i%2));}
ps("\nDaffodil marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;daf_market(t,c,530+(i*9),521+(i*7),508+(i*5),500+(i*3),2024);}
ps("\n");daf_report();daf_state();ps("\n=== Demo Complete ===\n");return 0;}
