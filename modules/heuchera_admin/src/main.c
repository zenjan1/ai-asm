/* heuchera_admin: Heuchera management technology administration (v1.0)
 * Heuchera planning, heuchera execution, heuchera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hu_t;
typedef struct{int n_hcp,n_hce,n_hcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hu_state_t;
static hu_t hcps[N],hcss[N-2],hcvss[N-4],hcas[N-6],hcmks[N-6]; static hu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HCR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hcr_init(void){if(init)return -1;st.n_hcp=0;st.n_hce=0;st.n_hcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hcps[i].active=0;for(int i=0;i<N-2;i++)hcss[i].active=0;for(int i=0;i<N-4;i++)hcvss[i].active=0;for(int i=0;i<N-6;i++)hcas[i].active=0;for(int i=0;i<N-6;i++)hcmks[i].active=0;init=1;ps("[HCR] Heuchera initialized\n");return 0;}
int hcr_planning(int t,int c,int a,int b,int d,int e,int y){return add(hcps,&st.n_hcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hcr_execution(int t,int c,int a,int b,int d,int e,int y){return add(hcss,&st.n_hce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hcr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hcvss,&st.n_hcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hcr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hcr_market(int t,int c,int a,int b,int d,int e,int y){return add(hcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hcr_report(void){ps("[HCR] Hcp: ");pi(st.n_hcp);ps(" PCS=");pi(st.t_f1);ps("\nHce: ");pi(st.n_hce);ps(" PCS=");pi(st.t_f2);ps("\nHcv: ");pi(st.n_hcv);ps(" PCS=");pi(st.t_f3);ps("\nHcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hcr_state(void){ps("[HCR] Hcp=");pi(st.n_hcp);ps(" Hce=");pi(st.n_hce);ps(" Hcv=");pi(st.n_hcv);ps(" Hcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heuchera Admin Demo ===\n\n");hcr_init();
ps("Heuchera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hcr_planning(t,c,729+(i*17),718+(i*14),698+(i*10),680+(i*6),2020+(i%5));}
ps("\nHeuchera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hcr_execution(t,c,718+(i*15),707+(i*12),689+(i*8),676+(i*5),2021+(i%4));}
ps("\nHeuchera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hcr_evaluation(t,c,710+(i*13),699+(i*10),683+(i*7),672+(i*4),2022+(i%3));}
ps("\nHeuchera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hcr_accessory(t,c,702+(i*11),693+(i*9),679+(i*6),669+(i*3),2023+(i%2));}
ps("\nHeuchera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hcr_market(t,c,696+(i*9),687+(i*7),674+(i*5),666+(i*3),2024);}
ps("\n");hcr_report();hcr_state();ps("\n=== Demo Complete ===\n");return 0;}
