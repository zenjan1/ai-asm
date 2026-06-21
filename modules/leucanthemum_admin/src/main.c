/* leucanthemum_admin: Leucanthemum management technology administration (v1.0)
 * Leucanthemum planning, leucanthemum execution, leucanthemum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lcx_t;
typedef struct{int n_lxp,n_lxe,n_lxv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lcx_state_t;
static lcx_t lcxps[N],lcxss[N-2],lcxvss[N-4],lcxas[N-6],lcxmks[N-6]; static lcx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lcx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lcx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LCX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lcx_init(void){if(init)return -1;st.n_lxp=0;st.n_lxe=0;st.n_lxv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lcxps[i].active=0;for(int i=0;i<N-2;i++)lcxss[i].active=0;for(int i=0;i<N-4;i++)lcxvss[i].active=0;for(int i=0;i<N-6;i++)lcxas[i].active=0;for(int i=0;i<N-6;i++)lcxmks[i].active=0;init=1;ps("[LCX] Leucanthemum initialized\n");return 0;}
int lcx_planning(int t,int c,int a,int b,int d,int e,int y){return add(lcxps,&st.n_lxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lcx_execution(int t,int c,int a,int b,int d,int e,int y){return add(lcxss,&st.n_lxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lcx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lcxvss,&st.n_lxv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lcx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lcxas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lcx_market(int t,int c,int a,int b,int d,int e,int y){return add(lcxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lcx_report(void){ps("[LCX] Lxp: ");pi(st.n_lxp);ps(" PCS=");pi(st.t_f1);ps("\nLxe: ");pi(st.n_lxe);ps(" PCS=");pi(st.t_f2);ps("\nLxv: ");pi(st.n_lxv);ps(" PCS=");pi(st.t_f3);ps("\nLxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lcx_state(void){ps("[LCX] Lxp=");pi(st.n_lxp);ps(" Lxe=");pi(st.n_lxe);ps(" Lxv=");pi(st.n_lxv);ps(" Lxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Leucanthemum Admin Demo ===\n\n");lcx_init();
ps("Leucanthemum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lcx_planning(t,c,831+(i*17),820+(i*14),800+(i*10),782+(i*6),2020+(i%5));}
ps("\nLeucanthemum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lcx_execution(t,c,820+(i*15),809+(i*12),791+(i*8),778+(i*5),2021+(i%4));}
ps("\nLeucanthemum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lcx_evaluation(t,c,812+(i*13),801+(i*10),785+(i*7),774+(i*4),2022+(i%3));}
ps("\nLeucanthemum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lcx_accessory(t,c,804+(i*11),795+(i*9),781+(i*6),771+(i*3),2023+(i%2));}
ps("\nLeucanthemum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lcx_market(t,c,798+(i*9),789+(i*7),776+(i*5),768+(i*3),2024);}
ps("\n");lcx_report();lcx_state();ps("\n=== Demo Complete ===\n");return 0;}
