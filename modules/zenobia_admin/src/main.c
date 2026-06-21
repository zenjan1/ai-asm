/* zenobia_admin: Zenobia management technology administration (v1.0)
 * Zenobia planning, zenobia execution, zenobia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zen_t;
typedef struct{int n_zenp,n_zene,n_zen2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zen_state_t;
static zen_t zenps[N],zenss[N-2],zenvss[N-4],zenas[N-6],zenmks[N-6]; static zen_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zen_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zen_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZEN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zen_init(void){if(init)return -1;st.n_zenp=0;st.n_zene=0;st.n_zen2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zenps[i].active=0;for(int i=0;i<N-2;i++)zenss[i].active=0;for(int i=0;i<N-4;i++)zenvss[i].active=0;for(int i=0;i<N-6;i++)zenas[i].active=0;for(int i=0;i<N-6;i++)zenmks[i].active=0;init=1;ps("[ZEN] Zenobia initialized\n");return 0;}
int zen_planning(int t,int c,int a,int b,int d,int e,int y){return add(zenps,&st.n_zenp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zen_execution(int t,int c,int a,int b,int d,int e,int y){return add(zenss,&st.n_zene,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zen_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zenvss,&st.n_zen2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zen_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zenas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zen_market(int t,int c,int a,int b,int d,int e,int y){return add(zenmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zen_report(void){ps("[ZEN] Zenp: ");pi(st.n_zenp);ps(" PCS=");pi(st.t_f1);ps("\nZene: ");pi(st.n_zene);ps(" PCS=");pi(st.t_f2);ps("\nZenv: ");pi(st.n_zen2);ps(" PCS=");pi(st.t_f3);ps("\nZenc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zen_state(void){ps("[ZEN] Zenp=");pi(st.n_zenp);ps(" Zene=");pi(st.n_zene);ps(" Zenv=");pi(st.n_zen2);ps(" Zenc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zenobia Admin Demo ===\n\n");zen_init();
ps("Zenobia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zen_planning(t,c,966+(i*17),955+(i*14),935+(i*10),917+(i*6),2020+(i%5));}
ps("\nZenobia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zen_execution(t,c,955+(i*15),944+(i*12),926+(i*8),913+(i*5),2021+(i%4));}
ps("\nZenobia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zen_evaluation(t,c,947+(i*13),936+(i*10),920+(i*7),909+(i*4),2022+(i%3));}
ps("\nZenobia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zen_accessory(t,c,939+(i*11),930+(i*9),916+(i*6),906+(i*3),2023+(i%2));}
ps("\nZenobia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zen_market(t,c,933+(i*9),924+(i*7),911+(i*5),903+(i*3),2024);}
ps("\n");zen_report();zen_state();ps("\n=== Demo Complete ===\n");return 0;}
