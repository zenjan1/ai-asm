/* nuxia_admin: Nuxia management technology administration (v1.0)
 * Nuxia planning, nuxia execution, nuxia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nux_t;
typedef struct{int n_nuxp,n_nuxe,n_nux2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nux_state_t;
static nux_t nuxps[N],nuxss[N-2],nuxvss[N-4],nuxas[N-6],nuxmks[N-6]; static nux_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nux_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nux_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NUX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nux_init(void){if(init)return -1;st.n_nuxp=0;st.n_nuxe=0;st.n_nux2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nuxps[i].active=0;for(int i=0;i<N-2;i++)nuxss[i].active=0;for(int i=0;i<N-4;i++)nuxvss[i].active=0;for(int i=0;i<N-6;i++)nuxas[i].active=0;for(int i=0;i<N-6;i++)nuxmks[i].active=0;init=1;ps("[NUX] Nuxia initialized\n");return 0;}
int nux_planning(int t,int c,int a,int b,int d,int e,int y){return add(nuxps,&st.n_nuxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nux_execution(int t,int c,int a,int b,int d,int e,int y){return add(nuxss,&st.n_nuxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nux_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nuxvss,&st.n_nux2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nux_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nuxas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nux_market(int t,int c,int a,int b,int d,int e,int y){return add(nuxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nux_report(void){ps("[NUX] Nuxp: ");pi(st.n_nuxp);ps(" PCS=");pi(st.t_f1);ps("\nNuxe: ");pi(st.n_nuxe);ps(" PCS=");pi(st.t_f2);ps("\nNuxv: ");pi(st.n_nux2);ps(" PCS=");pi(st.t_f3);ps("\nNuxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nux_state(void){ps("[NUX] Nuxp=");pi(st.n_nuxp);ps(" Nuxe=");pi(st.n_nuxe);ps(" Nuxv=");pi(st.n_nux2);ps(" Nuxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nuxia Admin Demo ===\n\n");nux_init();
ps("Nuxia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nux_planning(t,c,1068+(i*17),1057+(i*14),1037+(i*10),1019+(i*6),2020+(i%5));}
ps("\nNuxia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nux_execution(t,c,1057+(i*15),1046+(i*12),1028+(i*8),1015+(i*5),2021+(i%4));}
ps("\nNuxia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nux_evaluation(t,c,1049+(i*13),1038+(i*10),1022+(i*7),1011+(i*4),2022+(i%3));}
ps("\nNuxia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nux_accessory(t,c,1041+(i*11),1032+(i*9),1018+(i*6),1008+(i*3),2023+(i%2));}
ps("\nNuxia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nux_market(t,c,1035+(i*9),1026+(i*7),1013+(i*5),1005+(i*3),2024);}
ps("\n");nux_report();nux_state();ps("\n=== Demo Complete ===\n");return 0;}
