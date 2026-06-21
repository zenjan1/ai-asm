/* feverfew_admin: Feverfew management technology administration (v1.0)
 * Feverfew planning, feverfew execution, feverfew evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fv_t;
typedef struct{int n_frp,n_fre,n_frv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fv_state_t;
static fv_t frps[N],frss[N-2],frvss[N-4],fras[N-6],frmks[N-6]; static fv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FVR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fr_init(void){if(init)return -1;st.n_frp=0;st.n_fre=0;st.n_frv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)frps[i].active=0;for(int i=0;i<N-2;i++)frss[i].active=0;for(int i=0;i<N-4;i++)frvss[i].active=0;for(int i=0;i<N-6;i++)fras[i].active=0;for(int i=0;i<N-6;i++)frmks[i].active=0;init=1;ps("[FVR] Feverfew initialized\n");return 0;}
int fr_planning(int t,int c,int a,int b,int d,int e,int y){return add(frps,&st.n_frp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fr_execution(int t,int c,int a,int b,int d,int e,int y){return add(frss,&st.n_fre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(frvss,&st.n_frv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fr_market(int t,int c,int a,int b,int d,int e,int y){return add(frmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fr_report(void){ps("[FVR] Frp: ");pi(st.n_frp);ps(" PCS=");pi(st.t_f1);ps("\nFre: ");pi(st.n_fre);ps(" PCS=");pi(st.t_f2);ps("\nFrv: ");pi(st.n_frv);ps(" PCS=");pi(st.t_f3);ps("\nFrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fr_state(void){ps("[FVR] Frp=");pi(st.n_frp);ps(" Fre=");pi(st.n_fre);ps(" Frv=");pi(st.n_frv);ps(" Frc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Feverfew Admin Demo ===\n\n");fr_init();
ps("Feverfew planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fr_planning(t,c,709+(i*17),698+(i*14),678+(i*10),660+(i*6),2020+(i%5));}
ps("\nFeverfew execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fr_execution(t,c,698+(i*15),687+(i*12),669+(i*8),656+(i*5),2021+(i%4));}
ps("\nFeverfew evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fr_evaluation(t,c,690+(i*13),679+(i*10),663+(i*7),652+(i*4),2022+(i%3));}
ps("\nFeverfew accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fr_accessory(t,c,682+(i*11),673+(i*9),659+(i*6),649+(i*3),2023+(i%2));}
ps("\nFeverfew marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fr_market(t,c,676+(i*9),667+(i*7),654+(i*5),646+(i*3),2024);}
ps("\n");fr_report();fr_state();ps("\n=== Demo Complete ===\n");return 0;}
