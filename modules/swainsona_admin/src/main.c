/* swainsona_admin: Swainsona management technology administration (v1.0)
 * Swainsona planning, swainsona execution, swainsona evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} swai_t;
typedef struct{int n_swaip,n_swaie,n_swai2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} swai_state_t;
static swai_t swais[N],swaiess[N-2],swaivss[N-4],swaias[N-6],swaimks[N-6]; static swai_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(swai_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;swai_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SWAI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int swai_init(void){if(init)return -1;st.n_swaip=0;st.n_swaie=0;st.n_swai2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)swais[i].active=0;for(int i=0;i<N-2;i++)swaiess[i].active=0;for(int i=0;i<N-4;i++)swaivss[i].active=0;for(int i=0;i<N-6;i++)swaias[i].active=0;for(int i=0;i<N-6;i++)swaimks[i].active=0;init=1;ps("[SWAI] Swainsona initialized\n");return 0;}
int swai_planning(int t,int c,int a,int b,int d,int e,int y){return add(swais,&st.n_swaip,&st.t_f1,N,t,c,a,b,d,e,y);}
int swai_execution(int t,int c,int a,int b,int d,int e,int y){return add(swaiess,&st.n_swaie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int swai_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(swaivss,&st.n_swai2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int swai_accessory(int t,int c,int a,int b,int d,int e,int y){return add(swaias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int swai_market(int t,int c,int a,int b,int d,int e,int y){return add(swaimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void swai_report(void){ps("[SWAI] Swaip: ");pi(st.n_swaip);ps(" PCS=");pi(st.t_f1);ps("\nSwaie: ");pi(st.n_swaie);ps(" PCS=");pi(st.t_f2);ps("\nSwaiv: ");pi(st.n_swai2);ps(" PCS=");pi(st.t_f3);ps("\nSwaic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void swai_state(void){ps("[SWAI] Swaip=");pi(st.n_swaip);ps(" Swaie=");pi(st.n_swaie);ps(" Swaiv=");pi(st.n_swai2);ps(" Swaic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Swainsona Admin Demo ===\n\n");swai_init();
ps("Swainsona planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;swai_planning(t,c,1343+(i*17),1332+(i*14),1312+(i*10),1294+(i*6),2020+(i%5));}
ps("\nSwainsona execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;swai_execution(t,c,1332+(i*15),1321+(i*12),1303+(i*8),1290+(i*5),2021+(i%4));}
ps("\nSwainsona evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;swai_evaluation(t,c,1324+(i*13),1313+(i*10),1297+(i*7),1286+(i*4),2022+(i%3));}
ps("\nSwainsona accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;swai_accessory(t,c,1316+(i*11),1307+(i*9),1293+(i*6),1283+(i*3),2023+(i%2));}
ps("\nSwainsona marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;swai_market(t,c,1310+(i*9),1301+(i*7),1288+(i*5),1280+(i*3),2024);}
ps("\n");swai_report();swai_state();ps("\n=== Demo Complete ===\n");return 0;}
