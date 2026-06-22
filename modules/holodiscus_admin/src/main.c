/* holodiscus_admin: Holodiscus management technology administration (v1.0)
 * Holodiscus planning, holodiscus execution, holodiscus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} holo_t;
typedef struct{int n_holop,n_holo,n_holo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} holo_state_t;
static holo_t holos[N],holoe[N-2],holo2[N-4],holoac[N-6],holoam[N-6]; static holo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(holo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;holo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HOLO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int holo_init(void){if(init)return -1;st.n_holop=0;st.n_holo=0;st.n_holo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)holos[i].active=0;for(int i=0;i<N-2;i++)holoe[i].active=0;for(int i=0;i<N-4;i++)holo2[i].active=0;for(int i=0;i<N-6;i++)holoac[i].active=0;for(int i=0;i<N-6;i++)holoam[i].active=0;init=1;ps("[HOLO] Holodiscus initialized\n");return 0;}
int holo_planning(int t,int c,int a,int b,int d,int e,int y){return add(holos,&st.n_holop,&st.t_f1,N,t,c,a,b,d,e,y);}
int holo_execution(int t,int c,int a,int b,int d,int e,int y){return add(holoe,&st.n_holo,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int holo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(holo2,&st.n_holo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int holo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(holoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int holo_market(int t,int c,int a,int b,int d,int e,int y){return add(holoam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void holo_report(void){ps("[HOLO] Holop: ");pi(st.n_holop);ps(" PCS=");pi(st.t_f1);ps("\nHoloe: ");pi(st.n_holo);ps(" PCS=");pi(st.t_f2);ps("\nHolo2: ");pi(st.n_holo2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void holo_state(void){ps("[HOLO] Holop=");pi(st.n_holop);ps(" Holo=");pi(st.n_holo);ps(" Holo2=");pi(st.n_holo2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Holodiscus Admin Demo ===\n\n");holo_init();
ps("Holodiscus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;holo_planning(t,c,1533+(i*17),1522+(i*14),1502+(i*10),1484+(i*6),2020+(i%5));}
ps("\nHolodiscus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;holo_execution(t,c,1522+(i*15),1511+(i*12),1493+(i*8),1480+(i*5),2021+(i%4));}
ps("\nHolodiscus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;holo_evaluation(t,c,1514+(i*13),1503+(i*10),1487+(i*7),1476+(i*4),2022+(i%3));}
ps("\nHolodiscus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;holo_accessory(t,c,1506+(i*11),1497+(i*9),1483+(i*6),1473+(i*3),2023+(i%2));}
ps("\nHolodiscus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;holo_market(t,c,1500+(i*9),1491+(i*7),1478+(i*5),1470+(i*3),2024);}
ps("\n");holo_report();holo_state();ps("\n=== Demo Complete ===\n");return 0;}
