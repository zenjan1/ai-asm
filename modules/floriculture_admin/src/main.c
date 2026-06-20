/* floriculture_admin: Floriculture management technology administration (v1.0)
 * Floriculture planning, floriculture execution, floriculture evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} flc_t;
typedef struct{int n_fp,n_fe,n_fv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} flc_state_t;
static flc_t fps[N],fes[N-2],fvs[N-4],acs[N-6],mks[N-6]; static flc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(flc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;flc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FLC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int flc_init(void){if(init)return -1;st.n_fp=0;st.n_fe=0;st.n_fv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fps[i].active=0;for(int i=0;i<N-2;i++)fes[i].active=0;for(int i=0;i<N-4;i++)fvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FLC] Floriculture initialized\n");return 0;}
int flc_planning(int t,int c,int a,int b,int d,int e,int y){return add(fps,&st.n_fp,&st.t_f1,N,t,c,a,b,d,e,y);}
int flc_execution(int t,int c,int a,int b,int d,int e,int y){return add(fes,&st.n_fe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int flc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fvs,&st.n_fv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int flc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int flc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void flc_report(void){ps("[FLC] Fp: ");pi(st.n_fp);ps(" PCS=");pi(st.t_f1);ps("\nFe: ");pi(st.n_fe);ps(" PCS=");pi(st.t_f2);ps("\nFv: ");pi(st.n_fv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void flc_state(void){ps("[FLC] Fp=");pi(st.n_fp);ps(" Fe=");pi(st.n_fe);ps(" Fv=");pi(st.n_fv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Floriculture Admin Demo ===\n\n");flc_init();
ps("Floriculture planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;flc_planning(t,c,515+(i*17),504+(i*14),484+(i*10),466+(i*6),2020+(i%5));}
ps("\nFloriculture execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;flc_execution(t,c,504+(i*15),493+(i*12),475+(i*8),462+(i*5),2021+(i%4));}
ps("\nFloriculture evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;flc_evaluation(t,c,496+(i*13),485+(i*10),469+(i*7),458+(i*4),2022+(i%3));}
ps("\nFloriculture accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flc_accessory(t,c,488+(i*11),479+(i*9),465+(i*6),455+(i*3),2023+(i%2));}
ps("\nFloriculture marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flc_market(t,c,482+(i*9),473+(i*7),460+(i*5),452+(i*3),2024);}
ps("\n");flc_report();flc_state();ps("\n=== Demo Complete ===\n");return 0;}
