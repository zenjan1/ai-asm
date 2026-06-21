/* chamomile_admin: Chamomile management technology administration (v1.0)
 * Chamomile planning, chamomile execution, chamomile evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cm_t;
typedef struct{int n_cmp,n_cme,n_cmv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cm_state_t;
static cm_t cmps[N],cmss[N-2],cmvss[N-4],cmas[N-6],cmmks[N-6]; static cm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cm_init(void){if(init)return -1;st.n_cmp=0;st.n_cme=0;st.n_cmv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cmps[i].active=0;for(int i=0;i<N-2;i++)cmss[i].active=0;for(int i=0;i<N-4;i++)cmvss[i].active=0;for(int i=0;i<N-6;i++)cmas[i].active=0;for(int i=0;i<N-6;i++)cmmks[i].active=0;init=1;ps("[CHM] Chamomile initialized\n");return 0;}
int cm_planning(int t,int c,int a,int b,int d,int e,int y){return add(cmps,&st.n_cmp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cm_execution(int t,int c,int a,int b,int d,int e,int y){return add(cmss,&st.n_cme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cmvss,&st.n_cmv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cmas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cm_market(int t,int c,int a,int b,int d,int e,int y){return add(cmmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cm_report(void){ps("[CHM] Cmp: ");pi(st.n_cmp);ps(" PCS=");pi(st.t_f1);ps("\nCme: ");pi(st.n_cme);ps(" PCS=");pi(st.t_f2);ps("\nCmv: ");pi(st.n_cmv);ps(" PCS=");pi(st.t_f3);ps("\nCmc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cm_state(void){ps("[CHM] Cmp=");pi(st.n_cmp);ps(" Cme=");pi(st.n_cme);ps(" Cmv=");pi(st.n_cmv);ps(" Cmc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chamomile Admin Demo ===\n\n");cm_init();
ps("Chamomile planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cm_planning(t,c,666+(i*17),655+(i*14),635+(i*10),617+(i*6),2020+(i%5));}
ps("\nChamomile execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cm_execution(t,c,655+(i*15),644+(i*12),626+(i*8),613+(i*5),2021+(i%4));}
ps("\nChamomile evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cm_evaluation(t,c,647+(i*13),636+(i*10),620+(i*7),609+(i*4),2022+(i%3));}
ps("\nChamomile accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cm_accessory(t,c,639+(i*11),630+(i*9),616+(i*6),606+(i*3),2023+(i%2));}
ps("\nChamomile marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cm_market(t,c,633+(i*9),624+(i*7),611+(i*5),603+(i*3),2024);}
ps("\n");cm_report();cm_state();ps("\n=== Demo Complete ===\n");return 0;}
