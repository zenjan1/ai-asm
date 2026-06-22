/* oxydendrum_admin: Oxydendrum management technology administration (v1.0)
 * Oxydendrum planning, oxydendrum execution, oxydendrum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oxy_t;
typedef struct{int n_oxyp,n_oxye,n_oxy2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oxy_state_t;
static oxy_t oxyps[N],oxyss[N-2],oxyvss[N-4],oxyas[N-6],oxymks[N-6]; static oxy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oxy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oxy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OXY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oxy_init(void){if(init)return -1;st.n_oxyp=0;st.n_oxye=0;st.n_oxy2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)oxyps[i].active=0;for(int i=0;i<N-2;i++)oxyss[i].active=0;for(int i=0;i<N-4;i++)oxyvss[i].active=0;for(int i=0;i<N-6;i++)oxyas[i].active=0;for(int i=0;i<N-6;i++)oxymks[i].active=0;init=1;ps("[OXY] Oxydendrum initialized\n");return 0;}
int oxy_planning(int t,int c,int a,int b,int d,int e,int y){return add(oxyps,&st.n_oxyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int oxy_execution(int t,int c,int a,int b,int d,int e,int y){return add(oxyss,&st.n_oxye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oxy_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(oxyvss,&st.n_oxy2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oxy_accessory(int t,int c,int a,int b,int d,int e,int y){return add(oxyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oxy_market(int t,int c,int a,int b,int d,int e,int y){return add(oxymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oxy_report(void){ps("[OXY] Oxypp: ");pi(st.n_oxyp);ps(" PCS=");pi(st.t_f1);ps("\nOxye: ");pi(st.n_oxye);ps(" PCS=");pi(st.t_f2);ps("\nOxyv: ");pi(st.n_oxy2);ps(" PCS=");pi(st.t_f3);ps("\nOxyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oxy_state(void){ps("[OXY] Oxypp=");pi(st.n_oxyp);ps(" Oxye=");pi(st.n_oxye);ps(" Oxyv=");pi(st.n_oxy2);ps(" Oxyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oxydendrum Admin Demo ===\n\n");oxy_init();
ps("Oxydendrum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oxy_planning(t,c,1165+(i*17),1154+(i*14),1134+(i*10),1116+(i*6),2020+(i%5));}
ps("\nOxydendrum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oxy_execution(t,c,1154+(i*15),1143+(i*12),1125+(i*8),1112+(i*5),2021+(i%4));}
ps("\nOxydendrum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oxy_evaluation(t,c,1146+(i*13),1135+(i*10),1119+(i*7),1108+(i*4),2022+(i%3));}
ps("\nOxydendrum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oxy_accessory(t,c,1138+(i*11),1129+(i*9),1115+(i*6),1105+(i*3),2023+(i%2));}
ps("\nOxydendrum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oxy_market(t,c,1132+(i*9),1123+(i*7),1110+(i*5),1102+(i*3),2024);}
ps("\n");oxy_report();oxy_state();ps("\n=== Demo Complete ===\n");return 0;}
