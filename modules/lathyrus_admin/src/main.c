/* lathyrus_admin: Lathyrus management technology administration (v1.0)
 * Lathyrus planning, lathyrus execution, lathyrus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lat_t;
typedef struct{int n_latp,n_late,n_lat2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lat_state_t;
static lat_t latps[N],latss[N-2],latvss[N-4],latas[N-6],latmks[N-6]; static lat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lat_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lat_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lat_init(void){if(init)return -1;st.n_latp=0;st.n_late=0;st.n_lat2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)latps[i].active=0;for(int i=0;i<N-2;i++)latss[i].active=0;for(int i=0;i<N-4;i++)latvss[i].active=0;for(int i=0;i<N-6;i++)latas[i].active=0;for(int i=0;i<N-6;i++)latmks[i].active=0;init=1;ps("[LAT] Lathyrus initialized\n");return 0;}
int lat_planning(int t,int c,int a,int b,int d,int e,int y){return add(latps,&st.n_latp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lat_execution(int t,int c,int a,int b,int d,int e,int y){return add(latss,&st.n_late,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lat_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(latvss,&st.n_lat2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lat_accessory(int t,int c,int a,int b,int d,int e,int y){return add(latas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lat_market(int t,int c,int a,int b,int d,int e,int y){return add(latmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lat_report(void){ps("[LAT] Latp: ");pi(st.n_latp);ps(" PCS=");pi(st.t_f1);ps("\nLate: ");pi(st.n_late);ps(" PCS=");pi(st.t_f2);ps("\nLatv: ");pi(st.n_lat2);ps(" PCS=");pi(st.t_f3);ps("\nLatc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lat_state(void){ps("[LAT] Latp=");pi(st.n_latp);ps(" Late=");pi(st.n_late);ps(" Latv=");pi(st.n_lat2);ps(" Latc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lathyrus Admin Demo ===\n\n");lat_init();
ps("Lathyrus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lat_planning(t,c,935+(i*17),924+(i*14),904+(i*10),886+(i*6),2020+(i%5));}
ps("\nLathyrus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lat_execution(t,c,924+(i*15),913+(i*12),895+(i*8),882+(i*5),2021+(i%4));}
ps("\nLathyrus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lat_evaluation(t,c,916+(i*13),905+(i*10),889+(i*7),878+(i*4),2022+(i%3));}
ps("\nLathyrus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lat_accessory(t,c,908+(i*11),899+(i*9),885+(i*6),875+(i*3),2023+(i%2));}
ps("\nLathyrus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lat_market(t,c,902+(i*9),893+(i*7),880+(i*5),872+(i*3),2024);}
ps("\n");lat_report();lat_state();ps("\n=== Demo Complete ===\n");return 0;}
