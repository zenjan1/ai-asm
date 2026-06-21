/* solenostemon_admin: Solenostemon management technology administration (v1.0)
 * Solenostemon planning, solenostemon execution, solenostemon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sln_t;
typedef struct{int n_sln,n_sle,n_slv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sln_state_t;
static sln_t slns[N],slss[N-2],slvss[N-4],slas[N-6],slmks[N-6]; static sln_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sln_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sln_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SLN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sln_init(void){if(init)return -1;st.n_sln=0;st.n_sle=0;st.n_slv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)slns[i].active=0;for(int i=0;i<N-2;i++)slss[i].active=0;for(int i=0;i<N-4;i++)slvss[i].active=0;for(int i=0;i<N-6;i++)slas[i].active=0;for(int i=0;i<N-6;i++)slmks[i].active=0;init=1;ps("[SLN] Solenostemon initialized\n");return 0;}
int sln_planning(int t,int c,int a,int b,int d,int e,int y){return add(slns,&st.n_sln,&st.t_f1,N,t,c,a,b,d,e,y);}
int sln_execution(int t,int c,int a,int b,int d,int e,int y){return add(slss,&st.n_sle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sln_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(slvss,&st.n_slv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sln_accessory(int t,int c,int a,int b,int d,int e,int y){return add(slas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sln_market(int t,int c,int a,int b,int d,int e,int y){return add(slmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sln_report(void){ps("[SLN] Sln: ");pi(st.n_sln);ps(" PCS=");pi(st.t_f1);ps("\nSle: ");pi(st.n_sle);ps(" PCS=");pi(st.t_f2);ps("\nSlv: ");pi(st.n_slv);ps(" PCS=");pi(st.t_f3);ps("\nSlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sln_state(void){ps("[SLN] Sln=");pi(st.n_sln);ps(" Sle=");pi(st.n_sle);ps(" Slv=");pi(st.n_slv);ps(" Slc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Solenostemon Admin Demo ===\n\n");sln_init();
ps("Solenostemon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sln_planning(t,c,802+(i*17),791+(i*14),771+(i*10),753+(i*6),2020+(i%5));}
ps("\nSolenostemon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sln_execution(t,c,791+(i*15),780+(i*12),762+(i*8),749+(i*5),2021+(i%4));}
ps("\nSolenostemon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sln_evaluation(t,c,783+(i*13),772+(i*10),758+(i*7),747+(i*4),2022+(i%3));}
ps("\nSolenostemon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sln_accessory(t,c,775+(i*11),766+(i*9),752+(i*6),742+(i*3),2023+(i%2));}
ps("\nSolenostemon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sln_market(t,c,769+(i*9),760+(i*7),747+(i*5),739+(i*3),2024);}
ps("\n");sln_report();sln_state();ps("\n=== Demo Complete ===\n");return 0;}
