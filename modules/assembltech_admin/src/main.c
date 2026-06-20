/* assembltech_admin: Assembly technology administration (v1.0)
 * Mechanical assembly, electrical assembly, hydraulic assembly, automation assembly, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} asm_t;
typedef struct{int n_mc,n_el,n_hy,n_au,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} asm_state_t;
static asm_t mcs[N],els[N-2],hys[N-4],aus[N-6],mks[N-6]; static asm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;asm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ASM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int asm_init(void){if(init)return -1;st.n_mc=0;st.n_el=0;st.n_hy=0;st.n_au=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)els[i].active=0;for(int i=0;i<N-4;i++)hys[i].active=0;for(int i=0;i<N-6;i++)aus[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ASM] Assembltech initialized\n");return 0;}
int asm_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int asm_electrical(int t,int c,int a,int b,int d,int e,int y){return add(els,&st.n_el,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int asm_hydraulic(int t,int c,int a,int b,int d,int e,int y){return add(hys,&st.n_hy,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int asm_auto(int t,int c,int a,int b,int d,int e,int y){return add(aus,&st.n_au,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int asm_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void asm_report(void){ps("[ASM] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nEl: ");pi(st.n_el);ps(" PCS=");pi(st.t_f2);ps("\nHy: ");pi(st.n_hy);ps(" PCS=");pi(st.t_f3);ps("\nAu: ");pi(st.n_au);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void asm_state(void){ps("[ASM] Mc=");pi(st.n_mc);ps(" El=");pi(st.n_el);ps(" Hy=");pi(st.n_hy);ps(" Au=");pi(st.n_au);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Assembly Tech Admin Demo ===\n\n");asm_init();
ps("Mechanical assembly...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;asm_mechanical(t,c,249+(i*17),234+(i*14),214+(i*10),196+(i*6),2020+(i%5));}
ps("\nElectrical assembly...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;asm_electrical(t,c,238+(i*15),224+(i*12),206+(i*8),193+(i*5),2021+(i%4));}
ps("\nHydraulic assembly...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;asm_hydraulic(t,c,230+(i*13),216+(i*10),200+(i*7),189+(i*4),2022+(i%3));}
ps("\nAutomation assembly...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asm_auto(t,c,222+(i*11),210+(i*9),196+(i*6),186+(i*3),2023+(i%2));}
ps("\nAssembly marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asm_market(t,c,216+(i*9),205+(i*7),192+(i*5),184+(i*3),2024);}
ps("\n");asm_report();asm_state();ps("\n=== Demo Complete ===\n");return 0;}
