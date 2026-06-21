/* ugni_admin: Ugni management technology administration (v1.0)
 * Ugni planning, ugni execution, ugni evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ugn_t;
typedef struct{int n_ugnp,n_ugne,n_ugn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ugn_state_t;
static ugn_t ugnps[N],ugnss[N-2],ugnvss[N-4],ugnas[N-6],ugnmks[N-6]; static ugn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ugn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ugn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[UGN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ugn_init(void){if(init)return -1;st.n_ugnp=0;st.n_ugne=0;st.n_ugn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ugnps[i].active=0;for(int i=0;i<N-2;i++)ugnss[i].active=0;for(int i=0;i<N-4;i++)ugnvss[i].active=0;for(int i=0;i<N-6;i++)ugnas[i].active=0;for(int i=0;i<N-6;i++)ugnmks[i].active=0;init=1;ps("[UGN] Ugni initialized\n");return 0;}
int ugn_planning(int t,int c,int a,int b,int d,int e,int y){return add(ugnps,&st.n_ugnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ugn_execution(int t,int c,int a,int b,int d,int e,int y){return add(ugnss,&st.n_ugne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ugn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ugnvss,&st.n_ugn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ugn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ugnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ugn_market(int t,int c,int a,int b,int d,int e,int y){return add(ugnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ugn_report(void){ps("[UGN] Ugnp: ");pi(st.n_ugnp);ps(" PCS=");pi(st.t_f1);ps("\nUgne: ");pi(st.n_ugne);ps(" PCS=");pi(st.t_f2);ps("\nUgnv: ");pi(st.n_ugn2);ps(" PCS=");pi(st.t_f3);ps("\nUgnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ugn_state(void){ps("[UGN] Ugnp=");pi(st.n_ugnp);ps(" Ugne=");pi(st.n_ugne);ps(" Ugnv=");pi(st.n_ugn2);ps(" Ugnc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ugni Admin Demo ===\n\n");ugn_init();
ps("Ugni planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ugn_planning(t,c,954+(i*17),943+(i*14),923+(i*10),905+(i*6),2020+(i%5));}
ps("\nUgni execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ugn_execution(t,c,943+(i*15),932+(i*12),914+(i*8),901+(i*5),2021+(i%4));}
ps("\nUgni evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ugn_evaluation(t,c,935+(i*13),924+(i*10),908+(i*7),897+(i*4),2022+(i%3));}
ps("\nUgni accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ugn_accessory(t,c,927+(i*11),918+(i*9),904+(i*6),894+(i*3),2023+(i%2));}
ps("\nUgni marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ugn_market(t,c,921+(i*9),912+(i*7),899+(i*5),891+(i*3),2024);}
ps("\n");ugn_report();ugn_state();ps("\n=== Demo Complete ===\n");return 0;}
