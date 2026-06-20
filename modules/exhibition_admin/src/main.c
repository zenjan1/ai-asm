/* exhibition_admin: Exhibition management technology administration (v1.0)
 * Exhibition planning, exhibition execution, exhibition evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} exx_t;
typedef struct{int n_epl,n_ex,n_evl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} exx_state_t;
static exx_t epls[N],exs[N-2],evls[N-4],acs[N-6],mks[N-6]; static exx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(exx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;exx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EXX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int exx_init(void){if(init)return -1;st.n_epl=0;st.n_ex=0;st.n_evl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)epls[i].active=0;for(int i=0;i<N-2;i++)exs[i].active=0;for(int i=0;i<N-4;i++)evls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[EXX] Exhibition initialized\n");return 0;}
int exx_planning(int t,int c,int a,int b,int d,int e,int y){return add(epls,&st.n_epl,&st.t_f1,N,t,c,a,b,d,e,y);}
int exx_execution(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int exx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(evls,&st.n_evl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int exx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int exx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void exx_report(void){ps("[EXX] Epl: ");pi(st.n_epl);ps(" PCS=");pi(st.t_f1);ps("\nEx: ");pi(st.n_ex);ps(" PCS=");pi(st.t_f2);ps("\nEvl: ");pi(st.n_evl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void exx_state(void){ps("[EXX] Epl=");pi(st.n_epl);ps(" Ex=");pi(st.n_ex);ps(" Evl=");pi(st.n_evl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Exhibition Admin Demo ===\n\n");exx_init();
ps("Exhibition planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;exx_planning(t,c,467+(i*17),456+(i*14),436+(i*10),418+(i*6),2020+(i%5));}
ps("\nExhibition execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;exx_execution(t,c,456+(i*15),445+(i*12),427+(i*8),414+(i*5),2021+(i%4));}
ps("\nExhibition evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;exx_evaluation(t,c,448+(i*13),437+(i*10),421+(i*7),410+(i*4),2022+(i%3));}
ps("\nExhibition accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;exx_accessory(t,c,440+(i*11),431+(i*9),417+(i*6),407+(i*3),2023+(i%2));}
ps("\nExhibition marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;exx_market(t,c,434+(i*9),425+(i*7),412+(i*5),404+(i*3),2024);}
ps("\n");exx_report();exx_state();ps("\n=== Demo Complete ===\n");return 0;}
