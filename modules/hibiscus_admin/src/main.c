/* hibiscus_admin: Hibiscus management technology administration (v1.0)
 * Hibiscus planning, hibiscus execution, hibiscus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hib_t;
typedef struct{int n_hp,n_he,n_hv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hib_state_t;
static hib_t hps[N],hes[N-2],hvs[N-4],acs[N-6],mks[N-6]; static hib_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hib_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hib_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HIB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hib_init(void){if(init)return -1;st.n_hp=0;st.n_he=0;st.n_hv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hps[i].active=0;for(int i=0;i<N-2;i++)hes[i].active=0;for(int i=0;i<N-4;i++)hvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HIB] Hibiscus initialized\n");return 0;}
int hib_planning(int t,int c,int a,int b,int d,int e,int y){return add(hps,&st.n_hp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hib_execution(int t,int c,int a,int b,int d,int e,int y){return add(hes,&st.n_he,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hib_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hvs,&st.n_hv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hib_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hib_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hib_report(void){ps("[HIB] Hp: ");pi(st.n_hp);ps(" PCS=");pi(st.t_f1);ps("\nHe: ");pi(st.n_he);ps(" PCS=");pi(st.t_f2);ps("\nHv: ");pi(st.n_hv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hib_state(void){ps("[HIB] Hp=");pi(st.n_hp);ps(" He=");pi(st.n_he);ps(" Hv=");pi(st.n_hv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hibiscus Admin Demo ===\n\n");hib_init();
ps("Hibiscus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hib_planning(t,c,568+(i*17),557+(i*14),537+(i*10),519+(i*6),2020+(i%5));}
ps("\nHibiscus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hib_execution(t,c,557+(i*15),546+(i*12),528+(i*8),515+(i*5),2021+(i%4));}
ps("\nHibiscus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hib_evaluation(t,c,549+(i*13),538+(i*10),522+(i*7),511+(i*4),2022+(i%3));}
ps("\nHibiscus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hib_accessory(t,c,541+(i*11),532+(i*9),518+(i*6),508+(i*3),2023+(i%2));}
ps("\nHibiscus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hib_market(t,c,535+(i*9),526+(i*7),513+(i*5),505+(i*3),2024);}
ps("\n");hib_report();hib_state();ps("\n=== Demo Complete ===\n");return 0;}
