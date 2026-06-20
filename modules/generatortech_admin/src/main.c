/* generatortech_admin: Generator technology administration (v1.0)
 * Diesel generators, gas generators, wind generators, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gnr_t;
typedef struct{int n_di,n_ga,n_wi,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gnr_state_t;
static gnr_t dis[N],gas[N-2],wis[N-4],acs[N-6],mks[N-6]; static gnr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gnr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gnr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GNR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gnr_init(void){if(init)return -1;st.n_di=0;st.n_ga=0;st.n_wi=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dis[i].active=0;for(int i=0;i<N-2;i++)gas[i].active=0;for(int i=0;i<N-4;i++)wis[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GNR] Generatortech initialized\n");return 0;}
int gnr_diesel(int t,int c,int a,int b,int d,int e,int y){return add(dis,&st.n_di,&st.t_f1,N,t,c,a,b,d,e,y);}
int gnr_gas(int t,int c,int a,int b,int d,int e,int y){return add(gas,&st.n_ga,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gnr_wind(int t,int c,int a,int b,int d,int e,int y){return add(wis,&st.n_wi,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gnr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gnr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gnr_report(void){ps("[GNR] Di: ");pi(st.n_di);ps(" PCS=");pi(st.t_f1);ps("\nGa: ");pi(st.n_ga);ps(" PCS=");pi(st.t_f2);ps("\nWi: ");pi(st.n_wi);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gnr_state(void){ps("[GNR] Di=");pi(st.n_di);ps(" Ga=");pi(st.n_ga);ps(" Wi=");pi(st.n_wi);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Generator Tech Admin Demo ===\n\n");gnr_init();
ps("Diesel generators...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gnr_diesel(t,c,259+(i*17),244+(i*14),224+(i*10),206+(i*6),2020+(i%5));}
ps("\nGas generators...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gnr_gas(t,c,248+(i*15),234+(i*12),216+(i*8),203+(i*5),2021+(i%4));}
ps("\nWind generators...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gnr_wind(t,c,240+(i*13),226+(i*10),210+(i*7),199+(i*4),2022+(i%3));}
ps("\nGenerator accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gnr_accessory(t,c,232+(i*11),220+(i*9),206+(i*6),196+(i*3),2023+(i%2));}
ps("\nGenerator marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gnr_market(t,c,226+(i*9),215+(i*7),202+(i*5),194+(i*3),2024);}
ps("\n");gnr_report();gnr_state();ps("\n=== Demo Complete ===\n");return 0;}
