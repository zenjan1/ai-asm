/* koelreuteria_admin: Koelreuteria management technology administration (v1.0)
 * Koelreuteria planning, koelreuteria execution, koelreuteria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} koel_t;
typedef struct{int n_koelp,n_koel,n_koel2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} koel_state_t;
static koel_t koels[N],koel_e[N-2],koel2[N-4],koelac[N-6],koelam[N-6]; static koel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(koel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;koel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KOEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int koel_init(void){if(init)return -1;st.n_koelp=0;st.n_koel=0;st.n_koel2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)koels[i].active=0;for(int i=0;i<N-2;i++)koel_e[i].active=0;for(int i=0;i<N-4;i++)koel2[i].active=0;for(int i=0;i<N-6;i++)koelac[i].active=0;for(int i=0;i<N-6;i++)koelam[i].active=0;init=1;ps("[KOEL] Koelreuteria initialized\n");return 0;}
int koel_planning(int t,int c,int a,int b,int d,int e,int y){return add(koels,&st.n_koelp,&st.t_f1,N,t,c,a,b,d,e,y);}
int koel_execution(int t,int c,int a,int b,int d,int e,int y){return add(koel_e,&st.n_koel,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int koel_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(koel2,&st.n_koel2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int koel_accessory(int t,int c,int a,int b,int d,int e,int y){return add(koelac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int koel_market(int t,int c,int a,int b,int d,int e,int y){return add(koelam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void koel_report(void){ps("[KOEL] Koelp: ");pi(st.n_koelp);ps(" PCS=");pi(st.t_f1);ps("\nKoel: ");pi(st.n_koel);ps(" PCS=");pi(st.t_f2);ps("\nKoel2: ");pi(st.n_koel2);ps(" PCS=");pi(st.t_f3);ps("\nKac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void koel_state(void){ps("[KOEL] Koelp=");pi(st.n_koelp);ps(" Koel=");pi(st.n_koel);ps(" Koel2=");pi(st.n_koel2);ps(" Kac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Koelreuteria Admin Demo ===\n\n");koel_init();
ps("Koelreuteria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;koel_planning(t,c,1549+(i*17),1538+(i*14),1518+(i*10),1500+(i*6),2020+(i%5));}
ps("\nKoelreuteria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;koel_execution(t,c,1538+(i*15),1527+(i*12),1509+(i*8),1496+(i*5),2021+(i%4));}
ps("\nKoelreuteria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;koel_evaluation(t,c,1530+(i*13),1519+(i*10),1503+(i*7),1492+(i*4),2022+(i%3));}
ps("\nKoelreuteria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;koel_accessory(t,c,1522+(i*11),1513+(i*9),1499+(i*6),1489+(i*3),2023+(i%2));}
ps("\nKoelreuteria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;koel_market(t,c,1516+(i*9),1507+(i*7),1494+(i*5),1486+(i*3),2024);}
ps("\n");koel_report();koel_state();ps("\n=== Demo Complete ===\n");return 0;}
