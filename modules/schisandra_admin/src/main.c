/* schisandra_admin: Schisandra management technology administration (v1.0)
 * Schisandra planning, schisandra execution, schisandra evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sch_t;
typedef struct{int n_shp,n_she,n_shv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sch_state_t;
static sch_t schps[N],schss[N-2],schvss[N-4],schas[N-6],schmks[N-6]; static sch_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sch_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sch_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SCH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sch_init(void){if(init)return -1;st.n_shp=0;st.n_she=0;st.n_shv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)schps[i].active=0;for(int i=0;i<N-2;i++)schss[i].active=0;for(int i=0;i<N-4;i++)schvss[i].active=0;for(int i=0;i<N-6;i++)schas[i].active=0;for(int i=0;i<N-6;i++)schmks[i].active=0;init=1;ps("[SCH] Schisandra initialized\n");return 0;}
int sch_planning(int t,int c,int a,int b,int d,int e,int y){return add(schps,&st.n_shp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sch_execution(int t,int c,int a,int b,int d,int e,int y){return add(schss,&st.n_she,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sch_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(schvss,&st.n_shv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sch_accessory(int t,int c,int a,int b,int d,int e,int y){return add(schas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sch_market(int t,int c,int a,int b,int d,int e,int y){return add(schmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sch_report(void){ps("[SCH] Shp: ");pi(st.n_shp);ps(" PCS=");pi(st.t_f1);ps("\nShe: ");pi(st.n_she);ps(" PCS=");pi(st.t_f2);ps("\nShv: ");pi(st.n_shv);ps(" PCS=");pi(st.t_f3);ps("\nShc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sch_state(void){ps("[SCH] Shp=");pi(st.n_shp);ps(" She=");pi(st.n_she);ps(" Shv=");pi(st.n_shv);ps(" Shc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Schisandra Admin Demo ===\n\n");sch_init();
ps("Schisandra planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sch_planning(t,c,863+(i*17),852+(i*14),832+(i*10),814+(i*6),2020+(i%5));}
ps("\nSchisandra execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sch_execution(t,c,852+(i*15),841+(i*12),823+(i*8),810+(i*5),2021+(i%4));}
ps("\nSchisandra evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sch_evaluation(t,c,844+(i*13),833+(i*10),817+(i*7),806+(i*4),2022+(i%3));}
ps("\nSchisandra accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sch_accessory(t,c,836+(i*11),827+(i*9),813+(i*6),803+(i*3),2023+(i%2));}
ps("\nSchisandra marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sch_market(t,c,830+(i*9),821+(i*7),808+(i*5),800+(i*3),2024);}
ps("\n");sch_report();sch_state();ps("\n=== Demo Complete ===\n");return 0;}
