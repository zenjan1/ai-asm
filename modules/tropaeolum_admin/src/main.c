/* tropaeolum_admin: Tropaeolum management technology administration (v1.0)
 * Tropaeolum planning, tropaeolum execution, tropaeolum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tro_t;
typedef struct{int n_trp,n_tre,n_trv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tro_state_t;
static tro_t trps[N],trss[N-2],trvss[N-4],tras[N-6],trmks[N-6]; static tro_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tro_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tro_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TPL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tpl_init(void){if(init)return -1;st.n_trp=0;st.n_tre=0;st.n_trv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)trps[i].active=0;for(int i=0;i<N-2;i++)trss[i].active=0;for(int i=0;i<N-4;i++)trvss[i].active=0;for(int i=0;i<N-6;i++)tras[i].active=0;for(int i=0;i<N-6;i++)trmks[i].active=0;init=1;ps("[TPL] Tropaeolum initialized\n");return 0;}
int tpl_planning(int t,int c,int a,int b,int d,int e,int y){return add(trps,&st.n_trp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tpl_execution(int t,int c,int a,int b,int d,int e,int y){return add(trss,&st.n_tre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tpl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(trvss,&st.n_trv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tpl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tpl_market(int t,int c,int a,int b,int d,int e,int y){return add(trmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tpl_report(void){ps("[TPL] Trp: ");pi(st.n_trp);ps(" PCS=");pi(st.t_f1);ps("\nTre: ");pi(st.n_tre);ps(" PCS=");pi(st.t_f2);ps("\nTrv: ");pi(st.n_trv);ps(" PCS=");pi(st.t_f3);ps("\nTrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tpl_state(void){ps("[TPL] Trp=");pi(st.n_trp);ps(" Tre=");pi(st.n_tre);ps(" Trv=");pi(st.n_trv);ps(" Trc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tropaeolum Admin Demo ===\n\n");tpl_init();
ps("Tropaeolum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tpl_planning(t,c,807+(i*17),796+(i*14),776+(i*10),758+(i*6),2020+(i%5));}
ps("\nTropaeolum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tpl_execution(t,c,796+(i*15),785+(i*12),767+(i*8),754+(i*5),2021+(i%4));}
ps("\nTropaeolum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tpl_evaluation(t,c,788+(i*13),777+(i*10),763+(i*7),752+(i*4),2022+(i%3));}
ps("\nTropaeolum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tpl_accessory(t,c,780+(i*11),771+(i*9),757+(i*6),747+(i*3),2023+(i%2));}
ps("\nTropaeolum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tpl_market(t,c,774+(i*9),765+(i*7),752+(i*5),744+(i*3),2024);}
ps("\n");tpl_report();tpl_state();ps("\n=== Demo Complete ===\n");return 0;}
