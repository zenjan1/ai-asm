/* browallia_admin: Browallia management technology administration (v1.0)
 * Browallia planning, browallia execution, browallia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bro_t;
typedef struct{int n_brp,n_bre,n_brv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bro_state_t;
static bro_t brops[N],broes[N-2],brovs[N-4],acs[N-6],mks[N-6]; static bro_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bro_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bro_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BRO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bro_init(void){if(init)return -1;st.n_brp=0;st.n_bre=0;st.n_brv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brops[i].active=0;for(int i=0;i<N-2;i++)broes[i].active=0;for(int i=0;i<N-4;i++)brovs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BRO] Browallia initialized\n");return 0;}
int bro_planning(int t,int c,int a,int b,int d,int e,int y){return add(brops,&st.n_brp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bro_execution(int t,int c,int a,int b,int d,int e,int y){return add(broes,&st.n_bre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bro_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(brovs,&st.n_brv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bro_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bro_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bro_report(void){ps("[BRO] Brp: ");pi(st.n_brp);ps(" PCS=");pi(st.t_f1);ps("\nBre: ");pi(st.n_bre);ps(" PCS=");pi(st.t_f2);ps("\nBrv: ");pi(st.n_brv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bro_state(void){ps("[BRO] Brp=");pi(st.n_brp);ps(" Bre=");pi(st.n_bre);ps(" Brv=");pi(st.n_brv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Browallia Admin Demo ===\n\n");bro_init();
ps("Browallia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bro_planning(t,c,602+(i*17),591+(i*14),571+(i*10),553+(i*6),2020+(i%5));}
ps("\nBrowallia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bro_execution(t,c,591+(i*15),580+(i*12),562+(i*8),549+(i*5),2021+(i%4));}
ps("\nBrowallia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bro_evaluation(t,c,583+(i*13),572+(i*10),556+(i*7),545+(i*4),2022+(i%3));}
ps("\nBrowallia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bro_accessory(t,c,575+(i*11),566+(i*9),552+(i*6),542+(i*3),2023+(i%2));}
ps("\nBrowallia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bro_market(t,c,569+(i*9),560+(i*7),547+(i*5),539+(i*3),2024);}
ps("\n");bro_report();bro_state();ps("\n=== Demo Complete ===\n");return 0;}
