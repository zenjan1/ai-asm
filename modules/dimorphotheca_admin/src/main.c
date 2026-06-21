/* dimorphotheca_admin: Dimorphotheca management technology administration (v1.0)
 * Dimorphotheca planning, dimorphotheca execution, dimorphotheca evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dim_t;
typedef struct{int n_dimp,n_dime,n_dim2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dim_state_t;
static dim_t dimps[N],dimss[N-2],dimvss[N-4],dimas[N-6],dimmks[N-6]; static dim_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dim_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dim_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DIM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dim_init(void){if(init)return -1;st.n_dimp=0;st.n_dime=0;st.n_dim2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dimps[i].active=0;for(int i=0;i<N-2;i++)dimss[i].active=0;for(int i=0;i<N-4;i++)dimvss[i].active=0;for(int i=0;i<N-6;i++)dimas[i].active=0;for(int i=0;i<N-6;i++)dimmks[i].active=0;init=1;ps("[DIM] Dimorphotheca initialized\n");return 0;}
int dim_planning(int t,int c,int a,int b,int d,int e,int y){return add(dimps,&st.n_dimp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dim_execution(int t,int c,int a,int b,int d,int e,int y){return add(dimss,&st.n_dime,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dim_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dimvss,&st.n_dim2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dim_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dimas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dim_market(int t,int c,int a,int b,int d,int e,int y){return add(dimmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dim_report(void){ps("[DIM] Dimp: ");pi(st.n_dimp);ps(" PCS=");pi(st.t_f1);ps("\nDime: ");pi(st.n_dime);ps(" PCS=");pi(st.t_f2);ps("\nDimv: ");pi(st.n_dim2);ps(" PCS=");pi(st.t_f3);ps("\nDimc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dim_state(void){ps("[DIM] Dimp=");pi(st.n_dimp);ps(" Dime=");pi(st.n_dime);ps(" Dimv=");pi(st.n_dim2);ps(" Dimc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dimorphotheca Admin Demo ===\n\n");dim_init();
ps("Dimorphotheca planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dim_planning(t,c,993+(i*17),982+(i*14),962+(i*10),944+(i*6),2020+(i%5));}
ps("\nDimorphotheca execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dim_execution(t,c,982+(i*15),971+(i*12),953+(i*8),940+(i*5),2021+(i%4));}
ps("\nDimorphotheca evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dim_evaluation(t,c,974+(i*13),963+(i*10),947+(i*7),936+(i*4),2022+(i%3));}
ps("\nDimorphotheca accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dim_accessory(t,c,966+(i*11),957+(i*9),943+(i*6),933+(i*3),2023+(i%2));}
ps("\nDimorphotheca marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dim_market(t,c,960+(i*9),951+(i*7),938+(i*5),930+(i*3),2024);}
ps("\n");dim_report();dim_state();ps("\n=== Demo Complete ===\n");return 0;}
