/* edelweiss_admin: Edelweiss management technology administration (v1.0)
 * Edelweiss planning, edelweiss execution, edelweiss evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} edl_t;
typedef struct{int n_edp,n_ede,n_edv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} edl_state_t;
static edl_t edps[N],edss[N-2],edvss[N-4],edas[N-6],edmks[N-6]; static edl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(edl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;edl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EDL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int edl_init(void){if(init)return -1;st.n_edp=0;st.n_ede=0;st.n_edv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)edps[i].active=0;for(int i=0;i<N-2;i++)edss[i].active=0;for(int i=0;i<N-4;i++)edvss[i].active=0;for(int i=0;i<N-6;i++)edas[i].active=0;for(int i=0;i<N-6;i++)edmks[i].active=0;init=1;ps("[EDL] Edelweiss initialized\n");return 0;}
int edl_planning(int t,int c,int a,int b,int d,int e,int y){return add(edps,&st.n_edp,&st.t_f1,N,t,c,a,b,d,e,y);}
int edl_execution(int t,int c,int a,int b,int d,int e,int y){return add(edss,&st.n_ede,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int edl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(edvss,&st.n_edv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int edl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(edas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int edl_market(int t,int c,int a,int b,int d,int e,int y){return add(edmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void edl_report(void){ps("[EDL] Edp: ");pi(st.n_edp);ps(" PCS=");pi(st.t_f1);ps("\nEde: ");pi(st.n_ede);ps(" PCS=");pi(st.t_f2);ps("\nEdv: ");pi(st.n_edv);ps(" PCS=");pi(st.t_f3);ps("\nEdc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void edl_state(void){ps("[EDL] Edp=");pi(st.n_edp);ps(" Ede=");pi(st.n_ede);ps(" Edv=");pi(st.n_edv);ps(" Edc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Edelweiss Admin Demo ===\n\n");edl_init();
ps("Edelweiss planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;edl_planning(t,c,822+(i*17),811+(i*14),791+(i*10),773+(i*6),2020+(i%5));}
ps("\nEdelweiss execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;edl_execution(t,c,811+(i*15),800+(i*12),782+(i*8),769+(i*5),2021+(i%4));}
ps("\nEdelweiss evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;edl_evaluation(t,c,803+(i*13),792+(i*10),778+(i*7),767+(i*4),2022+(i%3));}
ps("\nEdelweiss accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;edl_accessory(t,c,795+(i*11),786+(i*9),772+(i*6),762+(i*3),2023+(i%2));}
ps("\nEdelweiss marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;edl_market(t,c,789+(i*9),780+(i*7),767+(i*5),759+(i*3),2024);}
ps("\n");edl_report();edl_state();ps("\n=== Demo Complete ===\n");return 0;}
