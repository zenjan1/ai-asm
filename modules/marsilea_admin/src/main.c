/* marsilea_admin: Marsilea management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mars_t;
typedef struct{int n_marop,n_mars,n_mars2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mars_state_t;
static mars_t maros[N],marse[N-2],mars2[N-4],marsac[N-6],marsam[N-6]; static mars_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mars_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mars_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MARS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mars_init(void){if(init)return -1;st.n_marop=0;st.n_mars=0;st.n_mars2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)maros[i].active=0;for(int i=0;i<N-2;i++)marse[i].active=0;for(int i=0;i<N-4;i++)mars2[i].active=0;for(int i=0;i<N-6;i++)marsac[i].active=0;for(int i=0;i<N-6;i++)marsam[i].active=0;init=1;ps("[MARS] Marsilea initialized\n");return 0;}
int mars_planning(int t,int c,int a,int b,int d,int e,int y){return add(maros,&st.n_marop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mars_execution(int t,int c,int a,int b,int d,int e,int y){return add(marse,&st.n_mars,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mars_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mars2,&st.n_mars2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mars_accessory(int t,int c,int a,int b,int d,int e,int y){return add(marsac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mars_market(int t,int c,int a,int b,int d,int e,int y){return add(marsam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mars_report(void){ps("[MARS] Marcp: ");pi(st.n_marop);ps(" PCS=");pi(st.t_f1);ps("\nMarse: ");pi(st.n_mars);ps(" PCS=");pi(st.t_f2);ps("\nMars2: ");pi(st.n_mars2);ps(" PCS=");pi(st.t_f3);ps("\nMrs: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mars_state(void){ps("[MARS] Marcp=");pi(st.n_marop);ps(" Mars=");pi(st.n_mars);ps(" Mrs2=");pi(st.n_mars2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Marsilea Admin Demo ===\n\n");mars_init();
ps("Marsilea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mars_planning(t,c,1565-(i*17),1554-(i*14),1534-(i*10),1516-(i*6),2020+(i%5));}
ps("\nMarsilea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mars_execution(t,c,1554-(i*15),1543-(i*12),1525-(i*8),1512-(i*5),2021+(i%4));}
ps("\nMarsilea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mars_evaluation(t,c,1546-(i*13),1535-(i*10),1519-(i*7),1508-(i*4),2022+(i%3));}
ps("\nMarsilea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mars_accessory(t,c,1538-(i*11),1529-(i*9),1515-(i*6),1505-(i*3),2023+(i%2));}
ps("\nMarsilea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mars_market(t,c,1532-(i*9),1523-(i*7),1510-(i*5),1502-(i*3),2024);}
ps("\n");mars_report();mars_state();ps("\n=== Demo Complete ===\n");return 0;}
