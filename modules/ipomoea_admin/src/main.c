/* ipomoea_admin: Ipomoea management technology administration (v1.0)
 * Ipomoea planning, ipomoea execution, ipomoea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ipo_t;
typedef struct{int n_ipop,n_ipoe,n_ipo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ipo_state_t;
static ipo_t ipops[N],iposs[N-2],ipovss[N-4],ipoas[N-6],ipomks[N-6]; static ipo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ipo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ipo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IPO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ipo_init(void){if(init)return -1;st.n_ipop=0;st.n_ipoe=0;st.n_ipo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ipops[i].active=0;for(int i=0;i<N-2;i++)iposs[i].active=0;for(int i=0;i<N-4;i++)ipovss[i].active=0;for(int i=0;i<N-6;i++)ipoas[i].active=0;for(int i=0;i<N-6;i++)ipomks[i].active=0;init=1;ps("[IPO] Ipomoea initialized\n");return 0;}
int ipo_planning(int t,int c,int a,int b,int d,int e,int y){return add(ipops,&st.n_ipop,&st.t_f1,N,t,c,a,b,d,e,y);}
int ipo_execution(int t,int c,int a,int b,int d,int e,int y){return add(iposs,&st.n_ipoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ipo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ipovss,&st.n_ipo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ipo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ipoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ipo_market(int t,int c,int a,int b,int d,int e,int y){return add(ipomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ipo_report(void){ps("[IPO] Ipop: ");pi(st.n_ipop);ps(" PCS=");pi(st.t_f1);ps("\nIpoe: ");pi(st.n_ipoe);ps(" PCS=");pi(st.t_f2);ps("\nIpov: ");pi(st.n_ipo2);ps(" PCS=");pi(st.t_f3);ps("\nIpoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ipo_state(void){ps("[IPO] Ipop=");pi(st.n_ipop);ps(" Ipoe=");pi(st.n_ipoe);ps(" Ipov=");pi(st.n_ipo2);ps(" Ipoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ipomoea Admin Demo ===\n\n");ipo_init();
ps("Ipomoea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ipo_planning(t,c,1025+(i*17),1014+(i*14),994+(i*10),976+(i*6),2020+(i%5));}
ps("\nIpomoea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ipo_execution(t,c,1014+(i*15),1003+(i*12),985+(i*8),972+(i*5),2021+(i%4));}
ps("\nIpomoea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ipo_evaluation(t,c,1006+(i*13),995+(i*10),979+(i*7),968+(i*4),2022+(i%3));}
ps("\nIpomoea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ipo_accessory(t,c,998+(i*11),989+(i*9),975+(i*6),965+(i*3),2023+(i%2));}
ps("\nIpomoea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ipo_market(t,c,992+(i*9),983+(i*7),970+(i*5),962+(i*3),2024);}
ps("\n");ipo_report();ipo_state();ps("\n=== Demo Complete ===\n");return 0;}
