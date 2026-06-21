/* isoetes_admin: Isoetes management technology administration (v1.0)
 * Isoetes planning, isoetes execution, isoetes evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} iso_t;
typedef struct{int n_isop,n_isoe,n_iso2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} iso_state_t;
static iso_t isops[N],isoss[N-2],isovss[N-4],isoas[N-6],isomks[N-6]; static iso_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(iso_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;iso_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ISO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int iso_init(void){if(init)return -1;st.n_isop=0;st.n_isoe=0;st.n_iso2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)isops[i].active=0;for(int i=0;i<N-2;i++)isoss[i].active=0;for(int i=0;i<N-4;i++)isovss[i].active=0;for(int i=0;i<N-6;i++)isoas[i].active=0;for(int i=0;i<N-6;i++)isomks[i].active=0;init=1;ps("[ISO] Isoetes initialized\n");return 0;}
int iso_planning(int t,int c,int a,int b,int d,int e,int y){return add(isops,&st.n_isop,&st.t_f1,N,t,c,a,b,d,e,y);}
int iso_execution(int t,int c,int a,int b,int d,int e,int y){return add(isoss,&st.n_isoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int iso_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(isovss,&st.n_iso2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int iso_accessory(int t,int c,int a,int b,int d,int e,int y){return add(isoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int iso_market(int t,int c,int a,int b,int d,int e,int y){return add(isomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void iso_report(void){ps("[ISO] Isop: ");pi(st.n_isop);ps(" PCS=");pi(st.t_f1);ps("\nIsoe: ");pi(st.n_isoe);ps(" PCS=");pi(st.t_f2);ps("\nIsov: ");pi(st.n_iso2);ps(" PCS=");pi(st.t_f3);ps("\nIsoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void iso_state(void){ps("[ISO] Isop=");pi(st.n_isop);ps(" Isoe=");pi(st.n_isoe);ps(" Isov=");pi(st.n_iso2);ps(" Isoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Isoetes Admin Demo ===\n\n");iso_init();
ps("Isoetes planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;iso_planning(t,c,1026+(i*17),1015+(i*14),995+(i*10),977+(i*6),2020+(i%5));}
ps("\nIsoetes execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;iso_execution(t,c,1015+(i*15),1004+(i*12),986+(i*8),973+(i*5),2021+(i%4));}
ps("\nIsoetes evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;iso_evaluation(t,c,1007+(i*13),996+(i*10),980+(i*7),969+(i*4),2022+(i%3));}
ps("\nIsoetes accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;iso_accessory(t,c,999+(i*11),990+(i*9),976+(i*6),966+(i*3),2023+(i%2));}
ps("\nIsoetes marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;iso_market(t,c,993+(i*9),984+(i*7),971+(i*5),963+(i*3),2024);}
ps("\n");iso_report();iso_state();ps("\n=== Demo Complete ===\n");return 0;}
