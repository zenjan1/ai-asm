/* pennywort_admin: Pennywort management technology administration (v1.0)
 * Pennywort planning, pennywort execution, pennywort evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pw_t;
typedef struct{int n_pwp,n_pwe,n_pwv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pw_state_t;
static pw_t pwp[N],pwss[N-2],pwvss[N-4],pwas[N-6],pwmks[N-6]; static pw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pw_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pw_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PNS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pw_init(void){if(init)return -1;st.n_pwp=0;st.n_pwe=0;st.n_pwv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pwp[i].active=0;for(int i=0;i<N-2;i++)pwss[i].active=0;for(int i=0;i<N-4;i++)pwvss[i].active=0;for(int i=0;i<N-6;i++)pwas[i].active=0;for(int i=0;i<N-6;i++)pwmks[i].active=0;init=1;ps("[PNS] Pennywort initialized\n");return 0;}
int pw_planning(int t,int c,int a,int b,int d,int e,int y){return add(pwp,&st.n_pwp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pw_execution(int t,int c,int a,int b,int d,int e,int y){return add(pwss,&st.n_pwe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pw_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pwvss,&st.n_pwv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pw_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pwas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pw_market(int t,int c,int a,int b,int d,int e,int y){return add(pwmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pw_report(void){ps("[PNS] Pwp: ");pi(st.n_pwp);ps(" PCS=");pi(st.t_f1);ps("\nPwe: ");pi(st.n_pwe);ps(" PCS=");pi(st.t_f2);ps("\nPwv: ");pi(st.n_pwv);ps(" PCS=");pi(st.t_f3);ps("\nPwc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pw_state(void){ps("[PNS] Pwp=");pi(st.n_pwp);ps(" Pwe=");pi(st.n_pwe);ps(" Pwv=");pi(st.n_pwv);ps(" Pwc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pennywort Admin Demo ===\n\n");pw_init();
ps("Pennywort planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pw_planning(t,c,697+(i*17),686+(i*14),666+(i*10),648+(i*6),2020+(i%5));}
ps("\nPennywort execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pw_execution(t,c,686+(i*15),675+(i*12),657+(i*8),644+(i*5),2021+(i%4));}
ps("\nPennywort evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pw_evaluation(t,c,678+(i*13),667+(i*10),651+(i*7),640+(i*4),2022+(i%3));}
ps("\nPennywort accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pw_accessory(t,c,670+(i*11),661+(i*9),647+(i*6),637+(i*3),2023+(i%2));}
ps("\nPennywort marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pw_market(t,c,664+(i*9),655+(i*7),642+(i*5),634+(i*3),2024);}
ps("\n");pw_report();pw_state();ps("\n=== Demo Complete ===\n");return 0;}
