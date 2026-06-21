/* stephanotis_admin: Stephanotis management technology administration (v1.0)
 * Stephanotis planning, stephanotis execution, stephanotis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ste_t;
typedef struct{int n_stp,n_ste,n_stv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ste_state_t;
static ste_t stps[N],stes[N-2],stvs[N-4],scs[N-6],smks[N-6]; static ste_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ste_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ste_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[STE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ste_init(void){if(init)return -1;st.n_stp=0;st.n_ste=0;st.n_stv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)stps[i].active=0;for(int i=0;i<N-2;i++)stes[i].active=0;for(int i=0;i<N-4;i++)stvs[i].active=0;for(int i=0;i<N-6;i++)scs[i].active=0;for(int i=0;i<N-6;i++)smks[i].active=0;init=1;ps("[STE] Stephanotis initialized\n");return 0;}
int ste_planning(int t,int c,int a,int b,int d,int e,int y){return add(stps,&st.n_stp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ste_execution(int t,int c,int a,int b,int d,int e,int y){return add(stes,&st.n_ste,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ste_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(stvs,&st.n_stv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ste_accessory(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ste_market(int t,int c,int a,int b,int d,int e,int y){return add(smks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ste_report(void){ps("[STE] Stp: ");pi(st.n_stp);ps(" PCS=");pi(st.t_f1);ps("\nSte: ");pi(st.n_ste);ps(" PCS=");pi(st.t_f2);ps("\nStv: ");pi(st.n_stv);ps(" PCS=");pi(st.t_f3);ps("\nSc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ste_state(void){ps("[STE] Stp=");pi(st.n_stp);ps(" Ste=");pi(st.n_ste);ps(" Stv=");pi(st.n_stv);ps(" Sc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Stephanotis Admin Demo ===\n\n");ste_init();
ps("Stephanotis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ste_planning(t,c,633+(i*17),622+(i*14),602+(i*10),584+(i*6),2020+(i%5));}
ps("\nStephanotis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ste_execution(t,c,622+(i*15),611+(i*12),593+(i*8),580+(i*5),2021+(i%4));}
ps("\nStephanotis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ste_evaluation(t,c,614+(i*13),603+(i*10),587+(i*7),576+(i*4),2022+(i%3));}
ps("\nStephanotis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ste_accessory(t,c,606+(i*11),597+(i*9),583+(i*6),573+(i*3),2023+(i%2));}
ps("\nStephanotis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ste_market(t,c,600+(i*9),591+(i*7),578+(i*5),570+(i*3),2024);}
ps("\n");ste_report();ste_state();ps("\n=== Demo Complete ===\n");return 0;}
