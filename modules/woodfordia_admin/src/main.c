/* woodfordia_admin: Woodfordia management technology administration (v1.0)
 * Woodfordia planning, woodfordia execution, woodfordia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wb_t;
typedef struct{int n_wbp,n_wbe,n_wbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wb_state_t;
static wb_t wbps[N],wbss[N-2],wbvss[N-4],wbas[N-6],wbmks[N-6]; static wb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WBN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wbn_init(void){if(init)return -1;st.n_wbp=0;st.n_wbe=0;st.n_wbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wbps[i].active=0;for(int i=0;i<N-2;i++)wbss[i].active=0;for(int i=0;i<N-4;i++)wbvss[i].active=0;for(int i=0;i<N-6;i++)wbas[i].active=0;for(int i=0;i<N-6;i++)wbmks[i].active=0;init=1;ps("[WBN] Woodfordia initialized\n");return 0;}
int wbn_planning(int t,int c,int a,int b,int d,int e,int y){return add(wbps,&st.n_wbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wbn_execution(int t,int c,int a,int b,int d,int e,int y){return add(wbss,&st.n_wbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wbn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wbvss,&st.n_wbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wbn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wbn_market(int t,int c,int a,int b,int d,int e,int y){return add(wbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wbn_report(void){ps("[WBN] Wbp: ");pi(st.n_wbp);ps(" PCS=");pi(st.t_f1);ps("\nWbe: ");pi(st.n_wbe);ps(" PCS=");pi(st.t_f2);ps("\nWbv: ");pi(st.n_wbv);ps(" PCS=");pi(st.t_f3);ps("\nWbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wbn_state(void){ps("[WBN] Wbp=");pi(st.n_wbp);ps(" Wbe=");pi(st.n_wbe);ps(" Wbv=");pi(st.n_wbv);ps(" Wbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Woodfordia Admin Demo ===\n\n");wbn_init();
ps("Woodfordia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wbn_planning(t,c,753+(i*17),742+(i*14),722+(i*10),704+(i*6),2020+(i%5));}
ps("\nWoodfordia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wbn_execution(t,c,742+(i*15),731+(i*12),713+(i*8),700+(i*5),2021+(i%4));}
ps("\nWoodfordia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wbn_evaluation(t,c,734+(i*13),723+(i*10),707+(i*7),696+(i*4),2022+(i%3));}
ps("\nWoodfordia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wbn_accessory(t,c,726+(i*11),717+(i*9),703+(i*6),693+(i*3),2023+(i%2));}
ps("\nWoodfordia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wbn_market(t,c,720+(i*9),711+(i*7),698+(i*5),690+(i*3),2024);}
ps("\n");wbn_report();wbn_state();ps("\n=== Demo Complete ===\n");return 0;}
