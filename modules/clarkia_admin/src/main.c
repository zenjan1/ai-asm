/* clarkia_admin: Clarkia management technology administration (v1.0)
 * Clarkia planning, clarkia execution, clarkia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cla_t;
typedef struct{int n_clap,n_clae,n_cla2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cla_state_t;
static cla_t claps[N],class2[N-2],clavss[N-4],claas[N-6],clamks[N-6]; static cla_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cla_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cla_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cla_init(void){if(init)return -1;st.n_clap=0;st.n_clae=0;st.n_cla2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)claps[i].active=0;for(int i=0;i<N-2;i++)class2[i].active=0;for(int i=0;i<N-4;i++)clavss[i].active=0;for(int i=0;i<N-6;i++)claas[i].active=0;for(int i=0;i<N-6;i++)clamks[i].active=0;init=1;ps("[CLA] Clarkia initialized\n");return 0;}
int cla_planning(int t,int c,int a,int b,int d,int e,int y){return add(claps,&st.n_clap,&st.t_f1,N,t,c,a,b,d,e,y);}
int cla_execution(int t,int c,int a,int b,int d,int e,int y){return add(class2,&st.n_clae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cla_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(clavss,&st.n_cla2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cla_accessory(int t,int c,int a,int b,int d,int e,int y){return add(claas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cla_market(int t,int c,int a,int b,int d,int e,int y){return add(clamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cla_report(void){ps("[CLA] Clap: ");pi(st.n_clap);ps(" PCS=");pi(st.t_f1);ps("\nClae: ");pi(st.n_clae);ps(" PCS=");pi(st.t_f2);ps("\nClav: ");pi(st.n_cla2);ps(" PCS=");pi(st.t_f3);ps("\nClac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cla_state(void){ps("[CLA] Clap=");pi(st.n_clap);ps(" Clae=");pi(st.n_clae);ps(" Clav=");pi(st.n_cla2);ps(" Clac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Clarkia Admin Demo ===\n\n");cla_init();
ps("Clarkia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cla_planning(t,c,910+(i*17),899+(i*14),879+(i*10),861+(i*6),2020+(i%5));}
ps("\nClarkia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cla_execution(t,c,899+(i*15),888+(i*12),870+(i*8),857+(i*5),2021+(i%4));}
ps("\nClarkia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cla_evaluation(t,c,891+(i*13),880+(i*10),864+(i*7),853+(i*4),2022+(i%3));}
ps("\nClarkia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cla_accessory(t,c,883+(i*11),874+(i*9),860+(i*6),850+(i*3),2023+(i%2));}
ps("\nClarkia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cla_market(t,c,877+(i*9),868+(i*7),855+(i*5),847+(i*3),2024);}
ps("\n");cla_report();cla_state();ps("\n=== Demo Complete ===\n");return 0;}
