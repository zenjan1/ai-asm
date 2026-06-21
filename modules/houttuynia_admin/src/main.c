/* houttuynia_admin: Houttuynia management technology administration (v1.0)
 * Houttuynia planning, houttuynia execution, houttuynia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hou_t;
typedef struct{int n_houp,n_houe,n_hou2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hou_state_t;
static hou_t hups[N],huss[N-2],huvss[N-4],huas[N-6],humks[N-6]; static hou_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hou_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hou_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HOU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hou_init(void){if(init)return -1;st.n_houp=0;st.n_houe=0;st.n_hou2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hups[i].active=0;for(int i=0;i<N-2;i++)huss[i].active=0;for(int i=0;i<N-4;i++)huvss[i].active=0;for(int i=0;i<N-6;i++)huas[i].active=0;for(int i=0;i<N-6;i++)humks[i].active=0;init=1;ps("[HOU] Houttuynia initialized\n");return 0;}
int hou_planning(int t,int c,int a,int b,int d,int e,int y){return add(hups,&st.n_houp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hou_execution(int t,int c,int a,int b,int d,int e,int y){return add(huss,&st.n_houe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hou_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(huvss,&st.n_hou2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hou_accessory(int t,int c,int a,int b,int d,int e,int y){return add(huas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hou_market(int t,int c,int a,int b,int d,int e,int y){return add(humks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hou_report(void){ps("[HOU] Houp: ");pi(st.n_houp);ps(" PCS=");pi(st.t_f1);ps("\nHoue: ");pi(st.n_houe);ps(" PCS=");pi(st.t_f2);ps("\nHouv: ");pi(st.n_hou2);ps(" PCS=");pi(st.t_f3);ps("\nHouc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hou_state(void){ps("[HOU] Houp=");pi(st.n_houp);ps(" Houe=");pi(st.n_houe);ps(" Houv=");pi(st.n_hou2);ps(" Houc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Houttuynia Admin Demo ===\n\n");hou_init();
ps("Houttuynia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hou_planning(t,c,927+(i*17),916+(i*14),896+(i*10),878+(i*6),2020+(i%5));}
ps("\nHouttuynia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hou_execution(t,c,916+(i*15),905+(i*12),887+(i*8),874+(i*5),2021+(i%4));}
ps("\nHouttuynia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hou_evaluation(t,c,908+(i*13),897+(i*10),881+(i*7),870+(i*4),2022+(i%3));}
ps("\nHouttuynia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hou_accessory(t,c,900+(i*11),891+(i*9),877+(i*6),867+(i*3),2023+(i%2));}
ps("\nHouttuynia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hou_market(t,c,894+(i*9),885+(i*7),872+(i*5),864+(i*3),2024);}
ps("\n");hou_report();hou_state();ps("\n=== Demo Complete ===\n");return 0;}
