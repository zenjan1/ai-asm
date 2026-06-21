/* ixora_admin: Ixora management technology administration (v1.0)
 * Ixora planning, ixora execution, ixora evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ixo_t;
typedef struct{int n_ixp,n_ixe,n_ixv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ixo_state_t;
static ixo_t ixps[N],ixes[N-2],ixvs[N-4],iacs[N-6],imks[N-6]; static ixo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ixo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ixo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IXO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ixo_init(void){if(init)return -1;st.n_ixp=0;st.n_ixe=0;st.n_ixv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ixps[i].active=0;for(int i=0;i<N-2;i++)ixes[i].active=0;for(int i=0;i<N-4;i++)ixvs[i].active=0;for(int i=0;i<N-6;i++)iacs[i].active=0;for(int i=0;i<N-6;i++)imks[i].active=0;init=1;ps("[IXO] Ixora initialized\n");return 0;}
int ixo_planning(int t,int c,int a,int b,int d,int e,int y){return add(ixps,&st.n_ixp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ixo_execution(int t,int c,int a,int b,int d,int e,int y){return add(ixes,&st.n_ixe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ixo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ixvs,&st.n_ixv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ixo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(iacs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ixo_market(int t,int c,int a,int b,int d,int e,int y){return add(imks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ixo_report(void){ps("[IXO] Ixp: ");pi(st.n_ixp);ps(" PCS=");pi(st.t_f1);ps("\nIxe: ");pi(st.n_ixe);ps(" PCS=");pi(st.t_f2);ps("\nIxv: ");pi(st.n_ixv);ps(" PCS=");pi(st.t_f3);ps("\nIc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ixo_state(void){ps("[IXO] Ixp=");pi(st.n_ixp);ps(" Ixe=");pi(st.n_ixe);ps(" Ixv=");pi(st.n_ixv);ps(" Ic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ixora Admin Demo ===\n\n");ixo_init();
ps("Ixora planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ixo_planning(t,c,634+(i*17),623+(i*14),603+(i*10),585+(i*6),2020+(i%5));}
ps("\nIxora execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ixo_execution(t,c,623+(i*15),612+(i*12),594+(i*8),581+(i*5),2021+(i%4));}
ps("\nIxora evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ixo_evaluation(t,c,615+(i*13),604+(i*10),588+(i*7),577+(i*4),2022+(i%3));}
ps("\nIxora accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ixo_accessory(t,c,607+(i*11),598+(i*9),584+(i*6),574+(i*3),2023+(i%2));}
ps("\nIxora marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ixo_market(t,c,601+(i*9),592+(i*7),579+(i*5),571+(i*3),2024);}
ps("\n");ixo_report();ixo_state();ps("\n=== Demo Complete ===\n");return 0;}
