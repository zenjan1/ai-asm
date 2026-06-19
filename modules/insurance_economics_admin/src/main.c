/* insurance_economics_admin: Insurance economics administration (v1.0)
 * Insurance demand, insurance supply, insurance contracts, insurance types, insurance regulation
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ins_t;
typedef struct{int n_id,n_is,n_ic,n_it,n_ir,t_f1,t_f2,t_f3,t_f4,t_f5;} ins_state_t;
static ins_t ids[N],iss[N-2],ics[N-4],its[N-6],irs[N-6]; static ins_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ins_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ins_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[INS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ins_init(void){if(init)return -1;st.n_id=0;st.n_is=0;st.n_ic=0;st.n_it=0;st.n_ir=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ids[i].active=0;for(int i=0;i<N-2;i++)iss[i].active=0;for(int i=0;i<N-4;i++)ics[i].active=0;for(int i=0;i<N-6;i++)its[i].active=0;for(int i=0;i<N-6;i++)irs[i].active=0;init=1;ps("[INS] Insurance economics initialized\n");return 0;}
int ins_demand(int t,int c,int a,int b,int d,int e,int y){return add(ids,&st.n_id,&st.t_f1,N,t,c,a,b,d,e,y);}
int ins_supply(int t,int c,int a,int b,int d,int e,int y){return add(iss,&st.n_is,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ins_contract(int t,int c,int a,int b,int d,int e,int y){return add(ics,&st.n_ic,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ins_type(int t,int c,int a,int b,int d,int e,int y){return add(its,&st.n_it,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ins_regulate(int t,int c,int a,int b,int d,int e,int y){return add(irs,&st.n_ir,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ins_report(void){ps("[INS] Demand: ");pi(st.n_id);ps(" Averse=");pi(st.t_f1);ps("\nSupply: ");pi(st.n_is);ps(" Premium=");pi(st.t_f2);ps("\nContract: ");pi(st.n_ic);ps(" Utmost=");pi(st.t_f3);ps("\nType: ");pi(st.n_it);ps(" Life=");pi(st.t_f4);ps("\nRegul: ");pi(st.n_ir);ps(" Solv=");pi(st.t_f5);ps("\n");}
void ins_state(void){ps("[INS] Id=");pi(st.n_id);ps(" Is=");pi(st.n_is);ps(" Ic=");pi(st.n_ic);ps(" It=");pi(st.n_it);ps(" Ir=");pi(st.n_ir);ps("\n");}
int main(void){
ps("=== Insurance Economics Admin Demo ===\n\n");ins_init();
ps("Insurance demand...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ins_demand(t,c,110+(i*17),95+(i*14),75+(i*10),58+(i*6),2020+(i%5));}
ps("\nInsurance supply...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ins_supply(t,c,99+(i*15),85+(i*12),67+(i*8),54+(i*5),2021+(i%4));}
ps("\nInsurance contracts...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ins_contract(t,c,91+(i*13),77+(i*10),61+(i*7),50+(i*4),2022+(i%3));}
ps("\nInsurance types...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ins_type(t,c,83+(i*11),71+(i*9),57+(i*6),47+(i*3),2023+(i%2));}
ps("\nInsurance regulation...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ins_regulate(t,c,77+(i*9),66+(i*7),53+(i*5),45+(i*3),2024);}
ps("\n");ins_report();ins_state();ps("\n=== Demo Complete ===\n");return 0;}
