/* legaltech_admin: Legaltech administration (v1.0)
 * Contract management, legal research, litigation management, compliance management, legal services
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lta_t;
typedef struct{int n_cm,n_lr,n_lt,n_cp,n_ls,t_f1,t_f2,t_f3,t_f4,t_f5;} lta_state_t;
static lta_t cms[N],lrs[N-2],lts[N-4],cps[N-6],lss[N-6]; static lta_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lta_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lta_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LTA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lta_init(void){if(init)return -1;st.n_cm=0;st.n_lr=0;st.n_lt=0;st.n_cp=0;st.n_ls=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cms[i].active=0;for(int i=0;i<N-2;i++)lrs[i].active=0;for(int i=0;i<N-4;i++)lts[i].active=0;for(int i=0;i<N-6;i++)cps[i].active=0;for(int i=0;i<N-6;i++)lss[i].active=0;init=1;ps("[LTA] Legaltech initialized\n");return 0;}
int lta_contract(int t,int c,int a,int b,int d,int e,int y){return add(cms,&st.n_cm,&st.t_f1,N,t,c,a,b,d,e,y);}
int lta_research(int t,int c,int a,int b,int d,int e,int y){return add(lrs,&st.n_lr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lta_litigation(int t,int c,int a,int b,int d,int e,int y){return add(lts,&st.n_lt,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lta_compliance(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lta_service(int t,int c,int a,int b,int d,int e,int y){return add(lss,&st.n_ls,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lta_report(void){ps("[LTA] Contract: ");pi(st.n_cm);ps(" CLM=");pi(st.t_f1);ps("\nRes: ");pi(st.n_lr);ps(" AI=");pi(st.t_f2);ps("\nLit: ");pi(st.n_lt);ps(" Case=");pi(st.t_f3);ps("\nComp: ");pi(st.n_cp);ps(" Reg=");pi(st.t_f4);ps("\nServ: ");pi(st.n_ls);ps(" Match=");pi(st.t_f5);ps("\n");}
void lta_state(void){ps("[LTA] Cm=");pi(st.n_cm);ps(" Lr=");pi(st.n_lr);ps(" Lt=");pi(st.n_lt);ps(" Cp=");pi(st.n_cp);ps(" Ls=");pi(st.n_ls);ps("\n");}
int main(void){
ps("=== Legaltech Admin Demo ===\n\n");lta_init();
ps("Contract management...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lta_contract(t,c,138+(i*17),123+(i*14),103+(i*10),86+(i*6),2020+(i%5));}
ps("\nLegal research...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lta_research(t,c,127+(i*15),113+(i*12),95+(i*8),82+(i*5),2021+(i%4));}
ps("\nLitigation management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lta_litigation(t,c,119+(i*13),105+(i*10),89+(i*7),78+(i*4),2022+(i%3));}
ps("\nCompliance management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lta_compliance(t,c,111+(i*11),99+(i*9),85+(i*6),75+(i*3),2023+(i%2));}
ps("\nLegal services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lta_service(t,c,105+(i*9),94+(i*7),81+(i*5),73+(i*3),2024);}
ps("\n");lta_report();lta_state();ps("\n=== Demo Complete ===\n");return 0;}
