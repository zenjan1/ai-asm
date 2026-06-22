/* gibbaeum_admin: Gibbaeum management technology administration (v1.0)
 * Gibbaeum planning, gibbaeum execution, gibbaeum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gib_t;
typedef struct{int n_gibp,n_gibe,n_gib2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gib_state_t;
static gib_t gibps[N],gibss[N-2],gibvss[N-4],gibas[N-6],gibmks[N-6]; static gib_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gib_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gib_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GIB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gib_init(void){if(init)return -1;st.n_gibp=0;st.n_gibe=0;st.n_gib2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gibps[i].active=0;for(int i=0;i<N-2;i++)gibss[i].active=0;for(int i=0;i<N-4;i++)gibvss[i].active=0;for(int i=0;i<N-6;i++)gibas[i].active=0;for(int i=0;i<N-6;i++)gibmks[i].active=0;init=1;ps("[GIB] Gibbaeum initialized\n");return 0;}
int gib_planning(int t,int c,int a,int b,int d,int e,int y){return add(gibps,&st.n_gibp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gib_execution(int t,int c,int a,int b,int d,int e,int y){return add(gibss,&st.n_gibe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gib_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gibvss,&st.n_gib2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gib_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gibas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gib_market(int t,int c,int a,int b,int d,int e,int y){return add(gibmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gib_report(void){ps("[GIB] Gibp: ");pi(st.n_gibp);ps(" PCS=");pi(st.t_f1);ps("\nGibe: ");pi(st.n_gibe);ps(" PCS=");pi(st.t_f2);ps("\nGibv: ");pi(st.n_gib2);ps(" PCS=");pi(st.t_f3);ps("\nGibc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gib_state(void){ps("[GIB] Gibp=");pi(st.n_gibp);ps(" Gibe=");pi(st.n_gibe);ps(" Gibv=");pi(st.n_gib2);ps(" Gibc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gibbaeum Admin Demo ===\n\n");gib_init();
ps("Gibbaeum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gib_planning(t,c,1286+(i*17),1275+(i*14),1255+(i*10),1237+(i*6),2020+(i%5));}
ps("\nGibbaeum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gib_execution(t,c,1275+(i*15),1264+(i*12),1246+(i*8),1233+(i*5),2021+(i%4));}
ps("\nGibbaeum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gib_evaluation(t,c,1267+(i*13),1256+(i*10),1240+(i*7),1229+(i*4),2022+(i%3));}
ps("\nGibbaeum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gib_accessory(t,c,1259+(i*11),1250+(i*9),1236+(i*6),1226+(i*3),2023+(i%2));}
ps("\nGibbaeum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gib_market(t,c,1253+(i*9),1244+(i*7),1231+(i*5),1223+(i*3),2024);}
ps("\n");gib_report();gib_state();ps("\n=== Demo Complete ===\n");return 0;}
