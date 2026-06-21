/* monarda_admin: Monarda management technology administration (v1.0)
 * Monarda planning, monarda execution, monarda evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mon_t;
typedef struct{int n_monp,n_mone,n_mon2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mon_state_t;
static mon_t monps[N],monss[N-2],monvss[N-4],monac[N-6],monmks[N-6]; static mon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mon_init(void){if(init)return -1;st.n_monp=0;st.n_mone=0;st.n_mon2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)monps[i].active=0;for(int i=0;i<N-2;i++)monss[i].active=0;for(int i=0;i<N-4;i++)monvss[i].active=0;for(int i=0;i<N-6;i++)monac[i].active=0;for(int i=0;i<N-6;i++)monmks[i].active=0;init=1;ps("[MON] Monarda initialized\n");return 0;}
int mon_planning(int t,int c,int a,int b,int d,int e,int y){return add(monps,&st.n_monp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mon_execution(int t,int c,int a,int b,int d,int e,int y){return add(monss,&st.n_mone,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mon_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(monvss,&st.n_mon2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mon_accessory(int t,int c,int a,int b,int d,int e,int y){return add(monac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mon_market(int t,int c,int a,int b,int d,int e,int y){return add(monmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mon_report(void){ps("[MON] Monp: ");pi(st.n_monp);ps(" PCS=");pi(st.t_f1);ps("\nMone: ");pi(st.n_mone);ps(" PCS=");pi(st.t_f2);ps("\nMonv: ");pi(st.n_mon2);ps(" PCS=");pi(st.t_f3);ps("\nMonc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mon_state(void){ps("[MON] Monp=");pi(st.n_monp);ps(" Mone=");pi(st.n_mone);ps(" Monv=");pi(st.n_mon2);ps(" Monc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Monarda Admin Demo ===\n\n");mon_init();
ps("Monarda planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mon_planning(t,c,941+(i*17),930+(i*14),910+(i*10),892+(i*6),2020+(i%5));}
ps("\nMonarda execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mon_execution(t,c,930+(i*15),919+(i*12),901+(i*8),888+(i*5),2021+(i%4));}
ps("\nMonarda evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mon_evaluation(t,c,922+(i*13),911+(i*10),895+(i*7),884+(i*4),2022+(i%3));}
ps("\nMonarda accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mon_accessory(t,c,914+(i*11),905+(i*9),891+(i*6),881+(i*3),2023+(i%2));}
ps("\nMonarda marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mon_market(t,c,908+(i*9),899+(i*7),886+(i*5),878+(i*3),2024);}
ps("\n");mon_report();mon_state();ps("\n=== Demo Complete ===\n");return 0;}
