/* lecanopteris_admin: Lecanopteris management technology administration (v1.0)
 * Lecanopteris planning, lecanopteris execution, lecanopteris evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lecb_t;
typedef struct{int n_lecbop,n_lecb,n_lecb2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lecb_state_t;
static lecb_t lecbos[N],lecbe[N-2],lecb2[N-4],lecbac[N-6],lecbam[N-6]; static lecb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(lecb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lecb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LECB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lecb_init(void){if(init)return -1;st.n_lecbop=0;st.n_lecb=0;st.n_lecb2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lecbos[i].active=0;for(int i=0;i<N-2;i++)lecbe[i].active=0;for(int i=0;i<N-4;i++)lecb2[i].active=0;for(int i=0;i<N-6;i++)lecbac[i].active=0;for(int i=0;i<N-6;i++)lecbam[i].active=0;init=1;ps("[LECB] Lecanopteris initialized\n");return 0;}
int lecb_planning(int t,int c,int a,int b,int d,int e,int y){return add(lecbos,&st.n_lecbop,&st.t_f1,N,t,c,a,b,d,e,y);}
int lecb_execution(int t,int c,int a,int b,int d,int e,int y){return add(lecbe,&st.n_lecb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lecb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lecb2,&st.n_lecb2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lecb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lecbac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lecb_market(int t,int c,int a,int b,int d,int e,int y){return add(lecbam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lecb_report(void){ps("[LECB] Lecbp: ");pi(st.n_lecbop);ps(" PCS=");pi(st.t_f1);ps("\nLecbe: ");pi(st.n_lecb);ps(" PCS=");pi(st.t_f2);ps("\nLecb2: ");pi(st.n_lecb2);ps(" PCS=");pi(st.t_f3);ps("\nLcb: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lecb_state(void){ps("[LECB] Lecbp=");pi(st.n_lecbop);ps(" Lecb=");pi(st.n_lecb);ps(" Lcb2=");pi(st.n_lecb2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lecanopteris Admin Demo ===\n\n");lecb_init();
ps("Lecanopteris planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lecb_planning(t,c,1552-(i*17),1541-(i*14),1521-(i*10),1503-(i*6),2020+(i%5));}
ps("\nLecanopteris execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lecb_execution(t,c,1541-(i*15),1530-(i*12),1512-(i*8),1499-(i*5),2021+(i%4));}
ps("\nLecanopteris evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lecb_evaluation(t,c,1533-(i*13),1522-(i*10),1506-(i*7),1495-(i*4),2022+(i%3));}
ps("\nLecanopteris accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lecb_accessory(t,c,1525-(i*11),1516-(i*9),1502-(i*6),1492-(i*3),2023+(i%2));}
ps("\nLecanopteris marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lecb_market(t,c,1519-(i*9),1510-(i*7),1497-(i*5),1489-(i*3),2024);}
ps("\n");lecb_report();lecb_state();ps("\n=== Demo Complete ===\n");return 0;}
