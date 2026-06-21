/* oregano_admin: Oregano management technology administration (v1.0)
 * Oregano planning, oregano execution, oregano evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} or_t;
typedef struct{int n_orp,n_ore,n_orv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} or_state_t;
static or_t orps[N],orss[N-2],orvss[N-4],oras[N-6],ormks[N-6]; static or_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(or_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;or_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ORG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int or_init(void){if(init)return -1;st.n_orp=0;st.n_ore=0;st.n_orv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)orps[i].active=0;for(int i=0;i<N-2;i++)orss[i].active=0;for(int i=0;i<N-4;i++)orvss[i].active=0;for(int i=0;i<N-6;i++)oras[i].active=0;for(int i=0;i<N-6;i++)ormks[i].active=0;init=1;ps("[ORG] Oregano initialized\n");return 0;}
int or_planning(int t,int c,int a,int b,int d,int e,int y){return add(orps,&st.n_orp,&st.t_f1,N,t,c,a,b,d,e,y);}
int or_execution(int t,int c,int a,int b,int d,int e,int y){return add(orss,&st.n_ore,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int or_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(orvss,&st.n_orv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int or_accessory(int t,int c,int a,int b,int d,int e,int y){return add(oras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int or_market(int t,int c,int a,int b,int d,int e,int y){return add(ormks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void or_report(void){ps("[ORG] Orp: ");pi(st.n_orp);ps(" PCS=");pi(st.t_f1);ps("\nOre: ");pi(st.n_ore);ps(" PCS=");pi(st.t_f2);ps("\nOrv: ");pi(st.n_orv);ps(" PCS=");pi(st.t_f3);ps("\nOrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void or_state(void){ps("[ORG] Orp=");pi(st.n_orp);ps(" Ore=");pi(st.n_ore);ps(" Orv=");pi(st.n_orv);ps(" Orc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oregano Admin Demo ===\n\n");or_init();
ps("Oregano planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;or_planning(t,c,654+(i*17),643+(i*14),623+(i*10),605+(i*6),2020+(i%5));}
ps("\nOregano execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;or_execution(t,c,643+(i*15),632+(i*12),614+(i*8),601+(i*5),2021+(i%4));}
ps("\nOregano evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;or_evaluation(t,c,635+(i*13),624+(i*10),608+(i*7),597+(i*4),2022+(i%3));}
ps("\nOregano accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;or_accessory(t,c,627+(i*11),618+(i*9),604+(i*6),594+(i*3),2023+(i%2));}
ps("\nOregano marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;or_market(t,c,621+(i*9),612+(i*7),599+(i*5),591+(i*3),2024);}
ps("\n");or_report();or_state();ps("\n=== Demo Complete ===\n");return 0;}
