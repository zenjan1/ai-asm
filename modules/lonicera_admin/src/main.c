/* lonicera_admin: Lonicera management technology administration (v1.0)
 * Lonicera planning, lonicera execution, lonicera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lon_t;
typedef struct{int n_lonp,n_lone,n_lon2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lon_state_t;
static lon_t lonps[N],lonss[N-2],lonvss[N-4],lonac[N-6],lonmks[N-6]; static lon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lon_init(void){if(init)return -1;st.n_lonp=0;st.n_lone=0;st.n_lon2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lonps[i].active=0;for(int i=0;i<N-2;i++)lonss[i].active=0;for(int i=0;i<N-4;i++)lonvss[i].active=0;for(int i=0;i<N-6;i++)lonac[i].active=0;for(int i=0;i<N-6;i++)lonmks[i].active=0;init=1;ps("[LON] Lonicera initialized\n");return 0;}
int lon_planning(int t,int c,int a,int b,int d,int e,int y){return add(lonps,&st.n_lonp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lon_execution(int t,int c,int a,int b,int d,int e,int y){return add(lonss,&st.n_lone,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lon_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lonvss,&st.n_lon2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lon_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lonac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lon_market(int t,int c,int a,int b,int d,int e,int y){return add(lonmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lon_report(void){ps("[LON] Lonp: ");pi(st.n_lonp);ps(" PCS=");pi(st.t_f1);ps("\nLone: ");pi(st.n_lone);ps(" PCS=");pi(st.t_f2);ps("\nLonv: ");pi(st.n_lon2);ps(" PCS=");pi(st.t_f3);ps("\nLonc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lon_state(void){ps("[LON] Lonp=");pi(st.n_lonp);ps(" Lone=");pi(st.n_lone);ps(" Lonv=");pi(st.n_lon2);ps(" Lonc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lonicera Admin Demo ===\n\n");lon_init();
ps("Lonicera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lon_planning(t,c,937+(i*17),926+(i*14),906+(i*10),888+(i*6),2020+(i%5));}
ps("\nLonicera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lon_execution(t,c,926+(i*15),915+(i*12),897+(i*8),884+(i*5),2021+(i%4));}
ps("\nLonicera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lon_evaluation(t,c,918+(i*13),907+(i*10),891+(i*7),880+(i*4),2022+(i%3));}
ps("\nLonicera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lon_accessory(t,c,910+(i*11),901+(i*9),887+(i*6),877+(i*3),2023+(i%2));}
ps("\nLonicera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lon_market(t,c,904+(i*9),895+(i*7),882+(i*5),874+(i*3),2024);}
ps("\n");lon_report();lon_state();ps("\n=== Demo Complete ===\n");return 0;}
