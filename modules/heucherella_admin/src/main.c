/* heucherella_admin: Heucherella management technology administration (v1.0)
 * Heucherella planning, heucherella execution, heucherella evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hce_t;
typedef struct{int n_hcp,n_hce,n_hcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hce_state_t;
static hce_t hceps[N],hcess[N-2],hcevss[N-4],hceas[N-6],hcemarks[N-6]; static hce_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hce_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hce_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HCE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hce_init(void){if(init)return -1;st.n_hcp=0;st.n_hce=0;st.n_hcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hceps[i].active=0;for(int i=0;i<N-2;i++)hcess[i].active=0;for(int i=0;i<N-4;i++)hcevss[i].active=0;for(int i=0;i<N-6;i++)hceas[i].active=0;for(int i=0;i<N-6;i++)hcemarks[i].active=0;init=1;ps("[HCE] Heucherella initialized\n");return 0;}
int hce_planning(int t,int c,int a,int b,int d,int e,int y){return add(hceps,&st.n_hcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hce_execution(int t,int c,int a,int b,int d,int e,int y){return add(hcess,&st.n_hce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hce_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hcevss,&st.n_hcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hce_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hceas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hce_market(int t,int c,int a,int b,int d,int e,int y){return add(hcemarks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hce_report(void){ps("[HCE] Hcp: ");pi(st.n_hcp);ps(" PCS=");pi(st.t_f1);ps("\nHce: ");pi(st.n_hce);ps(" PCS=");pi(st.t_f2);ps("\nHcv: ");pi(st.n_hcv);ps(" PCS=");pi(st.t_f3);ps("\nHcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hce_state(void){ps("[HCE] Hcp=");pi(st.n_hcp);ps(" Hce=");pi(st.n_hce);ps(" Hcv=");pi(st.n_hcv);ps(" Hcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heucherella Admin Demo ===\n\n");hce_init();
ps("Heucherella planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hce_planning(t,c,829+(i*17),818+(i*14),798+(i*10),780+(i*6),2020+(i%5));}
ps("\nHeucherella execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hce_execution(t,c,818+(i*15),807+(i*12),789+(i*8),776+(i*5),2021+(i%4));}
ps("\nHeucherella evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hce_evaluation(t,c,810+(i*13),799+(i*10),783+(i*7),772+(i*4),2022+(i%3));}
ps("\nHeucherella accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hce_accessory(t,c,802+(i*11),793+(i*9),779+(i*6),769+(i*3),2023+(i%2));}
ps("\nHeucherella marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hce_market(t,c,796+(i*9),787+(i*7),774+(i*5),766+(i*3),2024);}
ps("\n");hce_report();hce_state();ps("\n=== Demo Complete ===\n");return 0;}
