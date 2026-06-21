/* hosta_admin: Hosta management technology administration (v1.0)
 * Hosta planning, hosta execution, hosta evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ho_t;
typedef struct{int n_hsp,n_hse,n_hsv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ho_state_t;
static ho_t hsps[N],hsss[N-2],hsvss[N-4],hsas[N-6],hsmks[N-6]; static ho_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ho_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ho_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hst_init(void){if(init)return -1;st.n_hsp=0;st.n_hse=0;st.n_hsv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hsps[i].active=0;for(int i=0;i<N-2;i++)hsss[i].active=0;for(int i=0;i<N-4;i++)hsvss[i].active=0;for(int i=0;i<N-6;i++)hsas[i].active=0;for(int i=0;i<N-6;i++)hsmks[i].active=0;init=1;ps("[HST] Hosta initialized\n");return 0;}
int hst_planning(int t,int c,int a,int b,int d,int e,int y){return add(hsps,&st.n_hsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hst_execution(int t,int c,int a,int b,int d,int e,int y){return add(hsss,&st.n_hse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hst_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hsvss,&st.n_hsv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hst_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hsas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hst_market(int t,int c,int a,int b,int d,int e,int y){return add(hsmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hst_report(void){ps("[HST] Hsp: ");pi(st.n_hsp);ps(" PCS=");pi(st.t_f1);ps("\nHse: ");pi(st.n_hse);ps(" PCS=");pi(st.t_f2);ps("\nHsv: ");pi(st.n_hsv);ps(" PCS=");pi(st.t_f3);ps("\nHsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hst_state(void){ps("[HST] Hsp=");pi(st.n_hsp);ps(" Hse=");pi(st.n_hse);ps(" Hsv=");pi(st.n_hsv);ps(" Hsc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hosta Admin Demo ===\n\n");hst_init();
ps("Hosta planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hst_planning(t,c,786+(i*17),775+(i*14),755+(i*10),737+(i*6),2020+(i%5));}
ps("\nHosta execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hst_execution(t,c,775+(i*15),764+(i*12),746+(i*8),733+(i*5),2021+(i%4));}
ps("\nHosta evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hst_evaluation(t,c,767+(i*13),756+(i*10),742+(i*7),731+(i*4),2022+(i%3));}
ps("\nHosta accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hst_accessory(t,c,759+(i*11),750+(i*9),736+(i*6),726+(i*3),2023+(i%2));}
ps("\nHosta marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hst_market(t,c,753+(i*9),744+(i*7),731+(i*5),723+(i*3),2024);}
ps("\n");hst_report();hst_state();ps("\n=== Demo Complete ===\n");return 0;}
