/* xylosma_admin: Xylosma management technology administration (v1.0)
 * Xylosma planning, xylosma execution, xylosma evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} xyl_t;
typedef struct{int n_xylp,n_xyle,n_xyl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} xyl_state_t;
static xyl_t xylps[N],xylss[N-2],xylvss[N-4],xylas[N-6],xylmks[N-6]; static xyl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(xyl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;xyl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[XYL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int xyl_init(void){if(init)return -1;st.n_xylp=0;st.n_xyle=0;st.n_xyl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)xylps[i].active=0;for(int i=0;i<N-2;i++)xylss[i].active=0;for(int i=0;i<N-4;i++)xylvss[i].active=0;for(int i=0;i<N-6;i++)xylas[i].active=0;for(int i=0;i<N-6;i++)xylmks[i].active=0;init=1;ps("[XYL] Xylosma initialized\n");return 0;}
int xyl_planning(int t,int c,int a,int b,int d,int e,int y){return add(xylps,&st.n_xylp,&st.t_f1,N,t,c,a,b,d,e,y);}
int xyl_execution(int t,int c,int a,int b,int d,int e,int y){return add(xylss,&st.n_xyle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int xyl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(xylvss,&st.n_xyl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int xyl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(xylas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int xyl_market(int t,int c,int a,int b,int d,int e,int y){return add(xylmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void xyl_report(void){ps("[XYL] Xylp: ");pi(st.n_xylp);ps(" PCS=");pi(st.t_f1);ps("\nXyle: ");pi(st.n_xyle);ps(" PCS=");pi(st.t_f2);ps("\nXylv: ");pi(st.n_xyl2);ps(" PCS=");pi(st.t_f3);ps("\nXylc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void xyl_state(void){ps("[XYL] Xylp=");pi(st.n_xylp);ps(" Xyle=");pi(st.n_xyle);ps(" Xylv=");pi(st.n_xyl2);ps(" Xylc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Xylosma Admin Demo ===\n\n");xyl_init();
ps("Xylosma planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;xyl_planning(t,c,958+(i*17),947+(i*14),927+(i*10),909+(i*6),2020+(i%5));}
ps("\nXylosma execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;xyl_execution(t,c,947+(i*15),936+(i*12),918+(i*8),905+(i*5),2021+(i%4));}
ps("\nXylosma evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;xyl_evaluation(t,c,939+(i*13),928+(i*10),912+(i*7),901+(i*4),2022+(i%3));}
ps("\nXylosma accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xyl_accessory(t,c,931+(i*11),922+(i*9),908+(i*6),898+(i*3),2023+(i%2));}
ps("\nXylosma marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;xyl_market(t,c,925+(i*9),916+(i*7),903+(i*5),895+(i*3),2024);}
ps("\n");xyl_report();xyl_state();ps("\n=== Demo Complete ===\n");return 0;}
