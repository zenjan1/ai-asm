/* kundmannia_admin: Kundmannia management technology administration (v1.0)
 * Kundmannia planning, kundmannia execution, kundmannia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kun_t;
typedef struct{int n_kunp,n_kune,n_kun2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kun_state_t;
static kun_t kunps[N],kuness[N-2],kunvss[N-4],kunas[N-6],kunmks[N-6]; static kun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(kun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kun_init(void){if(init)return -1;st.n_kunp=0;st.n_kune=0;st.n_kun2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kunps[i].active=0;for(int i=0;i<N-2;i++)kuness[i].active=0;for(int i=0;i<N-4;i++)kunvss[i].active=0;for(int i=0;i<N-6;i++)kunas[i].active=0;for(int i=0;i<N-6;i++)kunmks[i].active=0;init=1;ps("[KUN] Kundmannia initialized\n");return 0;}
int kun_planning(int t,int c,int a,int b,int d,int e,int y){return add(kunps,&st.n_kunp,&st.t_f1,N,t,c,a,b,d,e,y);}
int kun_execution(int t,int c,int a,int b,int d,int e,int y){return add(kuness,&st.n_kune,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kunvss,&st.n_kun2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kunas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kun_market(int t,int c,int a,int b,int d,int e,int y){return add(kunmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kun_report(void){ps("[KUN] Kunp: ");pi(st.n_kunp);ps(" PCS=");pi(st.t_f1);ps("\nKune: ");pi(st.n_kune);ps(" PCS=");pi(st.t_f2);ps("\nKunv: ");pi(st.n_kun2);ps(" PCS=");pi(st.t_f3);ps("\nKunc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kun_state(void){ps("[KUN] Kunp=");pi(st.n_kunp);ps(" Kune=");pi(st.n_kune);ps(" Kunv=");pi(st.n_kun2);ps(" Kunc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kundmannia Admin Demo ===\n\n");kun_init();
ps("Kundmannia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kun_planning(t,c,1307+(i*17),1296+(i*14),1276+(i*10),1258+(i*6),2020+(i%5));}
ps("\nKundmannia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kun_execution(t,c,1296+(i*15),1285+(i*12),1267+(i*8),1254+(i*5),2021+(i%4));}
ps("\nKundmannia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kun_evaluation(t,c,1288+(i*13),1277+(i*10),1261+(i*7),1250+(i*4),2022+(i%3));}
ps("\nKundmannia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kun_accessory(t,c,1280+(i*11),1271+(i*9),1257+(i*6),1247+(i*3),2023+(i%2));}
ps("\nKundmannia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kun_market(t,c,1274+(i*9),1265+(i*7),1252+(i*5),1244+(i*3),2024);}
ps("\n");kun_report();kun_state();ps("\n=== Demo Complete ===\n");return 0;}
