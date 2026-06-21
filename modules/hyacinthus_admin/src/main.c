/* hyacinthus_admin: Hyacinthus management technology administration (v1.0)
 * Hyacinthus planning, hyacinthus execution, hyacinthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hya_t;
typedef struct{int n_hyap,n_hyae,n_hya2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hya_state_t;
static hya_t hyaps[N],hyass[N-2],hyavss[N-4],hyaas[N-6],hyamks[N-6]; static hya_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hya_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hya_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hya_init(void){if(init)return -1;st.n_hyap=0;st.n_hyae=0;st.n_hya2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hyaps[i].active=0;for(int i=0;i<N-2;i++)hyass[i].active=0;for(int i=0;i<N-4;i++)hyavss[i].active=0;for(int i=0;i<N-6;i++)hyaas[i].active=0;for(int i=0;i<N-6;i++)hyamks[i].active=0;init=1;ps("[HYA] Hyacinthus initialized\n");return 0;}
int hya_planning(int t,int c,int a,int b,int d,int e,int y){return add(hyaps,&st.n_hyap,&st.t_f1,N,t,c,a,b,d,e,y);}
int hya_execution(int t,int c,int a,int b,int d,int e,int y){return add(hyass,&st.n_hyae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hya_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hyavss,&st.n_hya2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hya_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hyaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hya_market(int t,int c,int a,int b,int d,int e,int y){return add(hyamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hya_report(void){ps("[HYA] Hyap: ");pi(st.n_hyap);ps(" PCS=");pi(st.t_f1);ps("\nHyae: ");pi(st.n_hyae);ps(" PCS=");pi(st.t_f2);ps("\nHyav: ");pi(st.n_hya2);ps(" PCS=");pi(st.t_f3);ps("\nHyac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hya_state(void){ps("[HYA] Hyap=");pi(st.n_hyap);ps(" Hyae=");pi(st.n_hyae);ps(" Hyav=");pi(st.n_hya2);ps(" Hyac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hyacinthus Admin Demo ===\n\n");hya_init();
ps("Hyacinthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hya_planning(t,c,1021+(i*17),1010+(i*14),990+(i*10),972+(i*6),2020+(i%5));}
ps("\nHyacinthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hya_execution(t,c,1010+(i*15),999+(i*12),981+(i*8),968+(i*5),2021+(i%4));}
ps("\nHyacinthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hya_evaluation(t,c,1002+(i*13),991+(i*10),975+(i*7),964+(i*4),2022+(i%3));}
ps("\nHyacinthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hya_accessory(t,c,994+(i*11),985+(i*9),971+(i*6),961+(i*3),2023+(i%2));}
ps("\nHyacinthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hya_market(t,c,988+(i*9),979+(i*7),966+(i*5),958+(i*3),2024);}
ps("\n");hya_report();hya_state();ps("\n=== Demo Complete ===\n");return 0;}
