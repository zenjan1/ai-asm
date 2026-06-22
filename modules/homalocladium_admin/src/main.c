/* homalocladium_admin: Homalocladium management technology administration (v1.0)
 * Homalocladium planning, homalocladium execution, homalocladium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hom_t;
typedef struct{int n_homp,n_home,n_hom2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hom_state_t;
static hom_t homps[N],homss[N-2],homvss[N-4],homas[N-6],hommks[N-6]; static hom_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hom_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hom_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HOM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hom_init(void){if(init)return -1;st.n_homp=0;st.n_home=0;st.n_hom2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)homps[i].active=0;for(int i=0;i<N-2;i++)homss[i].active=0;for(int i=0;i<N-4;i++)homvss[i].active=0;for(int i=0;i<N-6;i++)homas[i].active=0;for(int i=0;i<N-6;i++)hommks[i].active=0;init=1;ps("[HOM] Homalocladium initialized\n");return 0;}
int hom_planning(int t,int c,int a,int b,int d,int e,int y){return add(homps,&st.n_homp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hom_execution(int t,int c,int a,int b,int d,int e,int y){return add(homss,&st.n_home,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hom_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(homvss,&st.n_hom2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hom_accessory(int t,int c,int a,int b,int d,int e,int y){return add(homas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hom_market(int t,int c,int a,int b,int d,int e,int y){return add(hommks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hom_report(void){ps("[HOM] Homp: ");pi(st.n_homp);ps(" PCS=");pi(st.t_f1);ps("\nHome: ");pi(st.n_home);ps(" PCS=");pi(st.t_f2);ps("\nHomv: ");pi(st.n_hom2);ps(" PCS=");pi(st.t_f3);ps("\nHomc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hom_state(void){ps("[HOM] Homp=");pi(st.n_homp);ps(" Home=");pi(st.n_home);ps(" Homv=");pi(st.n_hom2);ps(" Homc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Homalocladium Admin Demo ===\n\n");hom_init();
ps("Homalocladium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hom_planning(t,c,1296+(i*17),1285+(i*14),1265+(i*10),1247+(i*6),2020+(i%5));}
ps("\nHomalocladium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hom_execution(t,c,1285+(i*15),1274+(i*12),1256+(i*8),1243+(i*5),2021+(i%4));}
ps("\nHomalocladium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hom_evaluation(t,c,1277+(i*13),1266+(i*10),1250+(i*7),1239+(i*4),2022+(i%3));}
ps("\nHomalocladium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hom_accessory(t,c,1269+(i*11),1260+(i*9),1246+(i*6),1236+(i*3),2023+(i%2));}
ps("\nHomalocladium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hom_market(t,c,1263+(i*9),1254+(i*7),1241+(i*5),1233+(i*3),2024);}
ps("\n");hom_report();hom_state();ps("\n=== Demo Complete ===\n");return 0;}
