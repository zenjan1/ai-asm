/* sugar_admin: Sugar management technology administration (v1.0)
 * Sugar planning, sugar execution, sugar evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sug_t;
typedef struct{int n_sg,n_se,n_sv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sug_state_t;
static sug_t sgs[N],ses[N-2],svs[N-4],acs[N-6],mks[N-6]; static sug_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sug_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sug_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SUG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sug_init(void){if(init)return -1;st.n_sg=0;st.n_se=0;st.n_sv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sgs[i].active=0;for(int i=0;i<N-2;i++)ses[i].active=0;for(int i=0;i<N-4;i++)svs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SUG] Sugar initialized\n");return 0;}
int sug_planning(int t,int c,int a,int b,int d,int e,int y){return add(sgs,&st.n_sg,&st.t_f1,N,t,c,a,b,d,e,y);}
int sug_execution(int t,int c,int a,int b,int d,int e,int y){return add(ses,&st.n_se,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sug_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(svs,&st.n_sv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sug_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sug_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sug_report(void){ps("[SUG] Sg: ");pi(st.n_sg);ps(" PCS=");pi(st.t_f1);ps("\nSe: ");pi(st.n_se);ps(" PCS=");pi(st.t_f2);ps("\nSv: ");pi(st.n_sv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sug_state(void){ps("[SUG] Sg=");pi(st.n_sg);ps(" Se=");pi(st.n_se);ps(" Sv=");pi(st.n_sv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sugar Admin Demo ===\n\n");sug_init();
ps("Sugar planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sug_planning(t,c,522+(i*17),511+(i*14),491+(i*10),473+(i*6),2020+(i%5));}
ps("\nSugar execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sug_execution(t,c,511+(i*15),500+(i*12),482+(i*8),469+(i*5),2021+(i%4));}
ps("\nSugar evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sug_evaluation(t,c,503+(i*13),492+(i*10),476+(i*7),465+(i*4),2022+(i%3));}
ps("\nSugar accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sug_accessory(t,c,495+(i*11),486+(i*9),472+(i*6),462+(i*3),2023+(i%2));}
ps("\nSugar marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sug_market(t,c,489+(i*9),480+(i*7),467+(i*5),459+(i*3),2024);}
ps("\n");sug_report();sug_state();ps("\n=== Demo Complete ===\n");return 0;}
