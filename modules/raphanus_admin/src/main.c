/* raphanus_admin: Raphanus management technology administration (v1.0)
 * Raphanus planning, raphanus execution, raphanus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rb_t;
typedef struct{int n_rbp,n_rbe,n_rbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rb_state_t;
static rb_t rbps[N],rbss[N-2],rbvss[N-4],rbas[N-6],rbmks[N-6]; static rb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RBN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rbn_init(void){if(init)return -1;st.n_rbp=0;st.n_rbe=0;st.n_rbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rbps[i].active=0;for(int i=0;i<N-2;i++)rbss[i].active=0;for(int i=0;i<N-4;i++)rbvss[i].active=0;for(int i=0;i<N-6;i++)rbas[i].active=0;for(int i=0;i<N-6;i++)rbmks[i].active=0;init=1;ps("[RBN] Raphanus initialized\n");return 0;}
int rbn_planning(int t,int c,int a,int b,int d,int e,int y){return add(rbps,&st.n_rbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rbn_execution(int t,int c,int a,int b,int d,int e,int y){return add(rbss,&st.n_rbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rbn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rbvss,&st.n_rbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rbn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rbn_market(int t,int c,int a,int b,int d,int e,int y){return add(rbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rbn_report(void){ps("[RBN] Rbp: ");pi(st.n_rbp);ps(" PCS=");pi(st.t_f1);ps("\nRbe: ");pi(st.n_rbe);ps(" PCS=");pi(st.t_f2);ps("\nRbv: ");pi(st.n_rbv);ps(" PCS=");pi(st.t_f3);ps("\nRbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rbn_state(void){ps("[RBN] Rbp=");pi(st.n_rbp);ps(" Rbe=");pi(st.n_rbe);ps(" Rbv=");pi(st.n_rbv);ps(" Rbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Raphanus Admin Demo ===\n\n");rbn_init();
ps("Raphanus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rbn_planning(t,c,750+(i*17),739+(i*14),719+(i*10),701+(i*6),2020+(i%5));}
ps("\nRaphanus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rbn_execution(t,c,739+(i*15),728+(i*12),710+(i*8),697+(i*5),2021+(i%4));}
ps("\nRaphanus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rbn_evaluation(t,c,731+(i*13),720+(i*10),704+(i*7),693+(i*4),2022+(i%3));}
ps("\nRaphanus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbn_accessory(t,c,723+(i*11),714+(i*9),700+(i*6),690+(i*3),2023+(i%2));}
ps("\nRaphanus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbn_market(t,c,717+(i*9),708+(i*7),695+(i*5),687+(i*3),2024);}
ps("\n");rbn_report();rbn_state();ps("\n=== Demo Complete ===\n");return 0;}
