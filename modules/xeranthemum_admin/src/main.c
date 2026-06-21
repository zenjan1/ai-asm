/* xeranthemum_admin: Xeranthemum management technology administration (v1.0)
 * Xeranthemum planning, xeranthemum execution, xeranthemum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} xe_t;
typedef struct{int n_xep,n_xee,n_xev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} xe_state_t;
static xe_t xeps[N],xess[N-2],xevss[N-4],xees[N-6],xemks[N-6]; static xe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(xe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;xe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[XER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int xer_init(void){if(init)return -1;st.n_xep=0;st.n_xee=0;st.n_xev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)xeps[i].active=0;for(int i=0;i<N-2;i++)xess[i].active=0;for(int i=0;i<N-4;i++)xevss[i].active=0;for(int i=0;i<N-6;i++)xees[i].active=0;for(int i=0;i<N-6;i++)xemks[i].active=0;init=1;ps("[XER] Xeranthemum initialized\n");return 0;}
int xer_planning(int t,int c,int a,int b,int d,int e,int y){return add(xeps,&st.n_xep,&st.t_f1,N,t,c,a,b,d,e,y);}
int xer_execution(int t,int c,int a,int b,int d,int e,int y){return add(xess,&st.n_xee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int xer_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(xevss,&st.n_xev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int xer_accessory(int t,int c,int a,int b,int d,int e,int y){return add(xees,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int xer_market(int t,int c,int a,int b,int d,int e,int y){return add(xemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void xer_report(void){ps("[XER] Xep: ");pi(st.n_xep);ps(" PCS=");pi(st.t_f1);ps("\nXee: ");pi(st.n_xee);ps(" PCS=");pi(st.t_f2);ps("\nXev: ");pi(st.n_xev);ps(" PCS=");pi(st.t_f3);ps("\nXec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void xer_state(void){ps("[XER] Xep=");pi(st.n_xep);ps(" Xee=");pi(st.n_xee);ps(" Xev=");pi(st.n_xev);ps(" Xec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Xeranthemum Admin Demo ===\n\n");xer_init();
ps("Xeranthemum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;xer_planning(t,c,755+(i*17),744+(i*14),724+(i*10),706+(i*6),2020+(i%5));}
ps("\nXeranthemum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;xer_execution(t,c,744+(i*15),733+(i*12),715+(i*8),702+(i*5),2021+(i%4));}
ps("\nXeranthemum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;xer_evaluation(t,c,736+(i*13),725+(i*10),711+(i*7),700+(i*4),2022+(i%3));}
ps("\nXeranthemum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xer_accessory(t,c,728+(i*11),719+(i*9),705+(i*6),695+(i*3),2023+(i%2));}
ps("\nXeranthemum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xer_market(t,c,722+(i*9),713+(i*7),700+(i*5),692+(i*3),2024);}
ps("\n");xer_report();xer_state();ps("\n=== Demo Complete ===\n");return 0;}
