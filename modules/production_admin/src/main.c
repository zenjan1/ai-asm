/* production_admin: Production technology administration (v1.0)
 * Production planning, production control, lean production, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} prx_t;
typedef struct{int n_pl,n_ct,n_ln,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} prx_state_t;
static prx_t pls[N],cts[N-2],lns[N-4],acs[N-6],mks[N-6]; static prx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(prx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;prx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PRX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int prx_init(void){if(init)return -1;st.n_pl=0;st.n_ct=0;st.n_ln=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pls[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)lns[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PRX] Production initialized\n");return 0;}
int prx_planning(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f1,N,t,c,a,b,d,e,y);}
int prx_control(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int prx_lean(int t,int c,int a,int b,int d,int e,int y){return add(lns,&st.n_ln,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int prx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int prx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void prx_report(void){ps("[PRX] Pl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f1);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f2);ps("\nLn: ");pi(st.n_ln);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void prx_state(void){ps("[PRX] Pl=");pi(st.n_pl);ps(" Ct=");pi(st.n_ct);ps(" Ln=");pi(st.n_ln);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Production Admin Demo ===\n\n");prx_init();
ps("Production planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;prx_planning(t,c,429+(i*17),418+(i*14),398+(i*10),380+(i*6),2020+(i%5));}
ps("\nProduction control...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;prx_control(t,c,418+(i*15),407+(i*12),389+(i*8),376+(i*5),2021+(i%4));}
ps("\nLean production...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;prx_lean(t,c,410+(i*13),399+(i*10),383+(i*7),372+(i*4),2022+(i%3));}
ps("\nProduction accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prx_accessory(t,c,402+(i*11),393+(i*9),379+(i*6),369+(i*3),2023+(i%2));}
ps("\nProduction marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prx_market(t,c,396+(i*9),387+(i*7),374+(i*5),366+(i*3),2024);}
ps("\n");prx_report();prx_state();ps("\n=== Demo Complete ===\n");return 0;}
