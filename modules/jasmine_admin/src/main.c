/* jasmine_admin: Jasmine management technology administration (v1.0)
 * Jasmine planning, jasmine execution, jasmine evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jas_t;
typedef struct{int n_jp,n_je,n_jv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jas_state_t;
static jas_t jps[N],jes[N-2],jvs[N-4],acs[N-6],mks[N-6]; static jas_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jas_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jas_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JAS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jas_init(void){if(init)return -1;st.n_jp=0;st.n_je=0;st.n_jv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jps[i].active=0;for(int i=0;i<N-2;i++)jes[i].active=0;for(int i=0;i<N-4;i++)jvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[JAS] Jasmine initialized\n");return 0;}
int jas_planning(int t,int c,int a,int b,int d,int e,int y){return add(jps,&st.n_jp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jas_execution(int t,int c,int a,int b,int d,int e,int y){return add(jes,&st.n_je,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jas_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jvs,&st.n_jv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jas_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jas_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jas_report(void){ps("[JAS] Jp: ");pi(st.n_jp);ps(" PCS=");pi(st.t_f1);ps("\nJe: ");pi(st.n_je);ps(" PCS=");pi(st.t_f2);ps("\nJv: ");pi(st.n_jv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jas_state(void){ps("[JAS] Jp=");pi(st.n_jp);ps(" Je=");pi(st.n_je);ps(" Jv=");pi(st.n_jv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jasmine Admin Demo ===\n\n");jas_init();
ps("Jasmine planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jas_planning(t,c,543+(i*17),532+(i*14),512+(i*10),494+(i*6),2020+(i%5));}
ps("\nJasmine execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jas_execution(t,c,532+(i*15),521+(i*12),503+(i*8),490+(i*5),2021+(i%4));}
ps("\nJasmine evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jas_evaluation(t,c,524+(i*13),513+(i*10),497+(i*7),486+(i*4),2022+(i%3));}
ps("\nJasmine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jas_accessory(t,c,516+(i*11),507+(i*9),493+(i*6),483+(i*3),2023+(i%2));}
ps("\nJasmine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jas_market(t,c,510+(i*9),501+(i*7),488+(i*5),480+(i*3),2024);}
ps("\n");jas_report();jas_state();ps("\n=== Demo Complete ===\n");return 0;}
