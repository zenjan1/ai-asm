/* opuntia_admin: Opuntia management technology administration (v1.0)
 * Opuntia planning, opuntia execution, opuntia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} opn_t;
typedef struct{int n_opp,n_ope,n_opv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} opn_state_t;
static opn_t opnps[N],opnss[N-2],opnvss[N-4],opnas[N-6],opnmks[N-6]; static opn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(opn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;opn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OPN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int opn_init(void){if(init)return -1;st.n_opp=0;st.n_ope=0;st.n_opv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)opnps[i].active=0;for(int i=0;i<N-2;i++)opnss[i].active=0;for(int i=0;i<N-4;i++)opnvss[i].active=0;for(int i=0;i<N-6;i++)opnas[i].active=0;for(int i=0;i<N-6;i++)opnmks[i].active=0;init=1;ps("[OPN] Opuntia initialized\n");return 0;}
int opn_planning(int t,int c,int a,int b,int d,int e,int y){return add(opnps,&st.n_opp,&st.t_f1,N,t,c,a,b,d,e,y);}
int opn_execution(int t,int c,int a,int b,int d,int e,int y){return add(opnss,&st.n_ope,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int opn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(opnvss,&st.n_opv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int opn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(opnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int opn_market(int t,int c,int a,int b,int d,int e,int y){return add(opnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void opn_report(void){ps("[OPN] Opp: ");pi(st.n_opp);ps(" PCS=");pi(st.t_f1);ps("\nOpe: ");pi(st.n_ope);ps(" PCS=");pi(st.t_f2);ps("\nOpv: ");pi(st.n_opv);ps(" PCS=");pi(st.t_f3);ps("\nOpc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void opn_state(void){ps("[OPN] Opp=");pi(st.n_opp);ps(" Ope=");pi(st.n_ope);ps(" Opv=");pi(st.n_opv);ps(" Opc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Opuntia Admin Demo ===\n\n");opn_init();
ps("Opuntia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;opn_planning(t,c,847+(i*17),836+(i*14),816+(i*10),798+(i*6),2020+(i%5));}
ps("\nOpuntia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;opn_execution(t,c,836+(i*15),825+(i*12),807+(i*8),794+(i*5),2021+(i%4));}
ps("\nOpuntia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;opn_evaluation(t,c,828+(i*13),817+(i*10),801+(i*7),790+(i*4),2022+(i%3));}
ps("\nOpuntia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;opn_accessory(t,c,820+(i*11),811+(i*9),797+(i*6),787+(i*3),2023+(i%2));}
ps("\nOpuntia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;opn_market(t,c,814+(i*9),805+(i*7),792+(i*5),784+(i*3),2024);}
ps("\n");opn_report();opn_state();ps("\n=== Demo Complete ===\n");return 0;}
