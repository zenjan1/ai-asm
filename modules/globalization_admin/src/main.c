/* globalization_admin: Globalization management technology administration (v1.0)
 * Globalization planning, globalization execution, globalization evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} glx_t;
typedef struct{int n_gp,n_ge,n_gv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} glx_state_t;
static glx_t gps[N],ges[N-2],gvs[N-4],acs[N-6],mks[N-6]; static glx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(glx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;glx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GLX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int glx_init(void){if(init)return -1;st.n_gp=0;st.n_ge=0;st.n_gv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gps[i].active=0;for(int i=0;i<N-2;i++)ges[i].active=0;for(int i=0;i<N-4;i++)gvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GLX] Globalization initialized\n");return 0;}
int glx_planning(int t,int c,int a,int b,int d,int e,int y){return add(gps,&st.n_gp,&st.t_f1,N,t,c,a,b,d,e,y);}
int glx_execution(int t,int c,int a,int b,int d,int e,int y){return add(ges,&st.n_ge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int glx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gvs,&st.n_gv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int glx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int glx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void glx_report(void){ps("[GLX] Gp: ");pi(st.n_gp);ps(" PCS=");pi(st.t_f1);ps("\nGe: ");pi(st.n_ge);ps(" PCS=");pi(st.t_f2);ps("\nGv: ");pi(st.n_gv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void glx_state(void){ps("[GLX] Gp=");pi(st.n_gp);ps(" Ge=");pi(st.n_ge);ps(" Gv=");pi(st.n_gv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Globalization Admin Demo ===\n\n");glx_init();
ps("Globalization planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;glx_planning(t,c,486+(i*17),475+(i*14),455+(i*10),437+(i*6),2020+(i%5));}
ps("\nGlobalization execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;glx_execution(t,c,475+(i*15),464+(i*12),446+(i*8),433+(i*5),2021+(i%4));}
ps("\nGlobalization evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;glx_evaluation(t,c,467+(i*13),456+(i*10),440+(i*7),429+(i*4),2022+(i%3));}
ps("\nGlobalization accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;glx_accessory(t,c,459+(i*11),450+(i*9),436+(i*6),426+(i*3),2023+(i%2));}
ps("\nGlobalization marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;glx_market(t,c,453+(i*9),444+(i*7),431+(i*5),423+(i*3),2024);}
ps("\n");glx_report();glx_state();ps("\n=== Demo Complete ===\n");return 0;}
