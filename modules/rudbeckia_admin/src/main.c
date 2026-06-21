/* rudbeckia_admin: Rudbeckia management technology administration (v1.0)
 * Rudbeckia planning, rudbeckia execution, rudbeckia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rd_t;
typedef struct{int n_rdp,n_rde,n_rdv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rd_state_t;
static rd_t rdps[N],rdss[N-2],rdvss[N-4],rdas[N-6],rdmks[N-6]; static rd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RDB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rdb_init(void){if(init)return -1;st.n_rdp=0;st.n_rde=0;st.n_rdv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rdps[i].active=0;for(int i=0;i<N-2;i++)rdss[i].active=0;for(int i=0;i<N-4;i++)rdvss[i].active=0;for(int i=0;i<N-6;i++)rdas[i].active=0;for(int i=0;i<N-6;i++)rdmks[i].active=0;init=1;ps("[RDB] Rudbeckia initialized\n");return 0;}
int rdb_planning(int t,int c,int a,int b,int d,int e,int y){return add(rdps,&st.n_rdp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rdb_execution(int t,int c,int a,int b,int d,int e,int y){return add(rdss,&st.n_rde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rdb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rdvss,&st.n_rdv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rdb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rdas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rdb_market(int t,int c,int a,int b,int d,int e,int y){return add(rdmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rdb_report(void){ps("[RDB] Rdp: ");pi(st.n_rdp);ps(" PCS=");pi(st.t_f1);ps("\nRde: ");pi(st.n_rde);ps(" PCS=");pi(st.t_f2);ps("\nRdv: ");pi(st.n_rdv);ps(" PCS=");pi(st.t_f3);ps("\nRdc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rdb_state(void){ps("[RDB] Rdp=");pi(st.n_rdp);ps(" Rde=");pi(st.n_rde);ps(" Rdv=");pi(st.n_rdv);ps(" Rdc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rudbeckia Admin Demo ===\n\n");rdb_init();
ps("Rudbeckia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rdb_planning(t,c,738+(i*17),727+(i*14),707+(i*10),689+(i*6),2020+(i%5));}
ps("\nRudbeckia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rdb_execution(t,c,727+(i*15),716+(i*12),698+(i*8),685+(i*5),2021+(i%4));}
ps("\nRudbeckia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rdb_evaluation(t,c,719+(i*13),708+(i*10),692+(i*7),681+(i*4),2022+(i%3));}
ps("\nRudbeckia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rdb_accessory(t,c,711+(i*11),702+(i*9),688+(i*6),678+(i*3),2023+(i%2));}
ps("\nRudbeckia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rdb_market(t,c,705+(i*9),696+(i*7),683+(i*5),675+(i*3),2024);}
ps("\n");rdb_report();rdb_state();ps("\n=== Demo Complete ===\n");return 0;}
