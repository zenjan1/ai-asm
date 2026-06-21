/* kuenowia_admin: Kuenowia management technology administration (v1.0)
 * Kuenowia planning, kuenowia execution, kuenowia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kue_t;
typedef struct{int n_kuep,n_kuee,n_kue2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kue_state_t;
static kue_t kueps[N],kuess[N-2],kuevss[N-4],kueas[N-6],kuemks[N-6]; static kue_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kue_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kue_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KUE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kue_init(void){if(init)return -1;st.n_kuep=0;st.n_kuee=0;st.n_kue2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kueps[i].active=0;for(int i=0;i<N-2;i++)kuess[i].active=0;for(int i=0;i<N-4;i++)kuevss[i].active=0;for(int i=0;i<N-6;i++)kueas[i].active=0;for(int i=0;i<N-6;i++)kuemks[i].active=0;init=1;ps("[KUE] Kuenowia initialized\n");return 0;}
int kue_planning(int t,int c,int a,int b,int d,int e,int y){return add(kueps,&st.n_kuep,&st.t_f1,N,t,c,a,b,d,e,y);}
int kue_execution(int t,int c,int a,int b,int d,int e,int y){return add(kuess,&st.n_kuee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kue_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kuevss,&st.n_kue2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kue_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kueas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kue_market(int t,int c,int a,int b,int d,int e,int y){return add(kuemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kue_report(void){ps("[KUE] Kuep: ");pi(st.n_kuep);ps(" PCS=");pi(st.t_f1);ps("\nKuee: ");pi(st.n_kuee);ps(" PCS=");pi(st.t_f2);ps("\nKuev: ");pi(st.n_kue2);ps(" PCS=");pi(st.t_f3);ps("\nKuec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kue_state(void){ps("[KUE] Kuep=");pi(st.n_kuep);ps(" Kuee=");pi(st.n_kuee);ps(" Kuev=");pi(st.n_kue2);ps(" Kuec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kuenowia Admin Demo ===\n\n");kue_init();
ps("Kuenowia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kue_planning(t,c,1038+(i*17),1027+(i*14),1007+(i*10),989+(i*6),2020+(i%5));}
ps("\nKuenowia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kue_execution(t,c,1027+(i*15),1016+(i*12),998+(i*8),985+(i*5),2021+(i%4));}
ps("\nKuenowia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kue_evaluation(t,c,1019+(i*13),1008+(i*10),992+(i*7),981+(i*4),2022+(i%3));}
ps("\nKuenowia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kue_accessory(t,c,1011+(i*11),1002+(i*9),988+(i*6),978+(i*3),2023+(i%2));}
ps("\nKuenowia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kue_market(t,c,1005+(i*9),996+(i*7),983+(i*5),975+(i*3),2024);}
ps("\n");kue_report();kue_state();ps("\n=== Demo Complete ===\n");return 0;}
