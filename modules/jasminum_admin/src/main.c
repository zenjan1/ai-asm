/* jasminum_admin: Jasminum management technology administration (v1.0)
 * Jasminum planning, jasminum execution, jasminum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jas_t;
typedef struct{int n_jasp,n_jase,n_jas2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jas_state_t;
static jas_t jasps[N],jass[N-2],jasvss[N-4],jasas[N-6],jasmks[N-6]; static jas_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jas_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jas_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JAS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jas_init(void){if(init)return -1;st.n_jasp=0;st.n_jase=0;st.n_jas2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jasps[i].active=0;for(int i=0;i<N-2;i++)jass[i].active=0;for(int i=0;i<N-4;i++)jasvss[i].active=0;for(int i=0;i<N-6;i++)jasas[i].active=0;for(int i=0;i<N-6;i++)jasmks[i].active=0;init=1;ps("[JAS] Jasminum initialized\n");return 0;}
int jas_planning(int t,int c,int a,int b,int d,int e,int y){return add(jasps,&st.n_jasp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jas_execution(int t,int c,int a,int b,int d,int e,int y){return add(jass,&st.n_jase,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jas_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jasvss,&st.n_jas2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jas_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jasas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jas_market(int t,int c,int a,int b,int d,int e,int y){return add(jasmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jas_report(void){ps("[JAS] Jasp: ");pi(st.n_jasp);ps(" PCS=");pi(st.t_f1);ps("\nJase: ");pi(st.n_jase);ps(" PCS=");pi(st.t_f2);ps("\nJasv: ");pi(st.n_jas2);ps(" PCS=");pi(st.t_f3);ps("\nJasc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jas_state(void){ps("[JAS] Jasp=");pi(st.n_jasp);ps(" Jase=");pi(st.n_jase);ps(" Jasv=");pi(st.n_jas2);ps(" Jasc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jasminum Admin Demo ===\n\n");jas_init();
ps("Jasminum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jas_planning(t,c,929+(i*17),918+(i*14),898+(i*10),880+(i*6),2020+(i%5));}
ps("\nJasminum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jas_execution(t,c,918+(i*15),907+(i*12),889+(i*8),876+(i*5),2021+(i%4));}
ps("\nJasminum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jas_evaluation(t,c,910+(i*13),899+(i*10),883+(i*7),872+(i*4),2022+(i%3));}
ps("\nJasminum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jas_accessory(t,c,902+(i*11),893+(i*9),879+(i*6),869+(i*3),2023+(i%2));}
ps("\nJasminum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jas_market(t,c,896+(i*9),887+(i*7),874+(i*5),866+(i*3),2024);}
ps("\n");jas_report();jas_state();ps("\n=== Demo Complete ===\n");return 0;}
