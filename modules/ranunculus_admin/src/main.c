/* ranunculus_admin: Ranunculus management technology administration (v1.0)
 * Ranunculus planning, ranunculus execution, ranunculus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ran_t;
typedef struct{int n_rap,n_rae,n_rav,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ran_state_t;
static ran_t raps[N],raes[N-2],ravs[N-4],acs[N-6],mks[N-6]; static ran_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ran_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ran_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ran_init(void){if(init)return -1;st.n_rap=0;st.n_rae=0;st.n_rav=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)raps[i].active=0;for(int i=0;i<N-2;i++)raes[i].active=0;for(int i=0;i<N-4;i++)ravs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RAN] Ranunculus initialized\n");return 0;}
int ran_planning(int t,int c,int a,int b,int d,int e,int y){return add(raps,&st.n_rap,&st.t_f1,N,t,c,a,b,d,e,y);}
int ran_execution(int t,int c,int a,int b,int d,int e,int y){return add(raes,&st.n_rae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ran_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ravs,&st.n_rav,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ran_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ran_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ran_report(void){ps("[RAN] Rap: ");pi(st.n_rap);ps(" PCS=");pi(st.t_f1);ps("\nRae: ");pi(st.n_rae);ps(" PCS=");pi(st.t_f2);ps("\nRav: ");pi(st.n_rav);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ran_state(void){ps("[RAN] Rap=");pi(st.n_rap);ps(" Rae=");pi(st.n_rae);ps(" Rav=");pi(st.n_rav);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ranunculus Admin Demo ===\n\n");ran_init();
ps("Ranunculus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ran_planning(t,c,615+(i*17),604+(i*14),584+(i*10),566+(i*6),2020+(i%5));}
ps("\nRanunculus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ran_execution(t,c,604+(i*15),593+(i*12),575+(i*8),562+(i*5),2021+(i%4));}
ps("\nRanunculus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ran_evaluation(t,c,596+(i*13),585+(i*10),569+(i*7),558+(i*4),2022+(i%3));}
ps("\nRanunculus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ran_accessory(t,c,588+(i*11),579+(i*9),565+(i*6),555+(i*3),2023+(i%2));}
ps("\nRanunculus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ran_market(t,c,582+(i*9),573+(i*7),560+(i*5),552+(i*3),2024);}
ps("\n");ran_report();ran_state();ps("\n=== Demo Complete ===\n");return 0;}
