/* chionodoxa_admin: Chionodoxa management technology administration (v1.0)
 * Chionodoxa planning, chionodoxa execution, chionodoxa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} chi_t;
typedef struct{int n_chip,n_chie,n_chi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} chi_state_t;
static chi_t chips[N],chiss[N-2],chivss[N-4],chias[N-6],chimks[N-6]; static chi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;chi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int chi_init(void){if(init)return -1;st.n_chip=0;st.n_chie=0;st.n_chi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)chips[i].active=0;for(int i=0;i<N-2;i++)chiss[i].active=0;for(int i=0;i<N-4;i++)chivss[i].active=0;for(int i=0;i<N-6;i++)chias[i].active=0;for(int i=0;i<N-6;i++)chimks[i].active=0;init=1;ps("[CHI] Chionodoxa initialized\n");return 0;}
int chi_planning(int t,int c,int a,int b,int d,int e,int y){return add(chips,&st.n_chip,&st.t_f1,N,t,c,a,b,d,e,y);}
int chi_execution(int t,int c,int a,int b,int d,int e,int y){return add(chiss,&st.n_chie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int chi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(chivss,&st.n_chi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int chi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(chias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int chi_market(int t,int c,int a,int b,int d,int e,int y){return add(chimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void chi_report(void){ps("[CHI] Chipp: ");pi(st.n_chip);ps(" PCS=");pi(st.t_f1);ps("\nChie: ");pi(st.n_chie);ps(" PCS=");pi(st.t_f2);ps("\nChiv: ");pi(st.n_chi2);ps(" PCS=");pi(st.t_f3);ps("\nChic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void chi_state(void){ps("[CHI] Chipp=");pi(st.n_chip);ps(" Chie=");pi(st.n_chie);ps(" Chiv=");pi(st.n_chi2);ps(" Chic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chionodoxa Admin Demo ===\n\n");chi_init();
ps("Chionodoxa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;chi_planning(t,c,983+(i*17),972+(i*14),952+(i*10),934+(i*6),2020+(i%5));}
ps("\nChionodoxa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;chi_execution(t,c,972+(i*15),961+(i*12),943+(i*8),930+(i*5),2021+(i%4));}
ps("\nChionodoxa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;chi_evaluation(t,c,964+(i*13),953+(i*10),937+(i*7),926+(i*4),2022+(i%3));}
ps("\nChionodoxa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chi_accessory(t,c,956+(i*11),947+(i*9),933+(i*6),923+(i*3),2023+(i%2));}
ps("\nChionodoxa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chi_market(t,c,950+(i*9),941+(i*7),928+(i*5),920+(i*3),2024);}
ps("\n");chi_report();chi_state();ps("\n=== Demo Complete ===\n");return 0;}
