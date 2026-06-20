/* inventory_admin: Inventory technology administration (v1.0)
 * Inventory planning, inventory control, inventory optimization, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ivt_t;
typedef struct{int n_pl,n_ct,n_op,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ivt_state_t;
static ivt_t pls[N],cts[N-2],ops[N-4],acs[N-6],mks[N-6]; static ivt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ivt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ivt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IVT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ivt_init(void){if(init)return -1;st.n_pl=0;st.n_ct=0;st.n_op=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pls[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)ops[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[IVT] Inventory initialized\n");return 0;}
int ivt_planning(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f1,N,t,c,a,b,d,e,y);}
int ivt_control(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ivt_optimization(int t,int c,int a,int b,int d,int e,int y){return add(ops,&st.n_op,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ivt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ivt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ivt_report(void){ps("[IVT] Pl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f1);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f2);ps("\nOp: ");pi(st.n_op);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ivt_state(void){ps("[IVT] Pl=");pi(st.n_pl);ps(" Ct=");pi(st.n_ct);ps(" Op=");pi(st.n_op);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Inventory Admin Demo ===\n\n");ivt_init();
ps("Inventory planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ivt_planning(t,c,432+(i*17),421+(i*14),401+(i*10),383+(i*6),2020+(i%5));}
ps("\nInventory control...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ivt_control(t,c,421+(i*15),410+(i*12),392+(i*8),379+(i*5),2021+(i%4));}
ps("\nInventory optimization...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ivt_optimization(t,c,413+(i*13),402+(i*10),386+(i*7),375+(i*4),2022+(i%3));}
ps("\nInventory accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ivt_accessory(t,c,405+(i*11),396+(i*9),382+(i*6),372+(i*3),2023+(i%2));}
ps("\nInventory marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ivt_market(t,c,399+(i*9),390+(i*7),377+(i*5),369+(i*3),2024);}
ps("\n");ivt_report();ivt_state();ps("\n=== Demo Complete ===\n");return 0;}
