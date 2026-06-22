/* townsendia_admin: Townsendia management technology administration (v1.0)
 * Townsendia planning, townsendia execution, townsendia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tow_t;
typedef struct{int n_towp,n_towe,n_tow2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tow_state_t;
static tow_t towps[N],towss[N-2],towvss[N-4],towas[N-6],towmks[N-6]; static tow_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tow_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tow_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TOW] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tow_init(void){if(init)return -1;st.n_towp=0;st.n_towe=0;st.n_tow2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)towps[i].active=0;for(int i=0;i<N-2;i++)towss[i].active=0;for(int i=0;i<N-4;i++)towvss[i].active=0;for(int i=0;i<N-6;i++)towas[i].active=0;for(int i=0;i<N-6;i++)towmks[i].active=0;init=1;ps("[TOW] Townsendia initialized\n");return 0;}
int tow_planning(int t,int c,int a,int b,int d,int e,int y){return add(towps,&st.n_towp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tow_execution(int t,int c,int a,int b,int d,int e,int y){return add(towss,&st.n_towe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tow_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(towvss,&st.n_tow2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tow_accessory(int t,int c,int a,int b,int d,int e,int y){return add(towas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tow_market(int t,int c,int a,int b,int d,int e,int y){return add(towmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tow_report(void){ps("[TOW] Towpp: ");pi(st.n_towp);ps(" PCS=");pi(st.t_f1);ps("\nTowe: ");pi(st.n_towe);ps(" PCS=");pi(st.t_f2);ps("\nTowv: ");pi(st.n_tow2);ps(" PCS=");pi(st.t_f3);ps("\nTowc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tow_state(void){ps("[TOW] Towpp=");pi(st.n_towp);ps(" Towe=");pi(st.n_towe);ps(" Towv=");pi(st.n_tow2);ps(" Towc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Townsendia Admin Demo ===\n\n");tow_init();
ps("Townsendia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tow_planning(t,c,1108+(i*17),1097+(i*14),1077+(i*10),1059+(i*6),2020+(i%5));}
ps("\nTownsendia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tow_execution(t,c,1097+(i*15),1086+(i*12),1068+(i*8),1055+(i*5),2021+(i%4));}
ps("\nTownsendia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tow_evaluation(t,c,1089+(i*13),1078+(i*10),1062+(i*7),1051+(i*4),2022+(i%3));}
ps("\nTownsendia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tow_accessory(t,c,1081+(i*11),1072+(i*9),1058+(i*6),1048+(i*3),2023+(i%2));}
ps("\nTownsendia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tow_market(t,c,1075+(i*9),1066+(i*7),1053+(i*5),1045+(i*3),2024);}
ps("\n");tow_report();tow_state();ps("\n=== Demo Complete ===\n");return 0;}
