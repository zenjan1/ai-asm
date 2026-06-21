/* calceolaria_admin: Calceolaria management technology administration (v1.0)
 * Calceolaria planning, calceolaria execution, calceolaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cal_t;
typedef struct{int n_calp,n_cale,n_cal2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cal_state_t;
static cal_t calps[N],calss[N-2],calvss[N-4],cal_as[N-6],cal_mks[N-6]; static cal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cal_init(void){if(init)return -1;st.n_calp=0;st.n_cale=0;st.n_cal2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)calps[i].active=0;for(int i=0;i<N-2;i++)calss[i].active=0;for(int i=0;i<N-4;i++)calvss[i].active=0;for(int i=0;i<N-6;i++)cal_as[i].active=0;for(int i=0;i<N-6;i++)cal_mks[i].active=0;init=1;ps("[CAL] Calceolaria initialized\n");return 0;}
int cal_planning(int t,int c,int a,int b,int d,int e,int y){return add(calps,&st.n_calp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cal_execution(int t,int c,int a,int b,int d,int e,int y){return add(calss,&st.n_cale,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(calvss,&st.n_cal2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cal_as,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cal_market(int t,int c,int a,int b,int d,int e,int y){return add(cal_mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cal_report(void){ps("[CAL] Calp: ");pi(st.n_calp);ps(" PCS=");pi(st.t_f1);ps("\nCale: ");pi(st.n_cale);ps(" PCS=");pi(st.t_f2);ps("\nCalv: ");pi(st.n_cal2);ps(" PCS=");pi(st.t_f3);ps("\nCalc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cal_state(void){ps("[CAL] Calp=");pi(st.n_calp);ps(" Cale=");pi(st.n_cale);ps(" Calv=");pi(st.n_cal2);ps(" Calc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Calceolaria Admin Demo ===\n\n");cal_init();
ps("Calceolaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cal_planning(t,c,981+(i*17),970+(i*14),950+(i*10),932+(i*6),2020+(i%5));}
ps("\nCalceolaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cal_execution(t,c,970+(i*15),959+(i*12),941+(i*8),928+(i*5),2021+(i%4));}
ps("\nCalceolaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cal_evaluation(t,c,962+(i*13),951+(i*10),935+(i*7),924+(i*4),2022+(i%3));}
ps("\nCalceolaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cal_accessory(t,c,954+(i*11),945+(i*9),931+(i*6),921+(i*3),2023+(i%2));}
ps("\nCalceolaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cal_market(t,c,948+(i*9),939+(i*7),926+(i*5),918+(i*3),2024);}
ps("\n");cal_report();cal_state();ps("\n=== Demo Complete ===\n");return 0;}
