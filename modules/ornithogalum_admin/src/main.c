/* ornithogalum_admin: Ornithogalum management technology administration (v1.0)
 * Ornithogalum planning, ornithogalum execution, ornithogalum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} orn_t;
typedef struct{int n_orp,n_ore,n_orv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} orn_state_t;
static orn_t ornps[N],ornes[N-2],ornvs[N-4],acs[N-6],mks[N-6]; static orn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(orn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;orn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ORN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int orn_init(void){if(init)return -1;st.n_orp=0;st.n_ore=0;st.n_orv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ornps[i].active=0;for(int i=0;i<N-2;i++)ornes[i].active=0;for(int i=0;i<N-4;i++)ornvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ORN] Ornithogalum initialized\n");return 0;}
int orn_planning(int t,int c,int a,int b,int d,int e,int y){return add(ornps,&st.n_orp,&st.t_f1,N,t,c,a,b,d,e,y);}
int orn_execution(int t,int c,int a,int b,int d,int e,int y){return add(ornes,&st.n_ore,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int orn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ornvs,&st.n_orv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int orn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int orn_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void orn_report(void){ps("[ORN] Orp: ");pi(st.n_orp);ps(" PCS=");pi(st.t_f1);ps("\nOre: ");pi(st.n_ore);ps(" PCS=");pi(st.t_f2);ps("\nOrv: ");pi(st.n_orv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void orn_state(void){ps("[ORN] Orp=");pi(st.n_orp);ps(" Ore=");pi(st.n_ore);ps(" Orv=");pi(st.n_orv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ornithogalum Admin Demo ===\n\n");orn_init();
ps("Ornithogalum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;orn_planning(t,c,608+(i*17),597+(i*14),577+(i*10),559+(i*6),2020+(i%5));}
ps("\nOrnithogalum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;orn_execution(t,c,597+(i*15),586+(i*12),568+(i*8),555+(i*5),2021+(i%4));}
ps("\nOrnithogalum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;orn_evaluation(t,c,589+(i*13),578+(i*10),562+(i*7),551+(i*4),2022+(i%3));}
ps("\nOrnithogalum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;orn_accessory(t,c,581+(i*11),572+(i*9),558+(i*6),548+(i*3),2023+(i%2));}
ps("\nOrnithogalum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;orn_market(t,c,575+(i*9),566+(i*7),553+(i*5),545+(i*3),2024);}
ps("\n");orn_report();orn_state();ps("\n=== Demo Complete ===\n");return 0;}
