/* viminaria_admin: Viminaria management technology administration (v1.0)
 * Viminaria planning, viminaria execution, viminaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vimi_t;
typedef struct{int n_vimip,n_vimi,n_vimi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vimi_state_t;
static vimi_t vimis[N],vimiess[N-2],vimivss[N-4],vimias[N-6],vimimks[N-6]; static vimi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(vimi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vimi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VIMI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vimi_init(void){if(init)return -1;st.n_vimip=0;st.n_vimi=0;st.n_vimi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vimis[i].active=0;for(int i=0;i<N-2;i++)vimiess[i].active=0;for(int i=0;i<N-4;i++)vimivss[i].active=0;for(int i=0;i<N-6;i++)vimias[i].active=0;for(int i=0;i<N-6;i++)vimimks[i].active=0;init=1;ps("[VIMI] Viminaria initialized\n");return 0;}
int vimi_planning(int t,int c,int a,int b,int d,int e,int y){return add(vimis,&st.n_vimip,&st.t_f1,N,t,c,a,b,d,e,y);}
int vimi_execution(int t,int c,int a,int b,int d,int e,int y){return add(vimiess,&st.n_vimi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vimi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vimivss,&st.n_vimi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vimi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vimias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vimi_market(int t,int c,int a,int b,int d,int e,int y){return add(vimimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vimi_report(void){ps("[VIMI] Vimip: ");pi(st.n_vimip);ps(" PCS=");pi(st.t_f1);ps("\nVimi: ");pi(st.n_vimi);ps(" PCS=");pi(st.t_f2);ps("\nVimv: ");pi(st.n_vimi2);ps(" PCS=");pi(st.t_f3);ps("\nVimc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vimi_state(void){ps("[VIMI] Vimip=");pi(st.n_vimip);ps(" Vimi=");pi(st.n_vimi);ps(" Vimv=");pi(st.n_vimi2);ps(" Vimc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Viminaria Admin Demo ===\n\n");vimi_init();
ps("Viminaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vimi_planning(t,c,1361+(i*17),1350+(i*14),1330+(i*10),1312+(i*6),2020+(i%5));}
ps("\nViminaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vimi_execution(t,c,1350+(i*15),1339+(i*12),1321+(i*8),1308+(i*5),2021+(i%4));}
ps("\nViminaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vimi_evaluation(t,c,1342+(i*13),1331+(i*10),1315+(i*7),1304+(i*4),2022+(i%3));}
ps("\nViminaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vimi_accessory(t,c,1334+(i*11),1325+(i*9),1311+(i*6),1301+(i*3),2023+(i%2));}
ps("\nViminaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vimi_market(t,c,1328+(i*9),1319+(i*7),1306+(i*5),1298+(i*3),2024);}
ps("\n");vimi_report();vimi_state();ps("\n=== Demo Complete ===\n");return 0;}
