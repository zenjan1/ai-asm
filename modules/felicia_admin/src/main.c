/* felicia_admin: Felicia management technology administration (v1.0)
 * Felicia planning, felicia execution, felicia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fx_t;
typedef struct{int n_fxp,n_fxe,n_fxv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fx_state_t;
static fx_t fxps[N],fxss[N-2],fxvss[N-4],fxas[N-6],fxmks[N-6]; static fx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FXI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fxi_init(void){if(init)return -1;st.n_fxp=0;st.n_fxe=0;st.n_fxv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fxps[i].active=0;for(int i=0;i<N-2;i++)fxss[i].active=0;for(int i=0;i<N-4;i++)fxvss[i].active=0;for(int i=0;i<N-6;i++)fxas[i].active=0;for(int i=0;i<N-6;i++)fxmks[i].active=0;init=1;ps("[FXI] Felicia initialized\n");return 0;}
int fxi_planning(int t,int c,int a,int b,int d,int e,int y){return add(fxps,&st.n_fxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fxi_execution(int t,int c,int a,int b,int d,int e,int y){return add(fxss,&st.n_fxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fxi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fxvss,&st.n_fxv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fxi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fxas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fxi_market(int t,int c,int a,int b,int d,int e,int y){return add(fxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fxi_report(void){ps("[FXI] Fxp: ");pi(st.n_fxp);ps(" PCS=");pi(st.t_f1);ps("\nFxe: ");pi(st.n_fxe);ps(" PCS=");pi(st.t_f2);ps("\nFxv: ");pi(st.n_fxv);ps(" PCS=");pi(st.t_f3);ps("\nFxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fxi_state(void){ps("[FXI] Fxp=");pi(st.n_fxp);ps(" Fxe=");pi(st.n_fxe);ps(" Fxv=");pi(st.n_fxv);ps(" Fxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Felicia Admin Demo ===\n\n");fxi_init();
ps("Felicia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fxi_planning(t,c,773+(i*17),762+(i*14),742+(i*10),724+(i*6),2020+(i%5));}
ps("\nFelicia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fxi_execution(t,c,762+(i*15),751+(i*12),733+(i*8),720+(i*5),2021+(i%4));}
ps("\nFelicia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fxi_evaluation(t,c,754+(i*13),743+(i*10),729+(i*7),718+(i*4),2022+(i%3));}
ps("\nFelicia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fxi_accessory(t,c,746+(i*11),737+(i*9),723+(i*6),713+(i*3),2023+(i%2));}
ps("\nFelicia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fxi_market(t,c,740+(i*9),731+(i*7),718+(i*5),710+(i*3),2024);}
ps("\n");fxi_report();fxi_state();ps("\n=== Demo Complete ===\n");return 0;}
