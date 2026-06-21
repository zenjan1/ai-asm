/* laelia_admin: Laelia management technology administration (v1.0)
 * Laelia planning, laelia execution, laelia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lai_t;
typedef struct{int n_laip,n_laie,n_lai2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lai_state_t;
static lai_t laips[N],laiss[N-2],laivss[N-4],laias[N-6],laimks[N-6]; static lai_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lai_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lai_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lai_init(void){if(init)return -1;st.n_laip=0;st.n_laie=0;st.n_lai2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)laips[i].active=0;for(int i=0;i<N-2;i++)laiss[i].active=0;for(int i=0;i<N-4;i++)laivss[i].active=0;for(int i=0;i<N-6;i++)laias[i].active=0;for(int i=0;i<N-6;i++)laimks[i].active=0;init=1;ps("[LAI] Laelia initialized\n");return 0;}
int lai_planning(int t,int c,int a,int b,int d,int e,int y){return add(laips,&st.n_laip,&st.t_f1,N,t,c,a,b,d,e,y);}
int lai_execution(int t,int c,int a,int b,int d,int e,int y){return add(laiss,&st.n_laie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lai_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(laivss,&st.n_lai2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lai_accessory(int t,int c,int a,int b,int d,int e,int y){return add(laias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lai_market(int t,int c,int a,int b,int d,int e,int y){return add(laimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lai_report(void){ps("[LAI] Laip: ");pi(st.n_laip);ps(" PCS=");pi(st.t_f1);ps("\nLaie: ");pi(st.n_laie);ps(" PCS=");pi(st.t_f2);ps("\nLaiv: ");pi(st.n_lai2);ps(" PCS=");pi(st.t_f3);ps("\nLaic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lai_state(void){ps("[LAI] Laip=");pi(st.n_laip);ps(" Laie=");pi(st.n_laie);ps(" Laiv=");pi(st.n_lai2);ps(" Laic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Laelia Admin Demo ===\n\n");lai_init();
ps("Laelia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lai_planning(t,c,1041+(i*17),1030+(i*14),1010+(i*10),992+(i*6),2020+(i%5));}
ps("\nLaelia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lai_execution(t,c,1030+(i*15),1019+(i*12),1001+(i*8),988+(i*5),2021+(i%4));}
ps("\nLaelia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lai_evaluation(t,c,1022+(i*13),1011+(i*10),995+(i*7),984+(i*4),2022+(i%3));}
ps("\nLaelia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lai_accessory(t,c,1014+(i*11),1005+(i*9),991+(i*6),981+(i*3),2023+(i%2));}
ps("\nLaelia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lai_market(t,c,1008+(i*9),999+(i*7),986+(i*5),978+(i*3),2024);}
ps("\n");lai_report();lai_state();ps("\n=== Demo Complete ===\n");return 0;}
