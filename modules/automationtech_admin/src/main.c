/* automationtech_admin: Automation technology administration (v1.0)
 * Production line automation, industrial robots, automation control, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aut_t;
typedef struct{int n_pl,n_rb,n_ct,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aut_state_t;
static aut_t pls[N],rbs[N-2],cts[N-4],acs[N-6],mks[N-6]; static aut_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aut_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aut_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AUT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aut_init(void){if(init)return -1;st.n_pl=0;st.n_rb=0;st.n_ct=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pls[i].active=0;for(int i=0;i<N-2;i++)rbs[i].active=0;for(int i=0;i<N-4;i++)cts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[AUT] Automationtech initialized\n");return 0;}
int aut_line(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f1,N,t,c,a,b,d,e,y);}
int aut_robot(int t,int c,int a,int b,int d,int e,int y){return add(rbs,&st.n_rb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aut_control(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aut_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aut_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aut_report(void){ps("[AUT] Pl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f1);ps("\nRb: ");pi(st.n_rb);ps(" PCS=");pi(st.t_f2);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aut_state(void){ps("[AUT] Pl=");pi(st.n_pl);ps(" Rb=");pi(st.n_rb);ps(" Ct=");pi(st.n_ct);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Automation Tech Admin Demo ===\n\n");aut_init();
ps("Production line automation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aut_line(t,c,307+(i*17),292+(i*14),272+(i*10),254+(i*6),2020+(i%5));}
ps("\nIndustrial robots...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aut_robot(t,c,296+(i*15),282+(i*12),264+(i*8),251+(i*5),2021+(i%4));}
ps("\nAutomation control...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aut_control(t,c,288+(i*13),274+(i*10),258+(i*7),247+(i*4),2022+(i%3));}
ps("\nAutomation accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aut_accessory(t,c,280+(i*11),268+(i*9),254+(i*6),244+(i*3),2023+(i%2));}
ps("\nAutomation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aut_market(t,c,274+(i*9),263+(i*7),250+(i*5),242+(i*3),2024);}
ps("\n");aut_report();aut_state();ps("\n=== Demo Complete ===\n");return 0;}
