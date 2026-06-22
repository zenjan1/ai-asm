/* acer_admin: Acer management technology administration (v1.0)
 * Acer planning, acer execution, acer evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acr_t;
typedef struct{int n_acrp,n_acre,n_acr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acr_state_t;
static acr_t acrps[N],acrss[N-2],acrvss[N-4],acras[N-6],acrmks[N-6]; static acr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(acr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acr_init(void){if(init)return -1;st.n_acrp=0;st.n_acre=0;st.n_acr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acrps[i].active=0;for(int i=0;i<N-2;i++)acrss[i].active=0;for(int i=0;i<N-4;i++)acrvss[i].active=0;for(int i=0;i<N-6;i++)acras[i].active=0;for(int i=0;i<N-6;i++)acrmks[i].active=0;init=1;ps("[ACR] Acer initialized\n");return 0;}
int acr_planning(int t,int c,int a,int b,int d,int e,int y){return add(acrps,&st.n_acrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int acr_execution(int t,int c,int a,int b,int d,int e,int y){return add(acrss,&st.n_acre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(acrvss,&st.n_acr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acr_market(int t,int c,int a,int b,int d,int e,int y){return add(acrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acr_report(void){ps("[ACR] Acepp: ");pi(st.n_acrp);ps(" PCS=");pi(st.t_f1);ps("\nAcepe: ");pi(st.n_acre);ps(" PCS=");pi(st.t_f2);ps("\nAcev: ");pi(st.n_acr2);ps(" PCS=");pi(st.t_f3);ps("\nAcec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acr_state(void){ps("[ACR] Acepp=");pi(st.n_acrp);ps(" Acepe=");pi(st.n_acre);ps(" Acev=");pi(st.n_acr2);ps(" Acec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Acer Admin Demo ===\n\n");acr_init();
ps("Acer planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acr_planning(t,c,1212+(i*17),1201+(i*14),1181+(i*10),1163+(i*6),2020+(i%5));}
ps("\nAcer execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acr_execution(t,c,1201+(i*15),1190+(i*12),1172+(i*8),1159+(i*5),2021+(i%4));}
ps("\nAcer evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acr_evaluation(t,c,1193+(i*13),1182+(i*10),1166+(i*7),1155+(i*4),2022+(i%3));}
ps("\nAcer accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acr_accessory(t,c,1185+(i*11),1176+(i*9),1162+(i*6),1152+(i*3),2023+(i%2));}
ps("\nAcer marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acr_market(t,c,1179+(i*9),1170+(i*7),1157+(i*5),1149+(i*3),2024);}
ps("\n");acr_report();acr_state();ps("\n=== Demo Complete ===\n");return 0;}
