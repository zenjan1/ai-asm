/* groundsel_admin: Groundsel management technology administration (v1.0)
 * Groundsel planning, groundsel execution, groundsel evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gd_t;
typedef struct{int n_gdp,n_gde,n_gdv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gd_state_t;
static gd_t gdps[N],gdss[N-2],gdvss[N-4],gdas[N-6],gdmks[N-6]; static gd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GDS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gd_init(void){if(init)return -1;st.n_gdp=0;st.n_gde=0;st.n_gdv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gdps[i].active=0;for(int i=0;i<N-2;i++)gdss[i].active=0;for(int i=0;i<N-4;i++)gdvss[i].active=0;for(int i=0;i<N-6;i++)gdas[i].active=0;for(int i=0;i<N-6;i++)gdmks[i].active=0;init=1;ps("[GDS] Groundsel initialized\n");return 0;}
int gd_planning(int t,int c,int a,int b,int d,int e,int y){return add(gdps,&st.n_gdp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gd_execution(int t,int c,int a,int b,int d,int e,int y){return add(gdss,&st.n_gde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gd_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gdvss,&st.n_gdv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gdas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gd_market(int t,int c,int a,int b,int d,int e,int y){return add(gdmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gd_report(void){ps("[GDS] Gdp: ");pi(st.n_gdp);ps(" PCS=");pi(st.t_f1);ps("\nGde: ");pi(st.n_gde);ps(" PCS=");pi(st.t_f2);ps("\nGdv: ");pi(st.n_gdv);ps(" PCS=");pi(st.t_f3);ps("\nGdc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gd_state(void){ps("[GDS] Gdp=");pi(st.n_gdp);ps(" Gde=");pi(st.n_gde);ps(" Gdv=");pi(st.n_gdv);ps(" Gdc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Groundsel Admin Demo ===\n\n");gd_init();
ps("Groundsel planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gd_planning(t,c,702+(i*17),691+(i*14),671+(i*10),653+(i*6),2020+(i%5));}
ps("\nGroundsel execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gd_execution(t,c,691+(i*15),680+(i*12),662+(i*8),649+(i*5),2021+(i%4));}
ps("\nGroundsel evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gd_evaluation(t,c,683+(i*13),672+(i*10),656+(i*7),645+(i*4),2022+(i%3));}
ps("\nGroundsel accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gd_accessory(t,c,675+(i*11),666+(i*9),652+(i*6),642+(i*3),2023+(i%2));}
ps("\nGroundsel marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gd_market(t,c,669+(i*9),660+(i*7),647+(i*5),639+(i*3),2024);}
ps("\n");gd_report();gd_state();ps("\n=== Demo Complete ===\n");return 0;}
