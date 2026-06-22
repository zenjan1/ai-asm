/* hunnemannia_admin: Hunnemannia management technology administration (v1.0)
 * Hunnemannia planning, hunnemannia execution, hunnemannia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hun_t;
typedef struct{int n_hunp,n_hune,n_hun2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hun_state_t;
static hun_t hunps[N],hunss[N-2],hunvss[N-4],hunas[N-6],hunmks[N-6]; static hun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hun_init(void){if(init)return -1;st.n_hunp=0;st.n_hune=0;st.n_hun2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hunps[i].active=0;for(int i=0;i<N-2;i++)hunss[i].active=0;for(int i=0;i<N-4;i++)hunvss[i].active=0;for(int i=0;i<N-6;i++)hunas[i].active=0;for(int i=0;i<N-6;i++)hunmks[i].active=0;init=1;ps("[HUN] Hunnemannia initialized\n");return 0;}
int hun_planning(int t,int c,int a,int b,int d,int e,int y){return add(hunps,&st.n_hunp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hun_execution(int t,int c,int a,int b,int d,int e,int y){return add(hunss,&st.n_hune,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hunvss,&st.n_hun2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hunas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hun_market(int t,int c,int a,int b,int d,int e,int y){return add(hunmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hun_report(void){ps("[HUN] Hunp: ");pi(st.n_hunp);ps(" PCS=");pi(st.t_f1);ps("\nHune: ");pi(st.n_hune);ps(" PCS=");pi(st.t_f2);ps("\nHunv: ");pi(st.n_hun2);ps(" PCS=");pi(st.t_f3);ps("\nHunc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hun_state(void){ps("[HUN] Hunp=");pi(st.n_hunp);ps(" Hune=");pi(st.n_hune);ps(" Hunv=");pi(st.n_hun2);ps(" Hunc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hunnemannia Admin Demo ===\n\n");hun_init();
ps("Hunnemannia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hun_planning(t,c,1295+(i*17),1284+(i*14),1264+(i*10),1246+(i*6),2020+(i%5));}
ps("\nHunnemannia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hun_execution(t,c,1284+(i*15),1273+(i*12),1255+(i*8),1242+(i*5),2021+(i%4));}
ps("\nHunnemannia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hun_evaluation(t,c,1276+(i*13),1265+(i*10),1249+(i*7),1238+(i*4),2022+(i%3));}
ps("\nHunnemannia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hun_accessory(t,c,1268+(i*11),1259+(i*9),1245+(i*6),1235+(i*3),2023+(i%2));}
ps("\nHunnemannia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hun_market(t,c,1262+(i*9),1253+(i*7),1240+(i*5),1232+(i*3),2024);}
ps("\n");hun_report();hun_state();ps("\n=== Demo Complete ===\n");return 0;}
