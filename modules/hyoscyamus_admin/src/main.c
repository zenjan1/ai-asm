/* hyoscyamus_admin: Hyoscyamus management technology administration (v1.0)
 * Hyoscyamus planning, hyoscyamus execution, hyoscyamus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hyo_t;
typedef struct{int n_hyop,n_hyoe,n_hyo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hyo_state_t;
static hyo_t hyops[N],hyoss[N-2],hyovss[N-4],hyoas[N-6],hyomks[N-6]; static hyo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hyo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hyo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hyo_init(void){if(init)return -1;st.n_hyop=0;st.n_hyoe=0;st.n_hyo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hyops[i].active=0;for(int i=0;i<N-2;i++)hyoss[i].active=0;for(int i=0;i<N-4;i++)hyovss[i].active=0;for(int i=0;i<N-6;i++)hyoas[i].active=0;for(int i=0;i<N-6;i++)hyomks[i].active=0;init=1;ps("[HYO] Hyoscyamus initialized\n");return 0;}
int hyo_planning(int t,int c,int a,int b,int d,int e,int y){return add(hyops,&st.n_hyop,&st.t_f1,N,t,c,a,b,d,e,y);}
int hyo_execution(int t,int c,int a,int b,int d,int e,int y){return add(hyoss,&st.n_hyoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hyo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hyovss,&st.n_hyo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hyo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hyoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hyo_market(int t,int c,int a,int b,int d,int e,int y){return add(hyomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hyo_report(void){ps("[HYO] Hyop: ");pi(st.n_hyop);ps(" PCS=");pi(st.t_f1);ps("\nHyoe: ");pi(st.n_hyoe);ps(" PCS=");pi(st.t_f2);ps("\nHyov: ");pi(st.n_hyo2);ps(" PCS=");pi(st.t_f3);ps("\nHyoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hyo_state(void){ps("[HYO] Hyop=");pi(st.n_hyop);ps(" Hyoe=");pi(st.n_hyoe);ps(" Hyov=");pi(st.n_hyo2);ps(" Hyoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hyoscyamus Admin Demo ===\n\n");hyo_init();
ps("Hyoscyamus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hyo_planning(t,c,1024+(i*17),1013+(i*14),993+(i*10),975+(i*6),2020+(i%5));}
ps("\nHyoscyamus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hyo_execution(t,c,1013+(i*15),1002+(i*12),984+(i*8),971+(i*5),2021+(i%4));}
ps("\nHyoscyamus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hyo_evaluation(t,c,1005+(i*13),994+(i*10),978+(i*7),967+(i*4),2022+(i%3));}
ps("\nHyoscyamus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyo_accessory(t,c,997+(i*11),988+(i*9),974+(i*6),964+(i*3),2023+(i%2));}
ps("\nHyoscyamus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyo_market(t,c,991+(i*9),982+(i*7),969+(i*5),961+(i*3),2024);}
ps("\n");hyo_report();hyo_state();ps("\n=== Demo Complete ===\n");return 0;}
