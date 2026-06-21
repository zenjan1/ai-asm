/* oedera_admin: Oedera management technology administration (v1.0)
 * Oedera planning, oedera execution, oedera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oed_t;
typedef struct{int n_oedp,n_oede,n_oed2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oed_state_t;
static oed_t oedps[N],oedss[N-2],oedvss[N-4],oedas[N-6],oedmks[N-6]; static oed_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oed_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oed_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OED] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oed_init(void){if(init)return -1;st.n_oedp=0;st.n_oede=0;st.n_oed2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)oedps[i].active=0;for(int i=0;i<N-2;i++)oedss[i].active=0;for(int i=0;i<N-4;i++)oedvss[i].active=0;for(int i=0;i<N-6;i++)oedas[i].active=0;for(int i=0;i<N-6;i++)oedmks[i].active=0;init=1;ps("[OED] Oedera initialized\n");return 0;}
int oed_planning(int t,int c,int a,int b,int d,int e,int y){return add(oedps,&st.n_oedp,&st.t_f1,N,t,c,a,b,d,e,y);}
int oed_execution(int t,int c,int a,int b,int d,int e,int y){return add(oedss,&st.n_oede,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oed_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(oedvss,&st.n_oed2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oed_accessory(int t,int c,int a,int b,int d,int e,int y){return add(oedas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oed_market(int t,int c,int a,int b,int d,int e,int y){return add(oedmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oed_report(void){ps("[OED] Oedp: ");pi(st.n_oedp);ps(" PCS=");pi(st.t_f1);ps("\nOede: ");pi(st.n_oede);ps(" PCS=");pi(st.t_f2);ps("\nOedv: ");pi(st.n_oed2);ps(" PCS=");pi(st.t_f3);ps("\nOedc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oed_state(void){ps("[OED] Oedp=");pi(st.n_oedp);ps(" Oede=");pi(st.n_oede);ps(" Oedv=");pi(st.n_oed2);ps(" Oedc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oedera Admin Demo ===\n\n");oed_init();
ps("Oedera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oed_planning(t,c,1073+(i*17),1062+(i*14),1042+(i*10),1024+(i*6),2020+(i%5));}
ps("\nOedera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oed_execution(t,c,1062+(i*15),1051+(i*12),1033+(i*8),1020+(i*5),2021+(i%4));}
ps("\nOedera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oed_evaluation(t,c,1054+(i*13),1043+(i*10),1027+(i*7),1016+(i*4),2022+(i%3));}
ps("\nOedera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oed_accessory(t,c,1046+(i*11),1037+(i*9),1023+(i*6),1013+(i*3),2023+(i%2));}
ps("\nOedera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oed_market(t,c,1040+(i*9),1031+(i*7),1018+(i*5),1010+(i*3),2024);}
ps("\n");oed_report();oed_state();ps("\n=== Demo Complete ===\n");return 0;}
