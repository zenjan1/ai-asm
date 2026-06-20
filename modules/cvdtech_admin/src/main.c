/* cvdtech_admin: CVD technology administration (v1.0)
 * CVD equipment, CVD process, CVD materials, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cvd_t;
typedef struct{int n_eq,n_pr,n_mt,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cvd_state_t;
static cvd_t eqs[N],prs[N-2],mts[N-4],acs[N-6],mks[N-6]; static cvd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cvd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cvd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CVD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cvd_init(void){if(init)return -1;st.n_eq=0;st.n_pr=0;st.n_mt=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eqs[i].active=0;for(int i=0;i<N-2;i++)prs[i].active=0;for(int i=0;i<N-4;i++)mts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CVD] Cvdtech initialized\n");return 0;}
int cvd_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f1,N,t,c,a,b,d,e,y);}
int cvd_process(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cvd_material(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cvd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cvd_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cvd_report(void){ps("[CVD] Eq: ");pi(st.n_eq);ps(" PCS=");pi(st.t_f1);ps("\nPr: ");pi(st.n_pr);ps(" PCS=");pi(st.t_f2);ps("\nMt: ");pi(st.n_mt);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cvd_state(void){ps("[CVD] Eq=");pi(st.n_eq);ps(" Pr=");pi(st.n_pr);ps(" Mt=");pi(st.n_mt);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== CVD Tech Admin Demo ===\n\n");cvd_init();
ps("CVD equipment...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cvd_equipment(t,c,319+(i*17),304+(i*14),284+(i*10),266+(i*6),2020+(i%5));}
ps("\nCVD process...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cvd_process(t,c,308+(i*15),294+(i*12),276+(i*8),263+(i*5),2021+(i%4));}
ps("\nCVD materials...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cvd_material(t,c,300+(i*13),286+(i*10),270+(i*7),259+(i*4),2022+(i%3));}
ps("\nCVD accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cvd_accessory(t,c,292+(i*11),280+(i*9),266+(i*6),256+(i*3),2023+(i%2));}
ps("\nCVD marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cvd_market(t,c,286+(i*9),275+(i*7),262+(i*5),254+(i*3),2024);}
ps("\n");cvd_report();cvd_state();ps("\n=== Demo Complete ===\n");return 0;}
