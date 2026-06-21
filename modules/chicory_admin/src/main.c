/* chicory_admin: Chicory management technology administration (v1.0)
 * Chicory planning, chicory execution, chicory evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cr_t;
typedef struct{int n_chp,n_che,n_chv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cr_state_t;
static cr_t chps[N],chss[N-2],chvss[N-4],chas[N-6],chmks[N-6]; static cr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ch_init(void){if(init)return -1;st.n_chp=0;st.n_che=0;st.n_chv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)chps[i].active=0;for(int i=0;i<N-2;i++)chss[i].active=0;for(int i=0;i<N-4;i++)chvss[i].active=0;for(int i=0;i<N-6;i++)chas[i].active=0;for(int i=0;i<N-6;i++)chmks[i].active=0;init=1;ps("[CHC] Chicory initialized\n");return 0;}
int ch_planning(int t,int c,int a,int b,int d,int e,int y){return add(chps,&st.n_chp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ch_execution(int t,int c,int a,int b,int d,int e,int y){return add(chss,&st.n_che,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ch_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(chvss,&st.n_chv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ch_accessory(int t,int c,int a,int b,int d,int e,int y){return add(chas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ch_market(int t,int c,int a,int b,int d,int e,int y){return add(chmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ch_report(void){ps("[CHC] Chp: ");pi(st.n_chp);ps(" PCS=");pi(st.t_f1);ps("\nChe: ");pi(st.n_che);ps(" PCS=");pi(st.t_f2);ps("\nChv: ");pi(st.n_chv);ps(" PCS=");pi(st.t_f3);ps("\nChc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ch_state(void){ps("[CHC] Chp=");pi(st.n_chp);ps(" Che=");pi(st.n_che);ps(" Chv=");pi(st.n_chv);ps(" Chc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chicory Admin Demo ===\n\n");ch_init();
ps("Chicory planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ch_planning(t,c,715+(i*17),704+(i*14),684+(i*10),666+(i*6),2020+(i%5));}
ps("\nChicory execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ch_execution(t,c,704+(i*15),693+(i*12),675+(i*8),662+(i*5),2021+(i%4));}
ps("\nChicory evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ch_evaluation(t,c,696+(i*13),685+(i*10),669+(i*7),658+(i*4),2022+(i%3));}
ps("\nChicory accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ch_accessory(t,c,688+(i*11),679+(i*9),665+(i*6),655+(i*3),2023+(i%2));}
ps("\nChicory marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ch_market(t,c,682+(i*9),673+(i*7),660+(i*5),652+(i*3),2024);}
ps("\n");ch_report();ch_state();ps("\n=== Demo Complete ===\n");return 0;}
