/* aster_admin: Aster management technology administration (v1.0)
 * Aster planning, aster execution, aster evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ast_t;
typedef struct{int n_asp,n_ase,n_asv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ast_state_t;
static ast_t asps[N],ases[N-2],asvs[N-4],acs[N-6],mks[N-6]; static ast_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ast_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ast_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ast_init(void){if(init)return -1;st.n_asp=0;st.n_ase=0;st.n_asv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)asps[i].active=0;for(int i=0;i<N-2;i++)ases[i].active=0;for(int i=0;i<N-4;i++)asvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[AST] Aster initialized\n");return 0;}
int ast_planning(int t,int c,int a,int b,int d,int e,int y){return add(asps,&st.n_asp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ast_execution(int t,int c,int a,int b,int d,int e,int y){return add(ases,&st.n_ase,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ast_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(asvs,&st.n_asv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ast_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ast_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ast_report(void){ps("[AST] Asp: ");pi(st.n_asp);ps(" PCS=");pi(st.t_f1);ps("\nAse: ");pi(st.n_ase);ps(" PCS=");pi(st.t_f2);ps("\nAsv: ");pi(st.n_asv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ast_state(void){ps("[AST] Asp=");pi(st.n_asp);ps(" Ase=");pi(st.n_ase);ps(" Asv=");pi(st.n_asv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aster Admin Demo ===\n\n");ast_init();
ps("Aster planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ast_planning(t,c,573+(i*17),562+(i*14),542+(i*10),524+(i*6),2020+(i%5));}
ps("\nAster execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ast_execution(t,c,562+(i*15),551+(i*12),533+(i*8),520+(i*5),2021+(i%4));}
ps("\nAster evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ast_evaluation(t,c,554+(i*13),543+(i*10),527+(i*7),516+(i*4),2022+(i%3));}
ps("\nAster accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ast_accessory(t,c,546+(i*11),537+(i*9),523+(i*6),513+(i*3),2023+(i%2));}
ps("\nAster marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ast_market(t,c,540+(i*9),531+(i*7),518+(i*5),510+(i*3),2024);}
ps("\n");ast_report();ast_state();ps("\n=== Demo Complete ===\n");return 0;}
