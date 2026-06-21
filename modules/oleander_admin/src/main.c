/* oleander_admin: Oleander management technology administration (v1.0)
 * Oleander planning, oleander execution, oleander evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ole_t;
typedef struct{int n_olp,n_ole,n_olv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ole_state_t;
static ole_t olps[N],oles[N-2],olvs[N-4],acs[N-6],mks[N-6]; static ole_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ole_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ole_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OLE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ole_init(void){if(init)return -1;st.n_olp=0;st.n_ole=0;st.n_olv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)olps[i].active=0;for(int i=0;i<N-2;i++)oles[i].active=0;for(int i=0;i<N-4;i++)olvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[OLE] Oleander initialized\n");return 0;}
int ole_planning(int t,int c,int a,int b,int d,int e,int y){return add(olps,&st.n_olp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ole_execution(int t,int c,int a,int b,int d,int e,int y){return add(oles,&st.n_ole,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ole_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(olvs,&st.n_olv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ole_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ole_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ole_report(void){ps("[OLE] Olp: ");pi(st.n_olp);ps(" PCS=");pi(st.t_f1);ps("\nOle: ");pi(st.n_ole);ps(" PCS=");pi(st.t_f2);ps("\nOlv: ");pi(st.n_olv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ole_state(void){ps("[OLE] Olp=");pi(st.n_olp);ps(" Ole=");pi(st.n_ole);ps(" Olv=");pi(st.n_olv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oleander Admin Demo ===\n\n");ole_init();
ps("Oleander planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ole_planning(t,c,592+(i*17),581+(i*14),561+(i*10),543+(i*6),2020+(i%5));}
ps("\nOleander execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ole_execution(t,c,581+(i*15),570+(i*12),552+(i*8),539+(i*5),2021+(i%4));}
ps("\nOleander evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ole_evaluation(t,c,573+(i*13),562+(i*10),546+(i*7),535+(i*4),2022+(i%3));}
ps("\nOleander accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ole_accessory(t,c,565+(i*11),556+(i*9),542+(i*6),532+(i*3),2023+(i%2));}
ps("\nOleander marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ole_market(t,c,559+(i*9),550+(i*7),537+(i*5),529+(i*3),2024);}
ps("\n");ole_report();ole_state();ps("\n=== Demo Complete ===\n");return 0;}
