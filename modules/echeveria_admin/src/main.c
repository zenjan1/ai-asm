/* echeveria_admin: Echeveria management technology administration (v1.0)
 * Echeveria planning, echeveria execution, echeveria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ea_t;
typedef struct{int n_ece,n_eex,n_ecv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ea_state_t;
static ea_t eces[N],ecss[N-2],ecvss[N-4],ecas[N-6],ecmks[N-6]; static ea_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ea_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ea_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ECE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ece_init(void){if(init)return -1;st.n_ece=0;st.n_eex=0;st.n_ecv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eces[i].active=0;for(int i=0;i<N-2;i++)ecss[i].active=0;for(int i=0;i<N-4;i++)ecvss[i].active=0;for(int i=0;i<N-6;i++)ecas[i].active=0;for(int i=0;i<N-6;i++)ecmks[i].active=0;init=1;ps("[ECE] Echeveria initialized\n");return 0;}
int ece_planning(int t,int c,int a,int b,int d,int e,int y){return add(eces,&st.n_ece,&st.t_f1,N,t,c,a,b,d,e,y);}
int ece_execution(int t,int c,int a,int b,int d,int e,int y){return add(ecss,&st.n_eex,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ece_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ecvss,&st.n_ecv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ece_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ecas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ece_market(int t,int c,int a,int b,int d,int e,int y){return add(ecmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ece_report(void){ps("[ECE] Ece: ");pi(st.n_ece);ps(" PCS=");pi(st.t_f1);ps("\nEex: ");pi(st.n_eex);ps(" PCS=");pi(st.t_f2);ps("\nEcv: ");pi(st.n_ecv);ps(" PCS=");pi(st.t_f3);ps("\nEcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ece_state(void){ps("[ECE] Ece=");pi(st.n_ece);ps(" Eex=");pi(st.n_eex);ps(" Ecv=");pi(st.n_ecv);ps(" Ecc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Echeveria Admin Demo ===\n\n");ece_init();
ps("Echeveria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ece_planning(t,c,770+(i*17),759+(i*14),739+(i*10),721+(i*6),2020+(i%5));}
ps("\nEcheveria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ece_execution(t,c,759+(i*15),748+(i*12),730+(i*8),717+(i*5),2021+(i%4));}
ps("\nEcheveria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ece_evaluation(t,c,751+(i*13),740+(i*10),726+(i*7),715+(i*4),2022+(i%3));}
ps("\nEcheveria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ece_accessory(t,c,743+(i*11),734+(i*9),720+(i*6),710+(i*3),2023+(i%2));}
ps("\nEcheveria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ece_market(t,c,737+(i*9),728+(i*7),715+(i*5),707+(i*3),2024);}
ps("\n");ece_report();ece_state();ps("\n=== Demo Complete ===\n");return 0;}
