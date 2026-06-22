/* hoffmannia_admin: Hoffmannia management technology administration (v1.0)
 * Hoffmannia planning, hoffmannia execution, hoffmannia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hof_t;
typedef struct{int n_hofp,n_hofe,n_hof2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hof_state_t;
static hof_t hofps[N],hofss[N-2],hofvss[N-4],hofas[N-6],hofmks[N-6]; static hof_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hof_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hof_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HOF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hof_init(void){if(init)return -1;st.n_hofp=0;st.n_hofe=0;st.n_hof2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hofps[i].active=0;for(int i=0;i<N-2;i++)hofss[i].active=0;for(int i=0;i<N-4;i++)hofvss[i].active=0;for(int i=0;i<N-6;i++)hofas[i].active=0;for(int i=0;i<N-6;i++)hofmks[i].active=0;init=1;ps("[HOF] Hoffmannia initialized\n");return 0;}
int hof_planning(int t,int c,int a,int b,int d,int e,int y){return add(hofps,&st.n_hofp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hof_execution(int t,int c,int a,int b,int d,int e,int y){return add(hofss,&st.n_hofe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hof_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hofvss,&st.n_hof2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hof_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hofas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hof_market(int t,int c,int a,int b,int d,int e,int y){return add(hofmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hof_report(void){ps("[HOF] Hofp: ");pi(st.n_hofp);ps(" PCS=");pi(st.t_f1);ps("\nHofe: ");pi(st.n_hofe);ps(" PCS=");pi(st.t_f2);ps("\nHofv: ");pi(st.n_hof2);ps(" PCS=");pi(st.t_f3);ps("\nHofc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hof_state(void){ps("[HOF] Hofp=");pi(st.n_hofp);ps(" Hofe=");pi(st.n_hofe);ps(" Hofv=");pi(st.n_hof2);ps(" Hofc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hoffmannia Admin Demo ===\n\n");hof_init();
ps("Hoffmannia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hof_planning(t,c,1293+(i*17),1282+(i*14),1262+(i*10),1244+(i*6),2020+(i%5));}
ps("\nHoffmannia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hof_execution(t,c,1282+(i*15),1271+(i*12),1253+(i*8),1240+(i*5),2021+(i%4));}
ps("\nHoffmannia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hof_evaluation(t,c,1274+(i*13),1263+(i*10),1247+(i*7),1236+(i*4),2022+(i%3));}
ps("\nHoffmannia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hof_accessory(t,c,1266+(i*11),1257+(i*9),1243+(i*6),1233+(i*3),2023+(i%2));}
ps("\nHoffmannia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hof_market(t,c,1260+(i*9),1251+(i*7),1238+(i*5),1230+(i*3),2024);}
ps("\n");hof_report();hof_state();ps("\n=== Demo Complete ===\n");return 0;}
