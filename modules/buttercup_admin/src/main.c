/* buttercup_admin: Buttercup management technology administration (v1.0)
 * Buttercup planning, buttercup execution, buttercup evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} but_t;
typedef struct{int n_bup,n_bue,n_buv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} but_state_t;
static but_t bups[N],bues[N-2],buvs[N-4],acs[N-6],mks[N-6]; static but_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(but_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;but_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BUT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int but_init(void){if(init)return -1;st.n_bup=0;st.n_bue=0;st.n_buv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bups[i].active=0;for(int i=0;i<N-2;i++)bues[i].active=0;for(int i=0;i<N-4;i++)buvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BUT] Buttercup initialized\n");return 0;}
int but_planning(int t,int c,int a,int b,int d,int e,int y){return add(bups,&st.n_bup,&st.t_f1,N,t,c,a,b,d,e,y);}
int but_execution(int t,int c,int a,int b,int d,int e,int y){return add(bues,&st.n_bue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int but_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(buvs,&st.n_buv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int but_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int but_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void but_report(void){ps("[BUT] Bup: ");pi(st.n_bup);ps(" PCS=");pi(st.t_f1);ps("\nBue: ");pi(st.n_bue);ps(" PCS=");pi(st.t_f2);ps("\nBuv: ");pi(st.n_buv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void but_state(void){ps("[BUT] Bup=");pi(st.n_bup);ps(" Bue=");pi(st.n_bue);ps(" Buv=");pi(st.n_buv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Buttercup Admin Demo ===\n\n");but_init();
ps("Buttercup planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;but_planning(t,c,583+(i*17),572+(i*14),552+(i*10),534+(i*6),2020+(i%5));}
ps("\nButtercup execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;but_execution(t,c,572+(i*15),561+(i*12),543+(i*8),530+(i*5),2021+(i%4));}
ps("\nButtercup evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;but_evaluation(t,c,564+(i*13),553+(i*10),537+(i*7),526+(i*4),2022+(i%3));}
ps("\nButtercup accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;but_accessory(t,c,556+(i*11),547+(i*9),533+(i*6),523+(i*3),2023+(i%2));}
ps("\nButtercup marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;but_market(t,c,550+(i*9),541+(i*7),528+(i*5),520+(i*3),2024);}
ps("\n");but_report();but_state();ps("\n=== Demo Complete ===\n");return 0;}
