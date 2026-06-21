/* haemanthus_admin: Haemanthus management technology administration (v1.0)
 * Haemanthus planning, haemanthus execution, haemanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hae_t;
typedef struct{int n_hap,n_hae,n_hav,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hae_state_t;
static hae_t haps[N],haes[N-2],havs[N-4],acs[N-6],mks[N-6]; static hae_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hae_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hae_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HAE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hae_init(void){if(init)return -1;st.n_hap=0;st.n_hae=0;st.n_hav=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)haps[i].active=0;for(int i=0;i<N-2;i++)haes[i].active=0;for(int i=0;i<N-4;i++)havs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HAE] Haemanthus initialized\n");return 0;}
int hae_planning(int t,int c,int a,int b,int d,int e,int y){return add(haps,&st.n_hap,&st.t_f1,N,t,c,a,b,d,e,y);}
int hae_execution(int t,int c,int a,int b,int d,int e,int y){return add(haes,&st.n_hae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hae_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(havs,&st.n_hav,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hae_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hae_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hae_report(void){ps("[HAE] Hap: ");pi(st.n_hap);ps(" PCS=");pi(st.t_f1);ps("\nHae: ");pi(st.n_hae);ps(" PCS=");pi(st.t_f2);ps("\nHav: ");pi(st.n_hav);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hae_state(void){ps("[HAE] Hap=");pi(st.n_hap);ps(" Hae=");pi(st.n_hae);ps(" Hav=");pi(st.n_hav);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Haemanthus Admin Demo ===\n\n");hae_init();
ps("Haemanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hae_planning(t,c,627+(i*17),616+(i*14),596+(i*10),578+(i*6),2020+(i%5));}
ps("\nHaemanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hae_execution(t,c,616+(i*15),605+(i*12),587+(i*8),574+(i*5),2021+(i%4));}
ps("\nHaemanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hae_evaluation(t,c,608+(i*13),597+(i*10),581+(i*7),570+(i*4),2022+(i%3));}
ps("\nHaemanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hae_accessory(t,c,600+(i*11),591+(i*9),577+(i*6),567+(i*3),2023+(i%2));}
ps("\nHaemanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hae_market(t,c,594+(i*9),585+(i*7),572+(i*5),564+(i*3),2024);}
ps("\n");hae_report();hae_state();ps("\n=== Demo Complete ===\n");return 0;}
