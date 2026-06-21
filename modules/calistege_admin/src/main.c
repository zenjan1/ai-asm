/* calistege_admin: Calistege management technology administration (v1.0)
 * Calistege planning, calistege execution, calistege evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cal_t;
typedef struct{int n_clp,n_cle,n_clv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cal_state_t;
static cal_t calps[N],cales[N-2],calvs[N-4],acs[N-6],mks[N-6]; static cal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cal_init(void){if(init)return -1;st.n_clp=0;st.n_cle=0;st.n_clv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)calps[i].active=0;for(int i=0;i<N-2;i++)cales[i].active=0;for(int i=0;i<N-4;i++)calvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CAL] Calistege initialized\n");return 0;}
int cal_planning(int t,int c,int a,int b,int d,int e,int y){return add(calps,&st.n_clp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cal_execution(int t,int c,int a,int b,int d,int e,int y){return add(cales,&st.n_cle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(calvs,&st.n_clv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cal_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cal_report(void){ps("[CAL] Clp: ");pi(st.n_clp);ps(" PCS=");pi(st.t_f1);ps("\nCle: ");pi(st.n_cle);ps(" PCS=");pi(st.t_f2);ps("\nClv: ");pi(st.n_clv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cal_state(void){ps("[CAL] Clp=");pi(st.n_clp);ps(" Cle=");pi(st.n_cle);ps(" Clv=");pi(st.n_clv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Calistege Admin Demo ===\n\n");cal_init();
ps("Calistege planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cal_planning(t,c,603+(i*17),592+(i*14),572+(i*10),554+(i*6),2020+(i%5));}
ps("\nCalistege execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cal_execution(t,c,592+(i*15),581+(i*12),563+(i*8),550+(i*5),2021+(i%4));}
ps("\nCalistege evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cal_evaluation(t,c,584+(i*13),573+(i*10),557+(i*7),546+(i*4),2022+(i%3));}
ps("\nCalistege accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cal_accessory(t,c,576+(i*11),567+(i*9),553+(i*6),543+(i*3),2023+(i%2));}
ps("\nCalistege marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cal_market(t,c,570+(i*9),561+(i*7),548+(i*5),540+(i*3),2024);}
ps("\n");cal_report();cal_state();ps("\n=== Demo Complete ===\n");return 0;}
