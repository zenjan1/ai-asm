/* astilbe_admin: Astilbe management technology administration (v1.0)
 * Astilbe planning, astilbe execution, astilbe evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ast_t;
typedef struct{int n_astp,n_aste,n_ast2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ast_state_t;
static ast_t astps[N],astss[N-2],astvss[N-4],astas[N-6],astmks[N-6]; static ast_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ast_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ast_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ast_init(void){if(init)return -1;st.n_astp=0;st.n_aste=0;st.n_ast2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)astps[i].active=0;for(int i=0;i<N-2;i++)astss[i].active=0;for(int i=0;i<N-4;i++)astvss[i].active=0;for(int i=0;i<N-6;i++)astas[i].active=0;for(int i=0;i<N-6;i++)astmks[i].active=0;init=1;ps("[AST] Astilbe initialized\n");return 0;}
int ast_planning(int t,int c,int a,int b,int d,int e,int y){return add(astps,&st.n_astp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ast_execution(int t,int c,int a,int b,int d,int e,int y){return add(astss,&st.n_aste,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ast_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(astvss,&st.n_ast2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ast_accessory(int t,int c,int a,int b,int d,int e,int y){return add(astas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ast_market(int t,int c,int a,int b,int d,int e,int y){return add(astmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ast_report(void){ps("[AST] Astp: ");pi(st.n_astp);ps(" PCS=");pi(st.t_f1);ps("\nAste: ");pi(st.n_aste);ps(" PCS=");pi(st.t_f2);ps("\nAstv: ");pi(st.n_ast2);ps(" PCS=");pi(st.t_f3);ps("\nAstc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ast_state(void){ps("[AST] Astp=");pi(st.n_astp);ps(" Aste=");pi(st.n_aste);ps(" Astv=");pi(st.n_ast2);ps(" Astc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Astilbe Admin Demo ===\n\n");ast_init();
ps("Astilbe planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ast_planning(t,c,976+(i*17),965+(i*14),945+(i*10),927+(i*6),2020+(i%5));}
ps("\nAstilbe execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ast_execution(t,c,965+(i*15),954+(i*12),936+(i*8),923+(i*5),2021+(i%4));}
ps("\nAstilbe evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ast_evaluation(t,c,957+(i*13),946+(i*10),930+(i*7),919+(i*4),2022+(i%3));}
ps("\nAstilbe accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ast_accessory(t,c,949+(i*11),940+(i*9),926+(i*6),916+(i*3),2023+(i%2));}
ps("\nAstilbe marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ast_market(t,c,943+(i*9),934+(i*7),921+(i*5),913+(i*3),2024);}
ps("\n");ast_report();ast_state();ps("\n=== Demo Complete ===\n");return 0;}
