/* xanthosia_admin: Xanthosia management technology administration (v1.0)
 * Xanthosia planning, xanthosia execution, xanthosia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} xa_t;
typedef struct{int n_xap,n_xae,n_xav,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} xa_state_t;
static xa_t xaps[N],xass[N-2],xavss[N-4],xas[N-6],xamks[N-6]; static xa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(xa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;xa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[XAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int xan_init(void){if(init)return -1;st.n_xap=0;st.n_xae=0;st.n_xav=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)xaps[i].active=0;for(int i=0;i<N-2;i++)xass[i].active=0;for(int i=0;i<N-4;i++)xavss[i].active=0;for(int i=0;i<N-6;i++)xas[i].active=0;for(int i=0;i<N-6;i++)xamks[i].active=0;init=1;ps("[XAN] Xanthosia initialized\n");return 0;}
int xan_planning(int t,int c,int a,int b,int d,int e,int y){return add(xaps,&st.n_xap,&st.t_f1,N,t,c,a,b,d,e,y);}
int xan_execution(int t,int c,int a,int b,int d,int e,int y){return add(xass,&st.n_xae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int xan_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(xavss,&st.n_xav,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int xan_accessory(int t,int c,int a,int b,int d,int e,int y){return add(xas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int xan_market(int t,int c,int a,int b,int d,int e,int y){return add(xamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void xan_report(void){ps("[XAN] Xap: ");pi(st.n_xap);ps(" PCS=");pi(st.t_f1);ps("\nXae: ");pi(st.n_xae);ps(" PCS=");pi(st.t_f2);ps("\nXav: ");pi(st.n_xav);ps(" PCS=");pi(st.t_f3);ps("\nXac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void xan_state(void){ps("[XAN] Xap=");pi(st.n_xap);ps(" Xae=");pi(st.n_xae);ps(" Xav=");pi(st.n_xav);ps(" Xac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Xanthosia Admin Demo ===\n\n");xan_init();
ps("Xanthosia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;xan_planning(t,c,754+(i*17),743+(i*14),723+(i*10),705+(i*6),2020+(i%5));}
ps("\nXanthosia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;xan_execution(t,c,743+(i*15),732+(i*12),714+(i*8),701+(i*5),2021+(i%4));}
ps("\nXanthosia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;xan_evaluation(t,c,735+(i*13),724+(i*10),710+(i*7),699+(i*4),2022+(i%3));}
ps("\nXanthosia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xan_accessory(t,c,727+(i*11),718+(i*9),704+(i*6),694+(i*3),2023+(i%2));}
ps("\nXanthosia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xan_market(t,c,721+(i*9),712+(i*7),699+(i*5),691+(i*3),2024);}
ps("\n");xan_report();xan_state();ps("\n=== Demo Complete ===\n");return 0;}
