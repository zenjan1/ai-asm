/* urtica_admin: Urtica management technology administration (v1.0)
 * Urtica planning, urtica execution, urtica evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ub_t;
typedef struct{int n_ubp,n_ube,n_ubv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ub_state_t;
static ub_t ubps[N],ubss[N-2],ubvss[N-4],ubas[N-6],ubmks[N-6]; static ub_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ub_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ub_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[UBN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ubn_init(void){if(init)return -1;st.n_ubp=0;st.n_ube=0;st.n_ubv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ubps[i].active=0;for(int i=0;i<N-2;i++)ubss[i].active=0;for(int i=0;i<N-4;i++)ubvss[i].active=0;for(int i=0;i<N-6;i++)ubas[i].active=0;for(int i=0;i<N-6;i++)ubmks[i].active=0;init=1;ps("[UBN] Urtica initialized\n");return 0;}
int ubn_planning(int t,int c,int a,int b,int d,int e,int y){return add(ubps,&st.n_ubp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ubn_execution(int t,int c,int a,int b,int d,int e,int y){return add(ubss,&st.n_ube,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ubn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ubvss,&st.n_ubv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ubn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ubas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ubn_market(int t,int c,int a,int b,int d,int e,int y){return add(ubmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ubn_report(void){ps("[UBN] Ubp: ");pi(st.n_ubp);ps(" PCS=");pi(st.t_f1);ps("\nUbe: ");pi(st.n_ube);ps(" PCS=");pi(st.t_f2);ps("\nUbv: ");pi(st.n_ubv);ps(" PCS=");pi(st.t_f3);ps("\nUbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ubn_state(void){ps("[UBN] Ubp=");pi(st.n_ubp);ps(" Ube=");pi(st.n_ube);ps(" Ubv=");pi(st.n_ubv);ps(" Ubc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Urtica Admin Demo ===\n\n");ubn_init();
ps("Urtica planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ubn_planning(t,c,751+(i*17),740+(i*14),720+(i*10),702+(i*6),2020+(i%5));}
ps("\nUrtica execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ubn_execution(t,c,740+(i*15),729+(i*12),711+(i*8),698+(i*5),2021+(i%4));}
ps("\nUrtica evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ubn_evaluation(t,c,732+(i*13),721+(i*10),705+(i*7),694+(i*4),2022+(i%3));}
ps("\nUrtica accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ubn_accessory(t,c,724+(i*11),715+(i*9),701+(i*6),691+(i*3),2023+(i%2));}
ps("\nUrtica marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ubn_market(t,c,718+(i*9),709+(i*7),696+(i*5),688+(i*3),2024);}
ps("\n");ubn_report();ubn_state();ps("\n=== Demo Complete ===\n");return 0;}
