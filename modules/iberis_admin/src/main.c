/* iberis_admin: Iberis management technology administration (v1.0)
 * Iberis planning, iberis execution, iberis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ibe_t;
typedef struct{int n_ibep,n_ibe,n_ibe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ibe_state_t;
static ibe_t ibeps[N],ibess[N-2],ibevss[N-4],ibeas[N-6],ibemks[N-6]; static ibe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ibe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ibe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IBE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ibe_init(void){if(init)return -1;st.n_ibep=0;st.n_ibe=0;st.n_ibe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ibeps[i].active=0;for(int i=0;i<N-2;i++)ibess[i].active=0;for(int i=0;i<N-4;i++)ibevss[i].active=0;for(int i=0;i<N-6;i++)ibeas[i].active=0;for(int i=0;i<N-6;i++)ibemks[i].active=0;init=1;ps("[IBE] Iberis initialized\n");return 0;}
int ibe_planning(int t,int c,int a,int b,int d,int e,int y){return add(ibeps,&st.n_ibep,&st.t_f1,N,t,c,a,b,d,e,y);}
int ibe_execution(int t,int c,int a,int b,int d,int e,int y){return add(ibess,&st.n_ibe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ibe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ibevss,&st.n_ibe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ibe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ibeas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ibe_market(int t,int c,int a,int b,int d,int e,int y){return add(ibemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ibe_report(void){ps("[IBE] Ibep: ");pi(st.n_ibep);ps(" PCS=");pi(st.t_f1);ps("\nIbee: ");pi(st.n_ibe);ps(" PCS=");pi(st.t_f2);ps("\nIbev: ");pi(st.n_ibe2);ps(" PCS=");pi(st.t_f3);ps("\nIbec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ibe_state(void){ps("[IBE] Ibep=");pi(st.n_ibep);ps(" Ibee=");pi(st.n_ibe);ps(" Ibev=");pi(st.n_ibe2);ps(" Ibec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Iberis Admin Demo ===\n\n");ibe_init();
ps("Iberis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ibe_planning(t,c,928+(i*17),917+(i*14),897+(i*10),879+(i*6),2020+(i%5));}
ps("\nIberis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ibe_execution(t,c,917+(i*15),906+(i*12),888+(i*8),875+(i*5),2021+(i%4));}
ps("\nIberis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ibe_evaluation(t,c,909+(i*13),898+(i*10),882+(i*7),871+(i*4),2022+(i%3));}
ps("\nIberis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ibe_accessory(t,c,901+(i*11),892+(i*9),878+(i*6),868+(i*3),2023+(i%2));}
ps("\nIberis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ibe_market(t,c,895+(i*9),886+(i*7),873+(i*5),865+(i*3),2024);}
ps("\n");ibe_report();ibe_state();ps("\n=== Demo Complete ===\n");return 0;}
