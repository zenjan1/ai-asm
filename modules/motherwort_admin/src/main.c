/* motherwort_admin: Motherwort management technology administration (v1.0)
 * Motherwort planning, motherwort execution, motherwort evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mo_t;
typedef struct{int n_mtp,n_mte,n_mtv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mo_state_t;
static mo_t mtps[N],mtss[N-2],mtvss[N-4],mtas[N-6],mtmks[N-6]; static mo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MTH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mt_init(void){if(init)return -1;st.n_mtp=0;st.n_mte=0;st.n_mtv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mtps[i].active=0;for(int i=0;i<N-2;i++)mtss[i].active=0;for(int i=0;i<N-4;i++)mtvss[i].active=0;for(int i=0;i<N-6;i++)mtas[i].active=0;for(int i=0;i<N-6;i++)mtmks[i].active=0;init=1;ps("[MTH] Motherwort initialized\n");return 0;}
int mt_planning(int t,int c,int a,int b,int d,int e,int y){return add(mtps,&st.n_mtp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mt_execution(int t,int c,int a,int b,int d,int e,int y){return add(mtss,&st.n_mte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mtvss,&st.n_mtv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mt_market(int t,int c,int a,int b,int d,int e,int y){return add(mtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mt_report(void){ps("[MTH] Mtp: ");pi(st.n_mtp);ps(" PCS=");pi(st.t_f1);ps("\nMte: ");pi(st.n_mte);ps(" PCS=");pi(st.t_f2);ps("\nMtv: ");pi(st.n_mtv);ps(" PCS=");pi(st.t_f3);ps("\nMtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mt_state(void){ps("[MTH] Mtp=");pi(st.n_mtp);ps(" Mte=");pi(st.n_mte);ps(" Mtv=");pi(st.n_mtv);ps(" Mtc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Motherwort Admin Demo ===\n\n");mt_init();
ps("Motherwort planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mt_planning(t,c,706+(i*17),695+(i*14),675+(i*10),657+(i*6),2020+(i%5));}
ps("\nMotherwort execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mt_execution(t,c,695+(i*15),684+(i*12),666+(i*8),653+(i*5),2021+(i%4));}
ps("\nMotherwort evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mt_evaluation(t,c,687+(i*13),676+(i*10),660+(i*7),649+(i*4),2022+(i%3));}
ps("\nMotherwort accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mt_accessory(t,c,679+(i*11),670+(i*9),656+(i*6),646+(i*3),2023+(i%2));}
ps("\nMotherwort marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mt_market(t,c,673+(i*9),664+(i*7),651+(i*5),643+(i*3),2024);}
ps("\n");mt_report();mt_state();ps("\n=== Demo Complete ===\n");return 0;}
