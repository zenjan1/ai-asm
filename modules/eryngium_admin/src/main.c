/* eryngium_admin: Eryngium management technology administration (v1.0)
 * Eryngium planning, eryngium execution, eryngium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} er_t;
typedef struct{int n_erp,n_ere,n_erv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} er_state_t;
static er_t erps[N],erss[N-2],ervss[N-4],eras[N-6],ermks[N-6]; static er_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(er_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;er_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ERG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int erg_init(void){if(init)return -1;st.n_erp=0;st.n_ere=0;st.n_erv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)erps[i].active=0;for(int i=0;i<N-2;i++)erss[i].active=0;for(int i=0;i<N-4;i++)ervss[i].active=0;for(int i=0;i<N-6;i++)eras[i].active=0;for(int i=0;i<N-6;i++)ermks[i].active=0;init=1;ps("[ERG] Eryngium initialized\n");return 0;}
int erg_planning(int t,int c,int a,int b,int d,int e,int y){return add(erps,&st.n_erp,&st.t_f1,N,t,c,a,b,d,e,y);}
int erg_execution(int t,int c,int a,int b,int d,int e,int y){return add(erss,&st.n_ere,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int erg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ervss,&st.n_erv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int erg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(eras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int erg_market(int t,int c,int a,int b,int d,int e,int y){return add(ermks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void erg_report(void){ps("[ERG] Erp: ");pi(st.n_erp);ps(" PCS=");pi(st.t_f1);ps("\nEre: ");pi(st.n_ere);ps(" PCS=");pi(st.t_f2);ps("\nErv: ");pi(st.n_erv);ps(" PCS=");pi(st.t_f3);ps("\nErc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void erg_state(void){ps("[ERG] Erp=");pi(st.n_erp);ps(" Ere=");pi(st.n_ere);ps(" Erv=");pi(st.n_erv);ps(" Erc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Eryngium Admin Demo ===\n\n");erg_init();
ps("Eryngium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;erg_planning(t,c,771+(i*17),760+(i*14),740+(i*10),722+(i*6),2020+(i%5));}
ps("\nEryngium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;erg_execution(t,c,760+(i*15),749+(i*12),731+(i*8),718+(i*5),2021+(i%4));}
ps("\nEryngium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;erg_evaluation(t,c,752+(i*13),741+(i*10),727+(i*7),716+(i*4),2022+(i%3));}
ps("\nEryngium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;erg_accessory(t,c,744+(i*11),735+(i*9),721+(i*6),711+(i*3),2023+(i%2));}
ps("\nEryngium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;erg_market(t,c,738+(i*9),729+(i*7),716+(i*5),708+(i*3),2024);}
ps("\n");erg_report();erg_state();ps("\n=== Demo Complete ===\n");return 0;}
