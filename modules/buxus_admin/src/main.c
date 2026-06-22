/* buxus_admin: Buxus management technology administration (v1.0)
 * Buxus planning, buxus execution, buxus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bux_t;
typedef struct{int n_buxp,n_buxe,n_bux2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bux_state_t;
static bux_t buxps[N],buxss[N-2],buxvss[N-4],buxas[N-6],buxmks[N-6]; static bux_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bux_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bux_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BUX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bux_init(void){if(init)return -1;st.n_buxp=0;st.n_buxe=0;st.n_bux2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)buxps[i].active=0;for(int i=0;i<N-2;i++)buxss[i].active=0;for(int i=0;i<N-4;i++)buxvss[i].active=0;for(int i=0;i<N-6;i++)buxas[i].active=0;for(int i=0;i<N-6;i++)buxmks[i].active=0;init=1;ps("[BUX] Buxus initialized\n");return 0;}
int bux_planning(int t,int c,int a,int b,int d,int e,int y){return add(buxps,&st.n_buxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bux_execution(int t,int c,int a,int b,int d,int e,int y){return add(buxss,&st.n_buxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bux_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(buxvss,&st.n_bux2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bux_accessory(int t,int c,int a,int b,int d,int e,int y){return add(buxas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bux_market(int t,int c,int a,int b,int d,int e,int y){return add(buxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bux_report(void){ps("[BUX] Buxpp: ");pi(st.n_buxp);ps(" PCS=");pi(st.t_f1);ps("\nBuxe: ");pi(st.n_buxe);ps(" PCS=");pi(st.t_f2);ps("\nBuxv: ");pi(st.n_bux2);ps(" PCS=");pi(st.t_f3);ps("\nBuxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bux_state(void){ps("[BUX] Buxpp=");pi(st.n_buxp);ps(" Buxe=");pi(st.n_buxe);ps(" Buxv=");pi(st.n_bux2);ps(" Buxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Buxus Admin Demo ===\n\n");bux_init();
ps("Buxus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bux_planning(t,c,1125+(i*17),1114+(i*14),1094+(i*10),1076+(i*6),2020+(i%5));}
ps("\nBuxus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bux_execution(t,c,1114+(i*15),1103+(i*12),1085+(i*8),1072+(i*5),2021+(i%4));}
ps("\nBuxus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bux_evaluation(t,c,1106+(i*13),1095+(i*10),1079+(i*7),1068+(i*4),2022+(i%3));}
ps("\nBuxus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bux_accessory(t,c,1098+(i*11),1089+(i*9),1075+(i*6),1065+(i*3),2023+(i%2));}
ps("\nBuxus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bux_market(t,c,1092+(i*9),1083+(i*7),1070+(i*5),1062+(i*3),2024);}
ps("\n");bux_report();bux_state();ps("\n=== Demo Complete ===\n");return 0;}
