/* safetytech_admin: Safety technology administration (v1.0)
 * Personal protection, production safety, environmental safety, emergency rescue, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} saf_t;
typedef struct{int n_pp,n_ps,n_es,n_er,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} saf_state_t;
static saf_t pps[N],pss[N-2],ess[N-4],ers[N-6],mks[N-6]; static saf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(saf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;saf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SAF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int saf_init(void){if(init)return -1;st.n_pp=0;st.n_ps=0;st.n_es=0;st.n_er=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pps[i].active=0;for(int i=0;i<N-2;i++)pss[i].active=0;for(int i=0;i<N-4;i++)ess[i].active=0;for(int i=0;i<N-6;i++)ers[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SAF] Safetytech initialized\n");return 0;}
int saf_personal(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f1,N,t,c,a,b,d,e,y);}
int saf_production(int t,int c,int a,int b,int d,int e,int y){return add(pss,&st.n_ps,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int saf_environment(int t,int c,int a,int b,int d,int e,int y){return add(ess,&st.n_es,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int saf_emergency(int t,int c,int a,int b,int d,int e,int y){return add(ers,&st.n_er,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int saf_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void saf_report(void){ps("[SAF] Pp: ");pi(st.n_pp);ps(" PCS=");pi(st.t_f1);ps("\nPs: ");pi(st.n_ps);ps(" PCS=");pi(st.t_f2);ps("\nEs: ");pi(st.n_es);ps(" PCS=");pi(st.t_f3);ps("\nEr: ");pi(st.n_er);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void saf_state(void){ps("[SAF] Pp=");pi(st.n_pp);ps(" Ps=");pi(st.n_ps);ps(" Es=");pi(st.n_es);ps(" Er=");pi(st.n_er);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Safety Tech Admin Demo ===\n\n");saf_init();
ps("Personal protection...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;saf_personal(t,c,249+(i*17),234+(i*14),214+(i*10),196+(i*6),2020+(i%5));}
ps("\nProduction safety...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;saf_production(t,c,238+(i*15),224+(i*12),206+(i*8),193+(i*5),2021+(i%4));}
ps("\nEnvironmental safety...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;saf_environment(t,c,230+(i*13),216+(i*10),200+(i*7),189+(i*4),2022+(i%3));}
ps("\nEmergency rescue...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;saf_emergency(t,c,222+(i*11),210+(i*9),196+(i*6),186+(i*3),2023+(i%2));}
ps("\nSafety marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;saf_market(t,c,216+(i*9),205+(i*7),192+(i*5),184+(i*3),2024);}
ps("\n");saf_report();saf_state();ps("\n=== Demo Complete ===\n");return 0;}
