/* disporum_admin: Disporum management technology administration (v1.0)
 * Disporum planning, disporum execution, disporum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dis_t;
typedef struct{int n_disp,n_dise,n_dis2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dis_state_t;
static dis_t disps[N],diss[N-2],disvss[N-4],disas[N-6],dismks[N-6]; static dis_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dis_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dis_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DIS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dis_init(void){if(init)return -1;st.n_disp=0;st.n_dise=0;st.n_dis2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)disps[i].active=0;for(int i=0;i<N-2;i++)diss[i].active=0;for(int i=0;i<N-4;i++)disvss[i].active=0;for(int i=0;i<N-6;i++)disas[i].active=0;for(int i=0;i<N-6;i++)dismks[i].active=0;init=1;ps("[DIS] Disporum initialized\n");return 0;}
int dis_planning(int t,int c,int a,int b,int d,int e,int y){return add(disps,&st.n_disp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dis_execution(int t,int c,int a,int b,int d,int e,int y){return add(diss,&st.n_dise,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dis_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(disvss,&st.n_dis2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dis_accessory(int t,int c,int a,int b,int d,int e,int y){return add(disas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dis_market(int t,int c,int a,int b,int d,int e,int y){return add(dismks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dis_report(void){ps("[DIS] Disp: ");pi(st.n_disp);ps(" PCS=");pi(st.t_f1);ps("\nDise: ");pi(st.n_dise);ps(" PCS=");pi(st.t_f2);ps("\nDisv: ");pi(st.n_dis2);ps(" PCS=");pi(st.t_f3);ps("\nDisc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dis_state(void){ps("[DIS] Disp=");pi(st.n_disp);ps(" Dise=");pi(st.n_dise);ps(" Disv=");pi(st.n_dis2);ps(" Disc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Disporum Admin Demo ===\n\n");dis_init();
ps("Disporum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dis_planning(t,c,916+(i*17),905+(i*14),885+(i*10),867+(i*6),2020+(i%5));}
ps("\nDisporum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dis_execution(t,c,905+(i*15),894+(i*12),876+(i*8),863+(i*5),2021+(i%4));}
ps("\nDisporum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dis_evaluation(t,c,897+(i*13),886+(i*10),870+(i*7),859+(i*4),2022+(i%3));}
ps("\nDisporum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dis_accessory(t,c,889+(i*11),880+(i*9),866+(i*6),856+(i*3),2023+(i%2));}
ps("\nDisporum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dis_market(t,c,883+(i*9),874+(i*7),861+(i*5),853+(i*3),2024);}
ps("\n");dis_report();dis_state();ps("\n=== Demo Complete ===\n");return 0;}
