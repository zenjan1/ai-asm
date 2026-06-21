/* ismene_admin: Ismene management technology administration (v1.0)
 * Ismene planning, ismene execution, ismene evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ism_t;
typedef struct{int n_isp,n_ise,n_isv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ism_state_t;
static ism_t isps[N],ises[N-2],isvs[N-4],acs[N-6],mks[N-6]; static ism_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ism_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ism_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ISM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ism_init(void){if(init)return -1;st.n_isp=0;st.n_ise=0;st.n_isv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)isps[i].active=0;for(int i=0;i<N-2;i++)ises[i].active=0;for(int i=0;i<N-4;i++)isvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ISM] Ismene initialized\n");return 0;}
int ism_planning(int t,int c,int a,int b,int d,int e,int y){return add(isps,&st.n_isp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ism_execution(int t,int c,int a,int b,int d,int e,int y){return add(ises,&st.n_ise,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ism_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(isvs,&st.n_isv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ism_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ism_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ism_report(void){ps("[ISM] Isp: ");pi(st.n_isp);ps(" PCS=");pi(st.t_f1);ps("\nIse: ");pi(st.n_ise);ps(" PCS=");pi(st.t_f2);ps("\nIsv: ");pi(st.n_isv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ism_state(void){ps("[ISM] Isp=");pi(st.n_isp);ps(" Ise=");pi(st.n_ise);ps(" Isv=");pi(st.n_isv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ismene Admin Demo ===\n\n");ism_init();
ps("Ismene planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ism_planning(t,c,624+(i*17),613+(i*14),593+(i*10),575+(i*6),2020+(i%5));}
ps("\nIsmene execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ism_execution(t,c,613+(i*15),602+(i*12),584+(i*8),571+(i*5),2021+(i%4));}
ps("\nIsmene evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ism_evaluation(t,c,605+(i*13),594+(i*10),578+(i*7),567+(i*4),2022+(i%3));}
ps("\nIsmene accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ism_accessory(t,c,597+(i*11),588+(i*9),574+(i*6),564+(i*3),2023+(i%2));}
ps("\nIsmene marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ism_market(t,c,591+(i*9),582+(i*7),569+(i*5),561+(i*3),2024);}
ps("\n");ism_report();ism_state();ps("\n=== Demo Complete ===\n");return 0;}
