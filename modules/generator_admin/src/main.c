/* generator_admin: Generator technology administration (v1.0)
 * Synchronous generator, asynchronous generator, DC generator, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gen_t;
typedef struct{int n_sy,n_as,n_dc,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gen_state_t;
static gen_t sys[N],ass[N-2],dcs[N-4],acs[N-6],mks[N-6]; static gen_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gen_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gen_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GEN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gen_init(void){if(init)return -1;st.n_sy=0;st.n_as=0;st.n_dc=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sys[i].active=0;for(int i=0;i<N-2;i++)ass[i].active=0;for(int i=0;i<N-4;i++)dcs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GEN] Generator initialized\n");return 0;}
int gen_synchronous(int t,int c,int a,int b,int d,int e,int y){return add(sys,&st.n_sy,&st.t_f1,N,t,c,a,b,d,e,y);}
int gen_asynchronous(int t,int c,int a,int b,int d,int e,int y){return add(ass,&st.n_as,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gen_dc(int t,int c,int a,int b,int d,int e,int y){return add(dcs,&st.n_dc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gen_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gen_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gen_report(void){ps("[GEN] Sy: ");pi(st.n_sy);ps(" PCS=");pi(st.t_f1);ps("\nAs: ");pi(st.n_as);ps(" PCS=");pi(st.t_f2);ps("\nDc: ");pi(st.n_dc);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gen_state(void){ps("[GEN] Sy=");pi(st.n_sy);ps(" As=");pi(st.n_as);ps(" Dc=");pi(st.n_dc);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Generator Admin Demo ===\n\n");gen_init();
ps("Synchronous generators...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gen_synchronous(t,c,373+(i*17),358+(i*14),338+(i*10),320+(i*6),2020+(i%5));}
ps("\nAsynchronous generators...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gen_asynchronous(t,c,362+(i*15),348+(i*12),330+(i*8),317+(i*5),2021+(i%4));}
ps("\nDC generators...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gen_dc(t,c,354+(i*13),340+(i*10),324+(i*7),313+(i*4),2022+(i%3));}
ps("\nGenerator accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gen_accessory(t,c,346+(i*11),334+(i*9),320+(i*6),310+(i*3),2023+(i%2));}
ps("\nGenerator marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gen_market(t,c,340+(i*9),329+(i*7),316+(i*5),308+(i*3),2024);}
ps("\n");gen_report();gen_state();ps("\n=== Demo Complete ===\n");return 0;}
