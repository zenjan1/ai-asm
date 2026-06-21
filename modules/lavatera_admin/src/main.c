/* lavatera_admin: Lavatera management technology administration (v1.0)
 * Lavatera planning, lavatera execution, lavatera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lvt_t;
typedef struct{int n_ltp,n_lte,n_ltv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lvt_state_t;
static lvt_t lvtps[N],lvtess[N-2],lvtvss[N-4],lvtas[N-6],lvtmks[N-6]; static lvt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lvt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lvt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LVT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lvt_init(void){if(init)return -1;st.n_ltp=0;st.n_lte=0;st.n_ltv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lvtps[i].active=0;for(int i=0;i<N-2;i++)lvtess[i].active=0;for(int i=0;i<N-4;i++)lvtvss[i].active=0;for(int i=0;i<N-6;i++)lvtas[i].active=0;for(int i=0;i<N-6;i++)lvtmks[i].active=0;init=1;ps("[LVT] Lavatera initialized\n");return 0;}
int lvt_planning(int t,int c,int a,int b,int d,int e,int y){return add(lvtps,&st.n_ltp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lvt_execution(int t,int c,int a,int b,int d,int e,int y){return add(lvtess,&st.n_lte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lvt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lvtvss,&st.n_ltv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lvt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lvtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lvt_market(int t,int c,int a,int b,int d,int e,int y){return add(lvtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lvt_report(void){ps("[LVT] Ltp: ");pi(st.n_ltp);ps(" PCS=");pi(st.t_f1);ps("\nLte: ");pi(st.n_lte);ps(" PCS=");pi(st.t_f2);ps("\nLtv: ");pi(st.n_ltv);ps(" PCS=");pi(st.t_f3);ps("\nLtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lvt_state(void){ps("[LVT] Ltp=");pi(st.n_ltp);ps(" Lte=");pi(st.n_lte);ps(" Ltv=");pi(st.n_ltv);ps(" Ltc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lavatera Admin Demo ===\n\n");lvt_init();
ps("Lavatera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lvt_planning(t,c,830+(i*17),819+(i*14),799+(i*10),781+(i*6),2020+(i%5));}
ps("\nLavatera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lvt_execution(t,c,819+(i*15),808+(i*12),790+(i*8),777+(i*5),2021+(i%4));}
ps("\nLavatera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lvt_evaluation(t,c,811+(i*13),800+(i*10),784+(i*7),773+(i*4),2022+(i%3));}
ps("\nLavatera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lvt_accessory(t,c,803+(i*11),794+(i*9),780+(i*6),770+(i*3),2023+(i%2));}
ps("\nLavatera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lvt_market(t,c,797+(i*9),788+(i*7),775+(i*5),767+(i*3),2024);}
ps("\n");lvt_report();lvt_state();ps("\n=== Demo Complete ===\n");return 0;}
