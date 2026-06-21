/* ginger_admin: Ginger management technology administration (v1.0)
 * Ginger planning, ginger execution, ginger evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gn_t;
typedef struct{int n_gnp,n_gne,n_gnv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gn_state_t;
static gn_t gnps[N],gnss[N-2],gnvss[N-4],gnas[N-6],gnmks[N-6]; static gn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GNG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gn_init(void){if(init)return -1;st.n_gnp=0;st.n_gne=0;st.n_gnv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gnps[i].active=0;for(int i=0;i<N-2;i++)gnss[i].active=0;for(int i=0;i<N-4;i++)gnvss[i].active=0;for(int i=0;i<N-6;i++)gnas[i].active=0;for(int i=0;i<N-6;i++)gnmks[i].active=0;init=1;ps("[GNG] Ginger initialized\n");return 0;}
int gn_planning(int t,int c,int a,int b,int d,int e,int y){return add(gnps,&st.n_gnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gn_execution(int t,int c,int a,int b,int d,int e,int y){return add(gnss,&st.n_gne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gnvss,&st.n_gnv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gn_market(int t,int c,int a,int b,int d,int e,int y){return add(gnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gn_report(void){ps("[GNG] Gnp: ");pi(st.n_gnp);ps(" PCS=");pi(st.t_f1);ps("\nGne: ");pi(st.n_gne);ps(" PCS=");pi(st.t_f2);ps("\nGnv: ");pi(st.n_gnv);ps(" PCS=");pi(st.t_f3);ps("\nGnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gn_state(void){ps("[GNG] Gnp=");pi(st.n_gnp);ps(" Gne=");pi(st.n_gne);ps(" Gnv=");pi(st.n_gnv);ps(" Gnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ginger Admin Demo ===\n\n");gn_init();
ps("Ginger planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gn_planning(t,c,669+(i*17),658+(i*14),638+(i*10),620+(i*6),2020+(i%5));}
ps("\nGinger execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gn_execution(t,c,658+(i*15),647+(i*12),629+(i*8),616+(i*5),2021+(i%4));}
ps("\nGinger evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gn_evaluation(t,c,650+(i*13),639+(i*10),623+(i*7),612+(i*4),2022+(i%3));}
ps("\nGinger accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gn_accessory(t,c,642+(i*11),633+(i*9),619+(i*6),609+(i*3),2023+(i%2));}
ps("\nGinger marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gn_market(t,c,636+(i*9),627+(i*7),614+(i*5),606+(i*3),2024);}
ps("\n");gn_report();gn_state();ps("\n=== Demo Complete ===\n");return 0;}
