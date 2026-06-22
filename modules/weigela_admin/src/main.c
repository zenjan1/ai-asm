/* weigela_admin: Weigela management technology administration (v1.0)
 * Weigela planning, weigela execution, weigela evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wei_t;
typedef struct{int n_weip,n_weie,n_wei2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wei_state_t;
static wei_t weips[N],weiss[N-2],weivss[N-4],weias[N-6],weimks[N-6]; static wei_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wei_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wei_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WEI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wei_init(void){if(init)return -1;st.n_weip=0;st.n_weie=0;st.n_wei2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)weips[i].active=0;for(int i=0;i<N-2;i++)weiss[i].active=0;for(int i=0;i<N-4;i++)weivss[i].active=0;for(int i=0;i<N-6;i++)weias[i].active=0;for(int i=0;i<N-6;i++)weimks[i].active=0;init=1;ps("[WEI] Weigela initialized\n");return 0;}
int wei_planning(int t,int c,int a,int b,int d,int e,int y){return add(weips,&st.n_weip,&st.t_f1,N,t,c,a,b,d,e,y);}
int wei_execution(int t,int c,int a,int b,int d,int e,int y){return add(weiss,&st.n_weie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wei_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(weivss,&st.n_wei2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wei_accessory(int t,int c,int a,int b,int d,int e,int y){return add(weias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wei_market(int t,int c,int a,int b,int d,int e,int y){return add(weimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wei_report(void){ps("[WEI] Weipp: ");pi(st.n_weip);ps(" PCS=");pi(st.t_f1);ps("\nWeie: ");pi(st.n_weie);ps(" PCS=");pi(st.t_f2);ps("\nWeiv: ");pi(st.n_wei2);ps(" PCS=");pi(st.t_f3);ps("\nWeic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wei_state(void){ps("[WEI] Weipp=");pi(st.n_weip);ps(" Weie=");pi(st.n_weie);ps(" Weiv=");pi(st.n_wei2);ps(" Weic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Weigela Admin Demo ===\n\n");wei_init();
ps("Weigela planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wei_planning(t,c,1111+(i*17),1100+(i*14),1080+(i*10),1062+(i*6),2020+(i%5));}
ps("\nWeigela execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wei_execution(t,c,1100+(i*15),1089+(i*12),1071+(i*8),1058+(i*5),2021+(i%4));}
ps("\nWeigela evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wei_evaluation(t,c,1092+(i*13),1081+(i*10),1065+(i*7),1054+(i*4),2022+(i%3));}
ps("\nWeigela accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wei_accessory(t,c,1084+(i*11),1075+(i*9),1061+(i*6),1051+(i*3),2023+(i%2));}
ps("\nWeigela marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wei_market(t,c,1078+(i*9),1069+(i*7),1056+(i*5),1048+(i*3),2024);}
ps("\n");wei_report();wei_state();ps("\n=== Demo Complete ===\n");return 0;}
