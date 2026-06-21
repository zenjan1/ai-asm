/* agapanthus_admin: Agapanthus management technology administration (v1.0)
 * Agapanthus planning, agapanthus execution, agapanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aga_t;
typedef struct{int n_agap,n_agae,n_aga2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aga_state_t;
static aga_t agaps[N],agass[N-2],agavss[N-4],agaas[N-6],agamks[N-6]; static aga_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aga_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aga_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AGA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aga_init(void){if(init)return -1;st.n_agap=0;st.n_agae=0;st.n_aga2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)agaps[i].active=0;for(int i=0;i<N-2;i++)agass[i].active=0;for(int i=0;i<N-4;i++)agavss[i].active=0;for(int i=0;i<N-6;i++)agaas[i].active=0;for(int i=0;i<N-6;i++)agamks[i].active=0;init=1;ps("[AGA] Agapanthus initialized\n");return 0;}
int aga_planning(int t,int c,int a,int b,int d,int e,int y){return add(agaps,&st.n_agap,&st.t_f1,N,t,c,a,b,d,e,y);}
int aga_execution(int t,int c,int a,int b,int d,int e,int y){return add(agass,&st.n_agae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aga_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(agavss,&st.n_aga2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aga_accessory(int t,int c,int a,int b,int d,int e,int y){return add(agaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aga_market(int t,int c,int a,int b,int d,int e,int y){return add(agamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aga_report(void){ps("[AGA] Agap: ");pi(st.n_agap);ps(" PCS=");pi(st.t_f1);ps("\nAgae: ");pi(st.n_agae);ps(" PCS=");pi(st.t_f2);ps("\nAgav: ");pi(st.n_aga2);ps(" PCS=");pi(st.t_f3);ps("\nAgac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aga_state(void){ps("[AGA] Agap=");pi(st.n_agap);ps(" Agae=");pi(st.n_agae);ps(" Agav=");pi(st.n_aga2);ps(" Agac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Agapanthus Admin Demo ===\n\n");aga_init();
ps("Agapanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aga_planning(t,c,969+(i*17),958+(i*14),938+(i*10),920+(i*6),2020+(i%5));}
ps("\nAgapanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aga_execution(t,c,958+(i*15),947+(i*12),929+(i*8),916+(i*5),2021+(i%4));}
ps("\nAgapanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aga_evaluation(t,c,950+(i*13),939+(i*10),923+(i*7),912+(i*4),2022+(i%3));}
ps("\nAgapanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aga_accessory(t,c,942+(i*11),933+(i*9),919+(i*6),909+(i*3),2023+(i%2));}
ps("\nAgapanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aga_market(t,c,936+(i*9),927+(i*7),914+(i*5),906+(i*3),2024);}
ps("\n");aga_report();aga_state();ps("\n=== Demo Complete ===\n");return 0;}
