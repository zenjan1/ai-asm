/* doronicum_admin: Doronicum management technology administration (v1.0)
 * Doronicum planning, doronicum execution, doronicum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} do_t;
typedef struct{int n_don,n_doe,n_dov,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} do_state_t;
static do_t dons[N],doss[N-2],dovss[N-4],doas[N-6],domks[N-6]; static do_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(do_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;do_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int don_init(void){if(init)return -1;st.n_don=0;st.n_doe=0;st.n_dov=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dons[i].active=0;for(int i=0;i<N-2;i++)doss[i].active=0;for(int i=0;i<N-4;i++)dovss[i].active=0;for(int i=0;i<N-6;i++)doas[i].active=0;for(int i=0;i<N-6;i++)domks[i].active=0;init=1;ps("[DON] Doronicum initialized\n");return 0;}
int don_planning(int t,int c,int a,int b,int d,int e,int y){return add(dons,&st.n_don,&st.t_f1,N,t,c,a,b,d,e,y);}
int don_execution(int t,int c,int a,int b,int d,int e,int y){return add(doss,&st.n_doe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int don_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dovss,&st.n_dov,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int don_accessory(int t,int c,int a,int b,int d,int e,int y){return add(doas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int don_market(int t,int c,int a,int b,int d,int e,int y){return add(domks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void don_report(void){ps("[DON] Don: ");pi(st.n_don);ps(" PCS=");pi(st.t_f1);ps("\nDoe: ");pi(st.n_doe);ps(" PCS=");pi(st.t_f2);ps("\nDov: ");pi(st.n_dov);ps(" PCS=");pi(st.t_f3);ps("\nDoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void don_state(void){ps("[DON] Don=");pi(st.n_don);ps(" Doe=");pi(st.n_doe);ps(" Dov=");pi(st.n_dov);ps(" Doc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Doronicum Admin Demo ===\n\n");don_init();
ps("Doronicum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;don_planning(t,c,769+(i*17),758+(i*14),738+(i*10),720+(i*6),2020+(i%5));}
ps("\nDoronicum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;don_execution(t,c,758+(i*15),747+(i*12),729+(i*8),716+(i*5),2021+(i%4));}
ps("\nDoronicum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;don_evaluation(t,c,750+(i*13),739+(i*10),725+(i*7),714+(i*4),2022+(i%3));}
ps("\nDoronicum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;don_accessory(t,c,742+(i*11),733+(i*9),719+(i*6),709+(i*3),2023+(i%2));}
ps("\nDoronicum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;don_market(t,c,736+(i*9),727+(i*7),714+(i*5),706+(i*3),2024);}
ps("\n");don_report();don_state();ps("\n=== Demo Complete ===\n");return 0;}
