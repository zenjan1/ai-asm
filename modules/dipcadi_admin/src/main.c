/* dipcadi_admin: Dipcadi management technology administration (v1.0)
 * Dipcadi planning, dipcadi execution, dipcadi evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dip_t;
typedef struct{int n_dip,n_die,n_div,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dip_state_t;
static dip_t dips[N],dies[N-2],divs[N-4],acs[N-6],mks[N-6]; static dip_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dip_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dip_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DIP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dip_init(void){if(init)return -1;st.n_dip=0;st.n_die=0;st.n_div=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dips[i].active=0;for(int i=0;i<N-2;i++)dies[i].active=0;for(int i=0;i<N-4;i++)divs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DIP] Dipcadi initialized\n");return 0;}
int dip_planning(int t,int c,int a,int b,int d,int e,int y){return add(dips,&st.n_dip,&st.t_f1,N,t,c,a,b,d,e,y);}
int dip_execution(int t,int c,int a,int b,int d,int e,int y){return add(dies,&st.n_die,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dip_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(divs,&st.n_div,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dip_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dip_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dip_report(void){ps("[DIP] Dip: ");pi(st.n_dip);ps(" PCS=");pi(st.t_f1);ps("\nDie: ");pi(st.n_die);ps(" PCS=");pi(st.t_f2);ps("\nDiv: ");pi(st.n_div);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dip_state(void){ps("[DIP] Dip=");pi(st.n_dip);ps(" Die=");pi(st.n_die);ps(" Div=");pi(st.n_div);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dipcadi Admin Demo ===\n\n");dip_init();
ps("Dipcadi planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dip_planning(t,c,607+(i*17),596+(i*14),576+(i*10),558+(i*6),2020+(i%5));}
ps("\nDipcadi execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dip_execution(t,c,596+(i*15),585+(i*12),567+(i*8),554+(i*5),2021+(i%4));}
ps("\nDipcadi evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dip_evaluation(t,c,588+(i*13),577+(i*10),561+(i*7),550+(i*4),2022+(i%3));}
ps("\nDipcadi accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dip_accessory(t,c,580+(i*11),571+(i*9),557+(i*6),547+(i*3),2023+(i%2));}
ps("\nDipcadi marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dip_market(t,c,574+(i*9),565+(i*7),552+(i*5),544+(i*3),2024);}
ps("\n");dip_report();dip_state();ps("\n=== Demo Complete ===\n");return 0;}
