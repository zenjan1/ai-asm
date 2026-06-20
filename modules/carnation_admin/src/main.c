/* carnation_admin: Carnation management technology administration (v1.0)
 * Carnation planning, carnation execution, carnation evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} car_t;
typedef struct{int n_cp,n_ce,n_cv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} car_state_t;
static car_t cps[N],ces[N-2],cvs[N-4],acs[N-6],mks[N-6]; static car_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(car_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;car_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int car_init(void){if(init)return -1;st.n_cp=0;st.n_ce=0;st.n_cv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cps[i].active=0;for(int i=0;i<N-2;i++)ces[i].active=0;for(int i=0;i<N-4;i++)cvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CAR] Carnation initialized\n");return 0;}
int car_planning(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f1,N,t,c,a,b,d,e,y);}
int car_execution(int t,int c,int a,int b,int d,int e,int y){return add(ces,&st.n_ce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int car_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cvs,&st.n_cv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int car_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int car_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void car_report(void){ps("[CAR] Cp: ");pi(st.n_cp);ps(" PCS=");pi(st.t_f1);ps("\nCe: ");pi(st.n_ce);ps(" PCS=");pi(st.t_f2);ps("\nCv: ");pi(st.n_cv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void car_state(void){ps("[CAR] Cp=");pi(st.n_cp);ps(" Ce=");pi(st.n_ce);ps(" Cv=");pi(st.n_cv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Carnation Admin Demo ===\n\n");car_init();
ps("Carnation planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;car_planning(t,c,550+(i*17),539+(i*14),519+(i*10),501+(i*6),2020+(i%5));}
ps("\nCarnation execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;car_execution(t,c,539+(i*15),528+(i*12),510+(i*8),497+(i*5),2021+(i%4));}
ps("\nCarnation evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;car_evaluation(t,c,531+(i*13),520+(i*10),504+(i*7),493+(i*4),2022+(i%3));}
ps("\nCarnation accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;car_accessory(t,c,523+(i*11),514+(i*9),500+(i*6),490+(i*3),2023+(i%2));}
ps("\nCarnation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;car_market(t,c,517+(i*9),508+(i*7),495+(i*5),487+(i*3),2024);}
ps("\n");car_report();car_state();ps("\n=== Demo Complete ===\n");return 0;}
