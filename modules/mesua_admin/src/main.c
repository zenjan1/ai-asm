/* mesua_admin: Mesua management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mesu_t;
typedef struct{int n_mesuop,n_mesu,n_mesu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mesu_state_t;
static mesu_t mesos[N],mesue[N-2],mesu2[N-4],mesuac[N-6],mesuam[N-6]; static mesu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mesu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mesu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MESU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mesu_init(void){if(init)return -1;st.n_mesuop=0;st.n_mesu=0;st.n_mesu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mesos[i].active=0;for(int i=0;i<N-2;i++)mesue[i].active=0;for(int i=0;i<N-4;i++)mesu2[i].active=0;for(int i=0;i<N-6;i++)mesuac[i].active=0;for(int i=0;i<N-6;i++)mesuam[i].active=0;init=1;ps("[MESU] Mesua initialized\n");return 0;}
int mesu_planning(int t,int c,int a,int b,int d,int e,int y){return add(mesos,&st.n_mesuop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mesu_execution(int t,int c,int a,int b,int d,int e,int y){return add(mesue,&st.n_mesu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mesu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mesu2,&st.n_mesu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mesu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mesuac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mesu_market(int t,int c,int a,int b,int d,int e,int y){return add(mesuam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mesu_report(void){ps("[MESU] Mesup: ");pi(st.n_mesuop);ps(" PCS=");pi(st.t_f1);ps("\nMesue: ");pi(st.n_mesu);ps(" PCS=");pi(st.t_f2);ps("\nMesu2: ");pi(st.n_mesu2);ps(" PCS=");pi(st.t_f3);ps("\nMsu: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mesu_state(void){ps("[MESU] Mesup=");pi(st.n_mesuop);ps(" Mesu=");pi(st.n_mesu);ps(" Su2=");pi(st.n_mesu2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mesua Admin Demo ===\n\n");mesu_init();
ps("Mesua planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mesu_planning(t,c,1572-(i*17),1561-(i*14),1541-(i*10),1523-(i*6),2020+(i%5));}
ps("\nMesua execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mesu_execution(t,c,1561-(i*15),1550-(i*12),1532-(i*8),1519-(i*5),2021+(i%4));}
ps("\nMesua evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mesu_evaluation(t,c,1553-(i*13),1542-(i*10),1526-(i*7),1515-(i*4),2022+(i%3));}
ps("\nMesua accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mesu_accessory(t,c,1545-(i*11),1536-(i*9),1522-(i*6),1512-(i*3),2023+(i%2));}
ps("\nMesua marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mesu_market(t,c,1539-(i*9),1530-(i*7),1517-(i*5),1509-(i*3),2024);}
ps("\n");mesu_report();mesu_state();ps("\n=== Demo Complete ===\n");return 0;}
