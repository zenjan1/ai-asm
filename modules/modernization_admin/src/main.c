/* modernization_admin: Modernization management technology administration (v1.0)
 * Modernization planning, modernization execution, modernization evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mdx_t;
typedef struct{int n_mp,n_me,n_mv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mdx_state_t;
static mdx_t mps[N],mes[N-2],mvs[N-4],acs[N-6],mks[N-6]; static mdx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mdx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mdx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MDX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mdx_init(void){if(init)return -1;st.n_mp=0;st.n_me=0;st.n_mv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mps[i].active=0;for(int i=0;i<N-2;i++)mes[i].active=0;for(int i=0;i<N-4;i++)mvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MDX] Modernization initialized\n");return 0;}
int mdx_planning(int t,int c,int a,int b,int d,int e,int y){return add(mps,&st.n_mp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mdx_execution(int t,int c,int a,int b,int d,int e,int y){return add(mes,&st.n_me,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mdx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mvs,&st.n_mv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mdx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mdx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mdx_report(void){ps("[MDX] Mp: ");pi(st.n_mp);ps(" PCS=");pi(st.t_f1);ps("\nMe: ");pi(st.n_me);ps(" PCS=");pi(st.t_f2);ps("\nMv: ");pi(st.n_mv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mdx_state(void){ps("[MDX] Mp=");pi(st.n_mp);ps(" Me=");pi(st.n_me);ps(" Mv=");pi(st.n_mv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Modernization Admin Demo ===\n\n");mdx_init();
ps("Modernization planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mdx_planning(t,c,478+(i*17),467+(i*14),447+(i*10),429+(i*6),2020+(i%5));}
ps("\nModernization execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mdx_execution(t,c,467+(i*15),456+(i*12),438+(i*8),425+(i*5),2021+(i%4));}
ps("\nModernization evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mdx_evaluation(t,c,459+(i*13),448+(i*10),432+(i*7),421+(i*4),2022+(i%3));}
ps("\nModernization accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdx_accessory(t,c,451+(i*11),442+(i*9),428+(i*6),418+(i*3),2023+(i%2));}
ps("\nModernization marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdx_market(t,c,445+(i*9),436+(i*7),423+(i*5),415+(i*3),2024);}
ps("\n");mdx_report();mdx_state();ps("\n=== Demo Complete ===\n");return 0;}
