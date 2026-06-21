/* horehound_admin: Horehound management technology administration (v1.0)
 * Horehound planning, horehound execution, horehound evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hh_t;
typedef struct{int n_hhp,n_hhe,n_hhv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hh_state_t;
static hh_t hhps[N],hhss[N-2],hhvss[N-4],hhas[N-6],hhmks[N-6]; static hh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hh_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hh_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HRH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hh_init(void){if(init)return -1;st.n_hhp=0;st.n_hhe=0;st.n_hhv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hhps[i].active=0;for(int i=0;i<N-2;i++)hhss[i].active=0;for(int i=0;i<N-4;i++)hhvss[i].active=0;for(int i=0;i<N-6;i++)hhas[i].active=0;for(int i=0;i<N-6;i++)hhmks[i].active=0;init=1;ps("[HRH] Horehound initialized\n");return 0;}
int hh_planning(int t,int c,int a,int b,int d,int e,int y){return add(hhps,&st.n_hhp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hh_execution(int t,int c,int a,int b,int d,int e,int y){return add(hhss,&st.n_hhe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hh_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hhvss,&st.n_hhv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hh_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hhas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hh_market(int t,int c,int a,int b,int d,int e,int y){return add(hhmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hh_report(void){ps("[HRH] Hhp: ");pi(st.n_hhp);ps(" PCS=");pi(st.t_f1);ps("\nHhe: ");pi(st.n_hhe);ps(" PCS=");pi(st.t_f2);ps("\nHhv: ");pi(st.n_hhv);ps(" PCS=");pi(st.t_f3);ps("\nHhc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hh_state(void){ps("[HRH] Hhp=");pi(st.n_hhp);ps(" Hhe=");pi(st.n_hhe);ps(" Hhv=");pi(st.n_hhv);ps(" Hhc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Horehound Admin Demo ===\n\n");hh_init();
ps("Horehound planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hh_planning(t,c,704+(i*17),693+(i*14),673+(i*10),655+(i*6),2020+(i%5));}
ps("\nHorehound execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hh_execution(t,c,693+(i*15),682+(i*12),664+(i*8),651+(i*5),2021+(i%4));}
ps("\nHorehound evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hh_evaluation(t,c,685+(i*13),674+(i*10),658+(i*7),647+(i*4),2022+(i%3));}
ps("\nHorehound accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hh_accessory(t,c,677+(i*11),668+(i*9),654+(i*6),644+(i*3),2023+(i%2));}
ps("\nHorehound marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hh_market(t,c,671+(i*9),662+(i*7),649+(i*5),641+(i*3),2024);}
ps("\n");hh_report();hh_state();ps("\n=== Demo Complete ===\n");return 0;}
