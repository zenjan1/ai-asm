/* vernonia_admin: Vernonia management technology administration (v1.0)
 * Vernonia planning, vernonia execution, vernonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vern_t;
typedef struct{int n_vernp,n_verne,n_vern2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vern_state_t;
static vern_t verns[N],verness[N-2],vernvss[N-4],vernas[N-6],vernmks[N-6]; static vern_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(vern_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vern_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VERN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vern_init(void){if(init)return -1;st.n_vernp=0;st.n_verne=0;st.n_vern2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)verns[i].active=0;for(int i=0;i<N-2;i++)verness[i].active=0;for(int i=0;i<N-4;i++)vernvss[i].active=0;for(int i=0;i<N-6;i++)vernas[i].active=0;for(int i=0;i<N-6;i++)vernmks[i].active=0;init=1;ps("[VERN] Vernonia initialized\n");return 0;}
int vern_planning(int t,int c,int a,int b,int d,int e,int y){return add(verns,&st.n_vernp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vern_execution(int t,int c,int a,int b,int d,int e,int y){return add(verness,&st.n_verne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vern_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vernvss,&st.n_vern2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vern_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vernas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vern_market(int t,int c,int a,int b,int d,int e,int y){return add(vernmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vern_report(void){ps("[VERN] Verp: ");pi(st.n_vernp);ps(" PCS=");pi(st.t_f1);ps("\nVerne: ");pi(st.n_verne);ps(" PCS=");pi(st.t_f2);ps("\nVern2: ");pi(st.n_vern2);ps(" PCS=");pi(st.t_f3);ps("\nVernc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vern_state(void){ps("[VERN] Verp=");pi(st.n_vernp);ps(" Verne=");pi(st.n_verne);ps(" Vern2=");pi(st.n_vern2);ps(" Vernc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vernonia Admin Demo ===\n\n");vern_init();
ps("Vernonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vern_planning(t,c,1356+(i*17),1345+(i*14),1325+(i*10),1307+(i*6),2020+(i%5));}
ps("\nVernonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vern_execution(t,c,1345+(i*15),1334+(i*12),1316+(i*8),1303+(i*5),2021+(i%4));}
ps("\nVernonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vern_evaluation(t,c,1337+(i*13),1326+(i*10),1310+(i*7),1299+(i*4),2022+(i%3));}
ps("\nVernonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vern_accessory(t,c,1329+(i*11),1320+(i*9),1306+(i*6),1296+(i*3),2023+(i%2));}
ps("\nVernonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vern_market(t,c,1323+(i*9),1314+(i*7),1301+(i*5),1293+(i*3),2024);}
ps("\n");vern_report();vern_state();ps("\n=== Demo Complete ===\n");return 0;}
