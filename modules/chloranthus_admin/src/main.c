/* chloranthus_admin: Chloranthus management technology administration (v1.0)
 * Chloranthus planning, chloranthus execution, chloranthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} chl_t;
typedef struct{int n_chlp,n_chle,n_chl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} chl_state_t;
static chl_t chlps[N],chlss[N-2],chlvss[N-4],chlas[N-6],chlmks[N-6]; static chl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(chl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;chl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int chl_init(void){if(init)return -1;st.n_chlp=0;st.n_chle=0;st.n_chl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)chlps[i].active=0;for(int i=0;i<N-2;i++)chlss[i].active=0;for(int i=0;i<N-4;i++)chlvss[i].active=0;for(int i=0;i<N-6;i++)chlas[i].active=0;for(int i=0;i<N-6;i++)chlmks[i].active=0;init=1;ps("[CHL] Chloranthus initialized\n");return 0;}
int chl_planning(int t,int c,int a,int b,int d,int e,int y){return add(chlps,&st.n_chlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int chl_execution(int t,int c,int a,int b,int d,int e,int y){return add(chlss,&st.n_chle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int chl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(chlvss,&st.n_chl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int chl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(chlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int chl_market(int t,int c,int a,int b,int d,int e,int y){return add(chlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void chl_report(void){ps("[CHL] Chlpp: ");pi(st.n_chlp);ps(" PCS=");pi(st.t_f1);ps("\nChlpe: ");pi(st.n_chle);ps(" PCS=");pi(st.t_f2);ps("\nChlv: ");pi(st.n_chl2);ps(" PCS=");pi(st.t_f3);ps("\nChlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void chl_state(void){ps("[CHL] Chlpp=");pi(st.n_chlp);ps(" Chlpe=");pi(st.n_chle);ps(" Chlv=");pi(st.n_chl2);ps(" Chlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chloranthus Admin Demo ===\n\n");chl_init();
ps("Chloranthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;chl_planning(t,c,1234+(i*17),1223+(i*14),1203+(i*10),1185+(i*6),2020+(i%5));}
ps("\nChloranthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;chl_execution(t,c,1223+(i*15),1212+(i*12),1194+(i*8),1181+(i*5),2021+(i%4));}
ps("\nChloranthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;chl_evaluation(t,c,1215+(i*13),1204+(i*10),1188+(i*7),1177+(i*4),2022+(i%3));}
ps("\nChloranthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chl_accessory(t,c,1207+(i*11),1198+(i*9),1184+(i*6),1174+(i*3),2023+(i%2));}
ps("\nChloranthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chl_market(t,c,1201+(i*9),1192+(i*7),1179+(i*5),1171+(i*3),2024);}
ps("\n");chl_report();chl_state();ps("\n=== Demo Complete ===\n");return 0;}
