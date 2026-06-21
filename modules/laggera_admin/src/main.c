/* laggera_admin: Laggera management technology administration (v1.0)
 * Laggera planning, laggera execution, laggera evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lag_t;
typedef struct{int n_lagp,n_lage,n_lag2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lag_state_t;
static lag_t lagps[N],lagss[N-2],lagvss[N-4],lagas[N-6],lagmks[N-6]; static lag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lag_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lag_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lag_init(void){if(init)return -1;st.n_lagp=0;st.n_lage=0;st.n_lag2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lagps[i].active=0;for(int i=0;i<N-2;i++)lagss[i].active=0;for(int i=0;i<N-4;i++)lagvss[i].active=0;for(int i=0;i<N-6;i++)lagas[i].active=0;for(int i=0;i<N-6;i++)lagmks[i].active=0;init=1;ps("[LAG] Laggera initialized\n");return 0;}
int lag_planning(int t,int c,int a,int b,int d,int e,int y){return add(lagps,&st.n_lagp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lag_execution(int t,int c,int a,int b,int d,int e,int y){return add(lagss,&st.n_lage,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lag_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lagvss,&st.n_lag2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lag_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lagas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lag_market(int t,int c,int a,int b,int d,int e,int y){return add(lagmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lag_report(void){ps("[LAG] Lagp: ");pi(st.n_lagp);ps(" PCS=");pi(st.t_f1);ps("\nLage: ");pi(st.n_lage);ps(" PCS=");pi(st.t_f2);ps("\nLagv: ");pi(st.n_lag2);ps(" PCS=");pi(st.t_f3);ps("\nLagc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lag_state(void){ps("[LAG] Lagp=");pi(st.n_lagp);ps(" Lage=");pi(st.n_lage);ps(" Lagv=");pi(st.n_lag2);ps(" Lagc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Laggera Admin Demo ===\n\n");lag_init();
ps("Laggera planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lag_planning(t,c,1040+(i*17),1029+(i*14),1009+(i*10),991+(i*6),2020+(i%5));}
ps("\nLaggera execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lag_execution(t,c,1029+(i*15),1018+(i*12),1000+(i*8),987+(i*5),2021+(i%4));}
ps("\nLaggera evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lag_evaluation(t,c,1021+(i*13),1010+(i*10),994+(i*7),983+(i*4),2022+(i%3));}
ps("\nLaggera accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lag_accessory(t,c,1013+(i*11),1004+(i*9),990+(i*6),980+(i*3),2023+(i%2));}
ps("\nLaggera marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lag_market(t,c,1007+(i*9),998+(i*7),985+(i*5),977+(i*3),2024);}
ps("\n");lag_report();lag_state();ps("\n=== Demo Complete ===\n");return 0;}
