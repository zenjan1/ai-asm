/* buddleja_admin: Buddleja management technology administration (v1.0)
 * Buddleja planning, buddleja execution, buddleja evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bud_t;
typedef struct{int n_budp,n_bude,n_bud2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bud_state_t;
static bud_t budps[N],budss[N-2],budvss[N-4],bud_as[N-6],bud_mks[N-6]; static bud_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bud_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bud_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BUD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bud_init(void){if(init)return -1;st.n_budp=0;st.n_bude=0;st.n_bud2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)budps[i].active=0;for(int i=0;i<N-2;i++)budss[i].active=0;for(int i=0;i<N-4;i++)budvss[i].active=0;for(int i=0;i<N-6;i++)bud_as[i].active=0;for(int i=0;i<N-6;i++)bud_mks[i].active=0;init=1;ps("[BUD] Buddleja initialized\n");return 0;}
int bud_planning(int t,int c,int a,int b,int d,int e,int y){return add(budps,&st.n_budp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bud_execution(int t,int c,int a,int b,int d,int e,int y){return add(budss,&st.n_bude,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bud_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(budvss,&st.n_bud2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bud_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bud_as,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bud_market(int t,int c,int a,int b,int d,int e,int y){return add(bud_mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bud_report(void){ps("[BUD] Budp: ");pi(st.n_budp);ps(" PCS=");pi(st.t_f1);ps("\nBude: ");pi(st.n_bude);ps(" PCS=");pi(st.t_f2);ps("\nBudv: ");pi(st.n_bud2);ps(" PCS=");pi(st.t_f3);ps("\nBudc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bud_state(void){ps("[BUD] Budp=");pi(st.n_budp);ps(" Bude=");pi(st.n_bude);ps(" Budv=");pi(st.n_bud2);ps(" Budc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Buddleja Admin Demo ===\n\n");bud_init();
ps("Buddleja planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bud_planning(t,c,980+(i*17),969+(i*14),949+(i*10),931+(i*6),2020+(i%5));}
ps("\nBuddleja execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bud_execution(t,c,969+(i*15),958+(i*12),940+(i*8),927+(i*5),2021+(i%4));}
ps("\nBuddleja evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bud_evaluation(t,c,961+(i*13),950+(i*10),934+(i*7),923+(i*4),2022+(i%3));}
ps("\nBuddleja accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bud_accessory(t,c,953+(i*11),944+(i*9),930+(i*6),920+(i*3),2023+(i%2));}
ps("\nBuddleja marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bud_market(t,c,947+(i*9),938+(i*7),925+(i*5),917+(i*3),2024);}
ps("\n");bud_report();bud_state();ps("\n=== Demo Complete ===\n");return 0;}
