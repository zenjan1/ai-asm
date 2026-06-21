/* bistort_admin: Bistort management technology administration (v1.0)
 * Bistort planning, bistort execution, bistort evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bi_t;
typedef struct{int n_bsp,n_bse,n_bsv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bi_state_t;
static bi_t bsps[N],bsss[N-2],bsvss[N-4],bsas[N-6],bsmks[N-6]; static bi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bs_init(void){if(init)return -1;st.n_bsp=0;st.n_bse=0;st.n_bsv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bsps[i].active=0;for(int i=0;i<N-2;i++)bsss[i].active=0;for(int i=0;i<N-4;i++)bsvss[i].active=0;for(int i=0;i<N-6;i++)bsas[i].active=0;for(int i=0;i<N-6;i++)bsmks[i].active=0;init=1;ps("[BST] Bistort initialized\n");return 0;}
int bs_planning(int t,int c,int a,int b,int d,int e,int y){return add(bsps,&st.n_bsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bs_execution(int t,int c,int a,int b,int d,int e,int y){return add(bsss,&st.n_bse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bs_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bsvss,&st.n_bsv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bs_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bsas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bs_market(int t,int c,int a,int b,int d,int e,int y){return add(bsmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bs_report(void){ps("[BST] Bsp: ");pi(st.n_bsp);ps(" PCS=");pi(st.t_f1);ps("\nBse: ");pi(st.n_bse);ps(" PCS=");pi(st.t_f2);ps("\nBsv: ");pi(st.n_bsv);ps(" PCS=");pi(st.t_f3);ps("\nBsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bs_state(void){ps("[BST] Bsp=");pi(st.n_bsp);ps(" Bse=");pi(st.n_bse);ps(" Bsv=");pi(st.n_bsv);ps(" Bsc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bistort Admin Demo ===\n\n");bs_init();
ps("Bistort planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bs_planning(t,c,705+(i*17),694+(i*14),674+(i*10),656+(i*6),2020+(i%5));}
ps("\nBistort execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bs_execution(t,c,694+(i*15),683+(i*12),665+(i*8),652+(i*5),2021+(i%4));}
ps("\nBistort evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bs_evaluation(t,c,686+(i*13),675+(i*10),659+(i*7),648+(i*4),2022+(i%3));}
ps("\nBistort accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bs_accessory(t,c,678+(i*11),669+(i*9),655+(i*6),645+(i*3),2023+(i%2));}
ps("\nBistort marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bs_market(t,c,672+(i*9),663+(i*7),650+(i*5),642+(i*3),2024);}
ps("\n");bs_report();bs_state();ps("\n=== Demo Complete ===\n");return 0;}
