/* linum_admin: Linum management technology administration (v1.0)
 * Linum planning, linum execution, linum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lin_t;
typedef struct{int n_linp,n_line,n_lin2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lin_state_t;
static lin_t linps[N],linss[N-2],linvss[N-4],linas[N-6],linmks[N-6]; static lin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lin_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lin_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LIN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lin_init(void){if(init)return -1;st.n_linp=0;st.n_line=0;st.n_lin2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)linps[i].active=0;for(int i=0;i<N-2;i++)linss[i].active=0;for(int i=0;i<N-4;i++)linvss[i].active=0;for(int i=0;i<N-6;i++)linas[i].active=0;for(int i=0;i<N-6;i++)linmks[i].active=0;init=1;ps("[LIN] Linum initialized\n");return 0;}
int lin_planning(int t,int c,int a,int b,int d,int e,int y){return add(linps,&st.n_linp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lin_execution(int t,int c,int a,int b,int d,int e,int y){return add(linss,&st.n_line,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lin_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(linvss,&st.n_lin2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lin_accessory(int t,int c,int a,int b,int d,int e,int y){return add(linas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lin_market(int t,int c,int a,int b,int d,int e,int y){return add(linmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lin_report(void){ps("[LIN] Linpp: ");pi(st.n_linp);ps(" PCS=");pi(st.t_f1);ps("\nLine: ");pi(st.n_line);ps(" PCS=");pi(st.t_f2);ps("\nLinv: ");pi(st.n_lin2);ps(" PCS=");pi(st.t_f3);ps("\nLinc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lin_state(void){ps("[LIN] Linpp=");pi(st.n_linp);ps(" Line=");pi(st.n_line);ps(" Linv=");pi(st.n_lin2);ps(" Linc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Linum Admin Demo ===\n\n");lin_init();
ps("Linum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lin_planning(t,c,1155+(i*17),1144+(i*14),1124+(i*10),1106+(i*6),2020+(i%5));}
ps("\nLinum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lin_execution(t,c,1144+(i*15),1133+(i*12),1115+(i*8),1102+(i*5),2021+(i%4));}
ps("\nLinum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lin_evaluation(t,c,1136+(i*13),1125+(i*10),1109+(i*7),1098+(i*4),2022+(i%3));}
ps("\nLinum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lin_accessory(t,c,1128+(i*11),1119+(i*9),1105+(i*6),1095+(i*3),2023+(i%2));}
ps("\nLinum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lin_market(t,c,1122+(i*9),1113+(i*7),1100+(i*5),1092+(i*3),2024);}
ps("\n");lin_report();lin_state();ps("\n=== Demo Complete ===\n");return 0;}
