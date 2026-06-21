/* dracaena_admin: Dracaena management technology administration (v1.0)
 * Dracaena planning, dracaena execution, dracaena evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dr_t;
typedef struct{int n_drp,n_dre,n_drv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dr_state_t;
static dr_t drps[N],drss[N-2],drvs[N-4],dras[N-6],drmks[N-6]; static dr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dr_init(void){if(init)return -1;st.n_drp=0;st.n_dre=0;st.n_drv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)drps[i].active=0;for(int i=0;i<N-2;i++)drss[i].active=0;for(int i=0;i<N-4;i++)drvs[i].active=0;for(int i=0;i<N-6;i++)dras[i].active=0;for(int i=0;i<N-6;i++)drmks[i].active=0;init=1;ps("[DRC] Dracaena initialized\n");return 0;}
int dr_planning(int t,int c,int a,int b,int d,int e,int y){return add(drps,&st.n_drp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dr_execution(int t,int c,int a,int b,int d,int e,int y){return add(drss,&st.n_dre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(drvs,&st.n_drv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dr_market(int t,int c,int a,int b,int d,int e,int y){return add(drmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dr_report(void){ps("[DRC] Drp: ");pi(st.n_drp);ps(" PCS=");pi(st.t_f1);ps("\nDre: ");pi(st.n_dre);ps(" PCS=");pi(st.t_f2);ps("\nDrv: ");pi(st.n_drv);ps(" PCS=");pi(st.t_f3);ps("\nDrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dr_state(void){ps("[DRC] Drp=");pi(st.n_drp);ps(" Dre=");pi(st.n_dre);ps(" Drv=");pi(st.n_drv);ps(" Drc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dracaena Admin Demo ===\n\n");dr_init();
ps("Dracaena planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dr_planning(t,c,642+(i*17),631+(i*14),611+(i*10),593+(i*6),2020+(i%5));}
ps("\nDracaena execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dr_execution(t,c,631+(i*15),620+(i*12),602+(i*8),589+(i*5),2021+(i%4));}
ps("\nDracaena evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dr_evaluation(t,c,623+(i*13),612+(i*10),596+(i*7),585+(i*4),2022+(i%3));}
ps("\nDracaena accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dr_accessory(t,c,615+(i*11),606+(i*9),592+(i*6),582+(i*3),2023+(i%2));}
ps("\nDracaena marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dr_market(t,c,609+(i*9),600+(i*7),587+(i*5),579+(i*3),2024);}
ps("\n");dr_report();dr_state();ps("\n=== Demo Complete ===\n");return 0;}
