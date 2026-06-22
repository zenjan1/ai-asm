/* silphium_admin: Silphium management technology administration (v1.0)
 * Silphium planning, silphium execution, silphium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} silp_t;
typedef struct{int n_silpp,n_silpe,n_silp2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} silp_state_t;
static silp_t silps[N],silpess[N-2],silpvss[N-4],silpas[N-6],silpmks[N-6]; static silp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(silp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;silp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SILP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int silp_init(void){if(init)return -1;st.n_silpp=0;st.n_silpe=0;st.n_silp2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)silps[i].active=0;for(int i=0;i<N-2;i++)silpess[i].active=0;for(int i=0;i<N-4;i++)silpvss[i].active=0;for(int i=0;i<N-6;i++)silpas[i].active=0;for(int i=0;i<N-6;i++)silpmks[i].active=0;init=1;ps("[SILP] Silphium initialized\n");return 0;}
int silp_planning(int t,int c,int a,int b,int d,int e,int y){return add(silps,&st.n_silpp,&st.t_f1,N,t,c,a,b,d,e,y);}
int silp_execution(int t,int c,int a,int b,int d,int e,int y){return add(silpess,&st.n_silpe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int silp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(silpvss,&st.n_silp2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int silp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(silpas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int silp_market(int t,int c,int a,int b,int d,int e,int y){return add(silpmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void silp_report(void){ps("[SILP] Silpp: ");pi(st.n_silpp);ps(" PCS=");pi(st.t_f1);ps("\nSilpe: ");pi(st.n_silpe);ps(" PCS=");pi(st.t_f2);ps("\nSilpv: ");pi(st.n_silp2);ps(" PCS=");pi(st.t_f3);ps("\nSilpc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void silp_state(void){ps("[SILP] Silpp=");pi(st.n_silpp);ps(" Silpe=");pi(st.n_silpe);ps(" Silpv=");pi(st.n_silp2);ps(" Silpc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Silphium Admin Demo ===\n\n");silp_init();
ps("Silphium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;silp_planning(t,c,1337+(i*17),1326+(i*14),1306+(i*10),1288+(i*6),2020+(i%5));}
ps("\nSilphium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;silp_execution(t,c,1326+(i*15),1315+(i*12),1297+(i*8),1284+(i*5),2021+(i%4));}
ps("\nSilphium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;silp_evaluation(t,c,1318+(i*13),1307+(i*10),1291+(i*7),1280+(i*4),2022+(i%3));}
ps("\nSilphium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;silp_accessory(t,c,1310+(i*11),1301+(i*9),1287+(i*6),1277+(i*3),2023+(i%2));}
ps("\nSilphium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;silp_market(t,c,1304+(i*9),1295+(i*7),1282+(i*5),1274+(i*3),2024);}
ps("\n");silp_report();silp_state();ps("\n=== Demo Complete ===\n");return 0;}
