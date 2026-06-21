/* justica_admin: Justica management technology administration (v1.0)
 * Justica planning, justica execution, justica evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jus_t;
typedef struct{int n_jusp,n_juse,n_jus2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jus_state_t;
static jus_t jusps[N],juss[N-2],jusvss[N-4],jusas[N-6],jusmks[N-6]; static jus_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jus_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jus_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JUS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jus_init(void){if(init)return -1;st.n_jusp=0;st.n_juse=0;st.n_jus2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jusps[i].active=0;for(int i=0;i<N-2;i++)juss[i].active=0;for(int i=0;i<N-4;i++)jusvss[i].active=0;for(int i=0;i<N-6;i++)jusas[i].active=0;for(int i=0;i<N-6;i++)jusmks[i].active=0;init=1;ps("[JUS] Justica initialized\n");return 0;}
int jus_planning(int t,int c,int a,int b,int d,int e,int y){return add(jusps,&st.n_jusp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jus_execution(int t,int c,int a,int b,int d,int e,int y){return add(juss,&st.n_juse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jus_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jusvss,&st.n_jus2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jus_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jusas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jus_market(int t,int c,int a,int b,int d,int e,int y){return add(jusmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jus_report(void){ps("[JUS] Jusp: ");pi(st.n_jusp);ps(" PCS=");pi(st.t_f1);ps("\nJuse: ");pi(st.n_juse);ps(" PCS=");pi(st.t_f2);ps("\nJusv: ");pi(st.n_jus2);ps(" PCS=");pi(st.t_f3);ps("\nJusc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jus_state(void){ps("[JUS] Jusp=");pi(st.n_jusp);ps(" Juse=");pi(st.n_juse);ps(" Jusv=");pi(st.n_jus2);ps(" Jusc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Justica Admin Demo ===\n\n");jus_init();
ps("Justica planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jus_planning(t,c,1033+(i*17),1022+(i*14),1002+(i*10),984+(i*6),2020+(i%5));}
ps("\nJustica execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jus_execution(t,c,1022+(i*15),1011+(i*12),993+(i*8),980+(i*5),2021+(i%4));}
ps("\nJustica evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jus_evaluation(t,c,1014+(i*13),1003+(i*10),987+(i*7),976+(i*4),2022+(i%3));}
ps("\nJustica accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jus_accessory(t,c,1006+(i*11),997+(i*9),983+(i*6),973+(i*3),2023+(i%2));}
ps("\nJustica marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jus_market(t,c,1000+(i*9),991+(i*7),978+(i*5),970+(i*3),2024);}
ps("\n");jus_report();jus_state();ps("\n=== Demo Complete ===\n");return 0;}
