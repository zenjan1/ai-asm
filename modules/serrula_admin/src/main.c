/* serrula_admin: Serrula management technology administration (v1.0)
 * Serrula planning, serrula execution, serrula evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} serr_t;
typedef struct{int n_serrp,n_serre,n_serr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} serr_state_t;
static serr_t serrs[N],serrss[N-2],serrvss[N-4],serras[N-6],serrmks[N-6]; static serr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(serr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;serr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SERR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int serr_init(void){if(init)return -1;st.n_serrp=0;st.n_serre=0;st.n_serr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)serrs[i].active=0;for(int i=0;i<N-2;i++)serrss[i].active=0;for(int i=0;i<N-4;i++)serrvss[i].active=0;for(int i=0;i<N-6;i++)serras[i].active=0;for(int i=0;i<N-6;i++)serrmks[i].active=0;init=1;ps("[SERR] Serrula initialized\n");return 0;}
int serr_planning(int t,int c,int a,int b,int d,int e,int y){return add(serrs,&st.n_serrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int serr_execution(int t,int c,int a,int b,int d,int e,int y){return add(serrss,&st.n_serre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int serr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(serrvss,&st.n_serr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int serr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(serras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int serr_market(int t,int c,int a,int b,int d,int e,int y){return add(serrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void serr_report(void){ps("[SERR] Serrp: ");pi(st.n_serrp);ps(" PCS=");pi(st.t_f1);ps("\nSerre: ");pi(st.n_serre);ps(" PCS=");pi(st.t_f2);ps("\nSerrv: ");pi(st.n_serr2);ps(" PCS=");pi(st.t_f3);ps("\nSerrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void serr_state(void){ps("[SERR] Serrp=");pi(st.n_serrp);ps(" Serre=");pi(st.n_serre);ps(" Serrv=");pi(st.n_serr2);ps(" Serrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Serrula Admin Demo ===\n\n");serr_init();
ps("Serrula planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;serr_planning(t,c,1335+(i*17),1324+(i*14),1304+(i*10),1286+(i*6),2020+(i%5));}
ps("\nSerrula execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;serr_execution(t,c,1324+(i*15),1313+(i*12),1295+(i*8),1282+(i*5),2021+(i%4));}
ps("\nSerrula evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;serr_evaluation(t,c,1316+(i*13),1305+(i*10),1289+(i*7),1278+(i*4),2022+(i%3));}
ps("\nSerrula accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;serr_accessory(t,c,1308+(i*11),1299+(i*9),1285+(i*6),1275+(i*3),2023+(i%2));}
ps("\nSerrula marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;serr_market(t,c,1302+(i*9),1293+(i*7),1280+(i*5),1272+(i*3),2024);}
ps("\n");serr_report();serr_state();ps("\n=== Demo Complete ===\n");return 0;}
