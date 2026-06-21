/* tsuga_admin: Tsuga management technology administration (v1.0)
 * Tsuga planning, tsuga execution, tsuga evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tsg_t;
typedef struct{int n_tgp,n_tge,n_tgv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tsg_state_t;
static tsg_t tsgps[N],tsgss[N-2],tsgvss[N-4],tsgas[N-6],tsgmks[N-6]; static tsg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tsg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tsg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TSG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tsg_init(void){if(init)return -1;st.n_tgp=0;st.n_tge=0;st.n_tgv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tsgps[i].active=0;for(int i=0;i<N-2;i++)tsgss[i].active=0;for(int i=0;i<N-4;i++)tsgvss[i].active=0;for(int i=0;i<N-6;i++)tsgas[i].active=0;for(int i=0;i<N-6;i++)tsgmks[i].active=0;init=1;ps("[TSG] Tsuga initialized\n");return 0;}
int tsg_planning(int t,int c,int a,int b,int d,int e,int y){return add(tsgps,&st.n_tgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tsg_execution(int t,int c,int a,int b,int d,int e,int y){return add(tsgss,&st.n_tge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tsg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tsgvss,&st.n_tgv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tsg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tsgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tsg_market(int t,int c,int a,int b,int d,int e,int y){return add(tsgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tsg_report(void){ps("[TSG] Tgp: ");pi(st.n_tgp);ps(" PCS=");pi(st.t_f1);ps("\nTge: ");pi(st.n_tge);ps(" PCS=");pi(st.t_f2);ps("\nTgv: ");pi(st.n_tgv);ps(" PCS=");pi(st.t_f3);ps("\nTgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tsg_state(void){ps("[TSG] Tgp=");pi(st.n_tgp);ps(" Tge=");pi(st.n_tge);ps(" Tgv=");pi(st.n_tgv);ps(" Tgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tsuga Admin Demo ===\n\n");tsg_init();
ps("Tsuga planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tsg_planning(t,c,869+(i*17),858+(i*14),838+(i*10),820+(i*6),2020+(i%5));}
ps("\nTsuga execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tsg_execution(t,c,858+(i*15),847+(i*12),829+(i*8),816+(i*5),2021+(i%4));}
ps("\nTsuga evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tsg_evaluation(t,c,850+(i*13),839+(i*10),823+(i*7),812+(i*4),2022+(i%3));}
ps("\nTsuga accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tsg_accessory(t,c,842+(i*11),833+(i*9),819+(i*6),809+(i*3),2023+(i%2));}
ps("\nTsuga marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tsg_market(t,c,836+(i*9),827+(i*7),814+(i*5),806+(i*3),2024);}
ps("\n");tsg_report();tsg_state();ps("\n=== Demo Complete ===\n");return 0;}
