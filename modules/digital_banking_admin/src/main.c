/* digital_banking_admin: Digital banking administration (v1.0)
 * Mobile banking, online banking, open banking, digital currency, smart risk control
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dba_t;
typedef struct{int n_mb,n_ob,n_opb,n_dc,n_src,t_f1,t_f2,t_f3,t_f4,t_f5;} dba_state_t;
static dba_t mbs[N],obs[N-2],opbs[N-4],dcs[N-6],srcs[N-6]; static dba_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dba_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dba_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DBA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dba_init(void){if(init)return -1;st.n_mb=0;st.n_ob=0;st.n_opb=0;st.n_dc=0;st.n_src=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mbs[i].active=0;for(int i=0;i<N-2;i++)obs[i].active=0;for(int i=0;i<N-4;i++)opbs[i].active=0;for(int i=0;i<N-6;i++)dcs[i].active=0;for(int i=0;i<N-6;i++)srcs[i].active=0;init=1;ps("[DBA] Digital banking initialized\n");return 0;}
int dba_mobile(int t,int c,int a,int b,int d,int e,int y){return add(mbs,&st.n_mb,&st.t_f1,N,t,c,a,b,d,e,y);}
int dba_online(int t,int c,int a,int b,int d,int e,int y){return add(obs,&st.n_ob,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dba_open(int t,int c,int a,int b,int d,int e,int y){return add(opbs,&st.n_opb,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dba_currency(int t,int c,int a,int b,int d,int e,int y){return add(dcs,&st.n_dc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dba_risk(int t,int c,int a,int b,int d,int e,int y){return add(srcs,&st.n_src,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dba_report(void){ps("[DBA] Mobile: ");pi(st.n_mb);ps(" App=");pi(st.t_f1);ps("\nOnline: ");pi(st.n_ob);ps(" Web=");pi(st.t_f2);ps("\nOpen: ");pi(st.n_opb);ps(" API=");pi(st.t_f3);ps("\nDC: ");pi(st.n_dc);ps(" CBDC=");pi(st.t_f4);ps("\nRisk: ");pi(st.n_src);ps(" AI=");pi(st.t_f5);ps("\n");}
void dba_state(void){ps("[DBA] Mb=");pi(st.n_mb);ps(" Ob=");pi(st.n_ob);ps(" Opb=");pi(st.n_opb);ps(" Dc=");pi(st.n_dc);ps(" Src=");pi(st.n_src);ps("\n");}
int main(void){
ps("=== Digital Banking Admin Demo ===\n\n");dba_init();
ps("Mobile banking...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dba_mobile(t,c,130+(i*17),115+(i*14),95+(i*10),78+(i*6),2020+(i%5));}
ps("\nOnline banking...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dba_online(t,c,119+(i*15),105+(i*12),87+(i*8),74+(i*5),2021+(i%4));}
ps("\nOpen banking...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dba_open(t,c,111+(i*13),97+(i*10),81+(i*7),70+(i*4),2022+(i%3));}
ps("\nDigital currency...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dba_currency(t,c,103+(i*11),91+(i*9),77+(i*6),67+(i*3),2023+(i%2));}
ps("\nSmart risk control...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dba_risk(t,c,97+(i*9),86+(i*7),73+(i*5),65+(i*3),2024);}
ps("\n");dba_report();dba_state();ps("\n=== Demo Complete ===\n");return 0;}
