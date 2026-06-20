/* hollyhock_admin: Hollyhock management technology administration (v1.0)
 * Hollyhock planning, hollyhock execution, hollyhock evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hol_t;
typedef struct{int n_hhp,n_hhe,n_hhv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hol_state_t;
static hol_t hhps[N],hhes[N-2],hhvs[N-4],acs[N-6],mks[N-6]; static hol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hol_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hol_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HOL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hol_init(void){if(init)return -1;st.n_hhp=0;st.n_hhe=0;st.n_hhv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hhps[i].active=0;for(int i=0;i<N-2;i++)hhes[i].active=0;for(int i=0;i<N-4;i++)hhvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HOL] Hollyhock initialized\n");return 0;}
int hol_planning(int t,int c,int a,int b,int d,int e,int y){return add(hhps,&st.n_hhp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hol_execution(int t,int c,int a,int b,int d,int e,int y){return add(hhes,&st.n_hhe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hol_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hhvs,&st.n_hhv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hol_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hol_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hol_report(void){ps("[HOL] Hhp: ");pi(st.n_hhp);ps(" PCS=");pi(st.t_f1);ps("\nHhe: ");pi(st.n_hhe);ps(" PCS=");pi(st.t_f2);ps("\nHhv: ");pi(st.n_hhv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hol_state(void){ps("[HOL] Hhp=");pi(st.n_hhp);ps(" Hhe=");pi(st.n_hhe);ps(" Hhv=");pi(st.n_hhv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hollyhock Admin Demo ===\n\n");hol_init();
ps("Hollyhock planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hol_planning(t,c,578+(i*17),567+(i*14),547+(i*10),529+(i*6),2020+(i%5));}
ps("\nHollyhock execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hol_execution(t,c,567+(i*15),556+(i*12),538+(i*8),525+(i*5),2021+(i%4));}
ps("\nHollyhock evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hol_evaluation(t,c,559+(i*13),548+(i*10),532+(i*7),521+(i*4),2022+(i%3));}
ps("\nHollyhock accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hol_accessory(t,c,551+(i*11),542+(i*9),528+(i*6),518+(i*3),2023+(i%2));}
ps("\nHollyhock marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hol_market(t,c,545+(i*9),536+(i*7),523+(i*5),515+(i*3),2024);}
ps("\n");hol_report();hol_state();ps("\n=== Demo Complete ===\n");return 0;}
