/* digitalis_admin: Digitalis management technology administration (v1.0)
 * Digitalis planning, digitalis execution, digitalis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dg_t;
typedef struct{int n_dgp,n_dge,n_dgv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dg_state_t;
static dg_t dgps[N],dgss[N-2],dgvss[N-4],dgas[N-6],dgmks[N-6]; static dg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DGT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dgt_init(void){if(init)return -1;st.n_dgp=0;st.n_dge=0;st.n_dgv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dgps[i].active=0;for(int i=0;i<N-2;i++)dgss[i].active=0;for(int i=0;i<N-4;i++)dgvss[i].active=0;for(int i=0;i<N-6;i++)dgas[i].active=0;for(int i=0;i<N-6;i++)dgmks[i].active=0;init=1;ps("[DGT] Digitalis initialized\n");return 0;}
int dgt_planning(int t,int c,int a,int b,int d,int e,int y){return add(dgps,&st.n_dgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dgt_execution(int t,int c,int a,int b,int d,int e,int y){return add(dgss,&st.n_dge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dgt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dgvss,&st.n_dgv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dgt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dgt_market(int t,int c,int a,int b,int d,int e,int y){return add(dgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dgt_report(void){ps("[DGT] Dgp: ");pi(st.n_dgp);ps(" PCS=");pi(st.t_f1);ps("\nDge: ");pi(st.n_dge);ps(" PCS=");pi(st.t_f2);ps("\nDgv: ");pi(st.n_dgv);ps(" PCS=");pi(st.t_f3);ps("\nDgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dgt_state(void){ps("[DGT] Dgp=");pi(st.n_dgp);ps(" Dge=");pi(st.n_dge);ps(" Dgv=");pi(st.n_dgv);ps(" Dgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Digitalis Admin Demo ===\n\n");dgt_init();
ps("Digitalis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dgt_planning(t,c,767+(i*17),756+(i*14),736+(i*10),718+(i*6),2020+(i%5));}
ps("\nDigitalis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dgt_execution(t,c,756+(i*15),745+(i*12),727+(i*8),714+(i*5),2021+(i%4));}
ps("\nDigitalis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dgt_evaluation(t,c,748+(i*13),737+(i*10),723+(i*7),712+(i*4),2022+(i%3));}
ps("\nDigitalis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dgt_accessory(t,c,740+(i*11),731+(i*9),717+(i*6),707+(i*3),2023+(i%2));}
ps("\nDigitalis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dgt_market(t,c,734+(i*9),725+(i*7),712+(i*5),704+(i*3),2024);}
ps("\n");dgt_report();dgt_state();ps("\n=== Demo Complete ===\n");return 0;}
