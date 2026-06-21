/* convolvulus_admin: Convolvulus management technology administration (v1.0)
 * Convolvulus planning, convolvulus execution, convolvulus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} con_t;
typedef struct{int n_conp,n_cone,n_con2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} con_state_t;
static con_t conps[N],conss[N-2],convss[N-4],conas[N-6],conmks[N-6]; static con_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(con_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;con_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int con_init(void){if(init)return -1;st.n_conp=0;st.n_cone=0;st.n_con2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)conps[i].active=0;for(int i=0;i<N-2;i++)conss[i].active=0;for(int i=0;i<N-4;i++)convss[i].active=0;for(int i=0;i<N-6;i++)conas[i].active=0;for(int i=0;i<N-6;i++)conmks[i].active=0;init=1;ps("[CON] Convolvulus initialized\n");return 0;}
int con_planning(int t,int c,int a,int b,int d,int e,int y){return add(conps,&st.n_conp,&st.t_f1,N,t,c,a,b,d,e,y);}
int con_execution(int t,int c,int a,int b,int d,int e,int y){return add(conss,&st.n_cone,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int con_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(convss,&st.n_con2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int con_accessory(int t,int c,int a,int b,int d,int e,int y){return add(conas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int con_market(int t,int c,int a,int b,int d,int e,int y){return add(conmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void con_report(void){ps("[CON] Conp: ");pi(st.n_conp);ps(" PCS=");pi(st.t_f1);ps("\nCone: ");pi(st.n_cone);ps(" PCS=");pi(st.t_f2);ps("\nConv: ");pi(st.n_con2);ps(" PCS=");pi(st.t_f3);ps("\nConc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void con_state(void){ps("[CON] Conp=");pi(st.n_conp);ps(" Cone=");pi(st.n_cone);ps(" Conv=");pi(st.n_con2);ps(" Conc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Convolvulus Admin Demo ===\n\n");con_init();
ps("Convolvulus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;con_planning(t,c,912+(i*17),901+(i*14),881+(i*10),863+(i*6),2020+(i%5));}
ps("\nConvolvulus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;con_execution(t,c,901+(i*15),890+(i*12),872+(i*8),859+(i*5),2021+(i%4));}
ps("\nConvolvulus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;con_evaluation(t,c,893+(i*13),882+(i*10),866+(i*7),855+(i*4),2022+(i%3));}
ps("\nConvolvulus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;con_accessory(t,c,885+(i*11),876+(i*9),862+(i*6),852+(i*3),2023+(i%2));}
ps("\nConvolvulus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;con_market(t,c,879+(i*9),870+(i*7),857+(i*5),849+(i*3),2024);}
ps("\n");con_report();con_state();ps("\n=== Demo Complete ===\n");return 0;}
