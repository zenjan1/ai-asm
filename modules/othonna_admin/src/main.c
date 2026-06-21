/* othonna_admin: Othonna management technology administration (v1.0)
 * Othonna planning, othonna execution, othonna evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oth_t;
typedef struct{int n_othp,n_othe,n_oth2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oth_state_t;
static oth_t othps[N],othss[N-2],othvss[N-4],othas[N-6],othmks[N-6]; static oth_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oth_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oth_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OTH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oth_init(void){if(init)return -1;st.n_othp=0;st.n_othe=0;st.n_oth2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)othps[i].active=0;for(int i=0;i<N-2;i++)othss[i].active=0;for(int i=0;i<N-4;i++)othvss[i].active=0;for(int i=0;i<N-6;i++)othas[i].active=0;for(int i=0;i<N-6;i++)othmks[i].active=0;init=1;ps("[OTH] Othonna initialized\n");return 0;}
int oth_planning(int t,int c,int a,int b,int d,int e,int y){return add(othps,&st.n_othp,&st.t_f1,N,t,c,a,b,d,e,y);}
int oth_execution(int t,int c,int a,int b,int d,int e,int y){return add(othss,&st.n_othe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oth_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(othvss,&st.n_oth2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oth_accessory(int t,int c,int a,int b,int d,int e,int y){return add(othas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oth_market(int t,int c,int a,int b,int d,int e,int y){return add(othmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oth_report(void){ps("[OTH] othp: ");pi(st.n_othp);ps(" PCS=");pi(st.t_f1);ps("\nothe: ");pi(st.n_othe);ps(" PCS=");pi(st.t_f2);ps("\nothv: ");pi(st.n_oth2);ps(" PCS=");pi(st.t_f3);ps("\nothc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oth_state(void){ps("[OTH] othp=");pi(st.n_othp);ps(" othe=");pi(st.n_othe);ps(" othv=");pi(st.n_oth2);ps(" othc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Othonna Admin Demo ===\n\n");oth_init();
ps("Othonna planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oth_planning(t,c,1080+(i*17),1069+(i*14),1049+(i*10),1031+(i*6),2020+(i%5));}
ps("\nOthonna execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oth_execution(t,c,1069+(i*15),1058+(i*12),1040+(i*8),1027+(i*5),2021+(i%4));}
ps("\nOthonna evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oth_evaluation(t,c,1061+(i*13),1050+(i*10),1034+(i*7),1023+(i*4),2022+(i%3));}
ps("\nOthonna accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oth_accessory(t,c,1053+(i*11),1044+(i*9),1030+(i*6),1020+(i*3),2023+(i%2));}
ps("\nOthonna marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oth_market(t,c,1047+(i*9),1038+(i*7),1025+(i*5),1017+(i*3),2024);}
ps("\n");oth_report();oth_state();ps("\n=== Demo Complete ===\n");return 0;}
