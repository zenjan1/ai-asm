/* fenugreek_admin: Fenugreek management technology administration (v1.0)
 * Fenugreek planning, fenugreek execution, fenugreek evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fn_t;
typedef struct{int n_fnp,n_fne,n_fnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fn_state_t;
static fn_t fnps[N],fnss[N-2],fnvss[N-4],fnas[N-6],fnmks[N-6]; static fn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FNK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fn_init(void){if(init)return -1;st.n_fnp=0;st.n_fne=0;st.n_fnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fnps[i].active=0;for(int i=0;i<N-2;i++)fnss[i].active=0;for(int i=0;i<N-4;i++)fnvss[i].active=0;for(int i=0;i<N-6;i++)fnas[i].active=0;for(int i=0;i<N-6;i++)fnmks[i].active=0;init=1;ps("[FNK] Fenugreek initialized\n");return 0;}
int fn_planning(int t,int c,int a,int b,int d,int e,int y){return add(fnps,&st.n_fnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fn_execution(int t,int c,int a,int b,int d,int e,int y){return add(fnss,&st.n_fne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fnvss,&st.n_fnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fn_market(int t,int c,int a,int b,int d,int e,int y){return add(fnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fn_report(void){ps("[FNK] Fnp: ");pi(st.n_fnp);ps(" PCS=");pi(st.t_f1);ps("\nFne: ");pi(st.n_fne);ps(" PCS=");pi(st.t_f2);ps("\nFnv: ");pi(st.n_fnv);ps(" PCS=");pi(st.t_f3);ps("\nFnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fn_state(void){ps("[FNK] Fnp=");pi(st.n_fnp);ps(" Fne=");pi(st.n_fne);ps(" Fnv=");pi(st.n_fnv);ps(" Fnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fenugreek Admin Demo ===\n\n");fn_init();
ps("Fenugreek planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fn_planning(t,c,680+(i*17),669+(i*14),649+(i*10),631+(i*6),2020+(i%5));}
ps("\nFenugreek execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fn_execution(t,c,669+(i*15),658+(i*12),640+(i*8),627+(i*5),2021+(i%4));}
ps("\nFenugreek evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fn_evaluation(t,c,661+(i*13),650+(i*10),634+(i*7),623+(i*4),2022+(i%3));}
ps("\nFenugreek accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fn_accessory(t,c,653+(i*11),644+(i*9),630+(i*6),620+(i*3),2023+(i%2));}
ps("\nFenugreek marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fn_market(t,c,647+(i*9),638+(i*7),625+(i*5),617+(i*3),2024);}
ps("\n");fn_report();fn_state();ps("\n=== Demo Complete ===\n");return 0;}
