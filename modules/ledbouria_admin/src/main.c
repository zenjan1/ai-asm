/* ledbouria_admin: Ledbouria management technology administration (v1.0)
 * Ledbouria planning, ledbouria execution, ledbouria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ledb_t;
typedef struct{int n_ledbop,n_ledb,n_ledb2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ledb_state_t;
static ledb_t ledbos[N],ledbe[N-2],ledb2[N-4],ledbac[N-6],ledbam[N-6]; static ledb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ledb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ledb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEDB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ledb_init(void){if(init)return -1;st.n_ledbop=0;st.n_ledb=0;st.n_ledb2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ledbos[i].active=0;for(int i=0;i<N-2;i++)ledbe[i].active=0;for(int i=0;i<N-4;i++)ledb2[i].active=0;for(int i=0;i<N-6;i++)ledbac[i].active=0;for(int i=0;i<N-6;i++)ledbam[i].active=0;init=1;ps("[LEDB] Ledbouria initialized\n");return 0;}
int ledb_planning(int t,int c,int a,int b,int d,int e,int y){return add(ledbos,&st.n_ledbop,&st.t_f1,N,t,c,a,b,d,e,y);}
int ledb_execution(int t,int c,int a,int b,int d,int e,int y){return add(ledbe,&st.n_ledb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ledb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ledb2,&st.n_ledb2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ledb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ledbac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ledb_market(int t,int c,int a,int b,int d,int e,int y){return add(ledbam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ledb_report(void){ps("[LEDB] Ledbp: ");pi(st.n_ledbop);ps(" PCS=");pi(st.t_f1);ps("\nLedbe: ");pi(st.n_ledb);ps(" PCS=");pi(st.t_f2);ps("\nLedb2: ");pi(st.n_ledb2);ps(" PCS=");pi(st.t_f3);ps("\nLdb: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ledb_state(void){ps("[LEDB] Ledbp=");pi(st.n_ledbop);ps(" Ldb=");pi(st.n_ledb);ps(" Ldb2=");pi(st.n_ledb2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ledbouria Admin Demo ===\n\n");ledb_init();
ps("Ledbouria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ledb_planning(t,c,1553-(i*17),1542-(i*14),1522-(i*10),1504-(i*6),2020+(i%5));}
ps("\nLedbouria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ledb_execution(t,c,1542-(i*15),1531-(i*12),1513-(i*8),1500-(i*5),2021+(i%4));}
ps("\nLedbouria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ledb_evaluation(t,c,1534-(i*13),1523-(i*10),1507-(i*7),1496-(i*4),2022+(i%3));}
ps("\nLedbouria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ledb_accessory(t,c,1526-(i*11),1517-(i*9),1503-(i*6),1493-(i*3),2023+(i%2));}
ps("\nLedbouria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ledb_market(t,c,1520-(i*9),1511-(i*7),1498-(i*5),1490-(i*3),2024);}
ps("\n");ledb_report();ledb_state();ps("\n=== Demo Complete ===\n");return 0;}
