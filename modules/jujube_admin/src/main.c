/* jujube_admin: Jujube management technology administration (v1.0)
 * Jujube planning, jujube execution, jujube evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} juj_t;
typedef struct{int n_jujp,n_juje,n_juj2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} juj_state_t;
static juj_t jujps[N],jujess[N-2],jujvss[N-4],jujas[N-6],jujmks[N-6]; static juj_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(juj_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;juj_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JUJ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int juj_init(void){if(init)return -1;st.n_jujp=0;st.n_juje=0;st.n_juj2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jujps[i].active=0;for(int i=0;i<N-2;i++)jujess[i].active=0;for(int i=0;i<N-4;i++)jujvss[i].active=0;for(int i=0;i<N-6;i++)jujas[i].active=0;for(int i=0;i<N-6;i++)jujmks[i].active=0;init=1;ps("[JUJ] Jujube initialized\n");return 0;}
int juj_planning(int t,int c,int a,int b,int d,int e,int y){return add(jujps,&st.n_jujp,&st.t_f1,N,t,c,a,b,d,e,y);}
int juj_execution(int t,int c,int a,int b,int d,int e,int y){return add(jujess,&st.n_juje,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int juj_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jujvss,&st.n_juj2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int juj_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jujas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int juj_market(int t,int c,int a,int b,int d,int e,int y){return add(jujmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void juj_report(void){ps("[JUJ] Jujp: ");pi(st.n_jujp);ps(" PCS=");pi(st.t_f1);ps("\nJuje: ");pi(st.n_juje);ps(" PCS=");pi(st.t_f2);ps("\nJujv: ");pi(st.n_juj2);ps(" PCS=");pi(st.t_f3);ps("\nJujc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void juj_state(void){ps("[JUJ] Jujp=");pi(st.n_jujp);ps(" Juje=");pi(st.n_juje);ps(" Jujv=");pi(st.n_juj2);ps(" Jujc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jujube Admin Demo ===\n\n");juj_init();
ps("Jujube planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;juj_planning(t,c,1305+(i*17),1294+(i*14),1274+(i*10),1256+(i*6),2020+(i%5));}
ps("\nJujube execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;juj_execution(t,c,1294+(i*15),1283+(i*12),1265+(i*8),1252+(i*5),2021+(i%4));}
ps("\nJujube evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;juj_evaluation(t,c,1286+(i*13),1275+(i*10),1259+(i*7),1248+(i*4),2022+(i%3));}
ps("\nJujube accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;juj_accessory(t,c,1278+(i*11),1269+(i*9),1255+(i*6),1245+(i*3),2023+(i%2));}
ps("\nJujube marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;juj_market(t,c,1272+(i*9),1263+(i*7),1250+(i*5),1242+(i*3),2024);}
ps("\n");juj_report();juj_state();ps("\n=== Demo Complete ===\n");return 0;}
