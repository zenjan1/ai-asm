/* lawsonia_admin: Lawsonia management technology administration (v1.0)
 * Lawsonia planning, lawsonia execution, lawsonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} law_t;
typedef struct{int n_lawp,n_lawe,n_law2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} law_state_t;
static law_t lawps[N],lawss[N-2],lawvss[N-4],lawas[N-6],lawmks[N-6]; static law_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(law_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;law_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAW] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int law_init(void){if(init)return -1;st.n_lawp=0;st.n_lawe=0;st.n_law2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lawps[i].active=0;for(int i=0;i<N-2;i++)lawss[i].active=0;for(int i=0;i<N-4;i++)lawvss[i].active=0;for(int i=0;i<N-6;i++)lawas[i].active=0;for(int i=0;i<N-6;i++)lawmks[i].active=0;init=1;ps("[LAW] Lawsonia initialized\n");return 0;}
int law_planning(int t,int c,int a,int b,int d,int e,int y){return add(lawps,&st.n_lawp,&st.t_f1,N,t,c,a,b,d,e,y);}
int law_execution(int t,int c,int a,int b,int d,int e,int y){return add(lawss,&st.n_lawe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int law_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lawvss,&st.n_law2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int law_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lawas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int law_market(int t,int c,int a,int b,int d,int e,int y){return add(lawmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void law_report(void){ps("[LAW] Lawp: ");pi(st.n_lawp);ps(" PCS=");pi(st.t_f1);ps("\nLawe: ");pi(st.n_lawe);ps(" PCS=");pi(st.t_f2);ps("\nLawv: ");pi(st.n_law2);ps(" PCS=");pi(st.t_f3);ps("\nLawc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void law_state(void){ps("[LAW] Lawp=");pi(st.n_lawp);ps(" Lawe=");pi(st.n_lawe);ps(" Lawv=");pi(st.n_law2);ps(" Lawc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lawsonia Admin Demo ===\n\n");law_init();
ps("Lawsonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;law_planning(t,c,1047+(i*17),1036+(i*14),1016+(i*10),998+(i*6),2020+(i%5));}
ps("\nLawsonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;law_execution(t,c,1036+(i*15),1025+(i*12),1007+(i*8),994+(i*5),2021+(i%4));}
ps("\nLawsonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;law_evaluation(t,c,1028+(i*13),1017+(i*10),1001+(i*7),990+(i*4),2022+(i%3));}
ps("\nLawsonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;law_accessory(t,c,1020+(i*11),1011+(i*9),997+(i*6),987+(i*3),2023+(i%2));}
ps("\nLawsonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;law_market(t,c,1014+(i*9),1005+(i*7),992+(i*5),984+(i*3),2024);}
ps("\n");law_report();law_state();ps("\n=== Demo Complete ===\n");return 0;}
