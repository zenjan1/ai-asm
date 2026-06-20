/* turbine_admin: Turbine technology administration (v1.0)
 * Steam turbine, gas turbine, hydraulic turbine, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} trb_t;
typedef struct{int n_st,n_gt,n_ht,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} trb_state_t;
static trb_t sts[N],gts[N-2],hts[N-4],acs[N-6],mks[N-6]; static trb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(trb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;trb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int trb_init(void){if(init)return -1;st.n_st=0;st.n_gt=0;st.n_ht=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sts[i].active=0;for(int i=0;i<N-2;i++)gts[i].active=0;for(int i=0;i<N-4;i++)hts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TRB] Turbine initialized\n");return 0;}
int trb_steam(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f1,N,t,c,a,b,d,e,y);}
int trb_gas(int t,int c,int a,int b,int d,int e,int y){return add(gts,&st.n_gt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int trb_hydraulic(int t,int c,int a,int b,int d,int e,int y){return add(hts,&st.n_ht,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int trb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int trb_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void trb_report(void){ps("[TRB] St: ");pi(st.n_st);ps(" PCS=");pi(st.t_f1);ps("\nGt: ");pi(st.n_gt);ps(" PCS=");pi(st.t_f2);ps("\nHt: ");pi(st.n_ht);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void trb_state(void){ps("[TRB] St=");pi(st.n_st);ps(" Gt=");pi(st.n_gt);ps(" Ht=");pi(st.n_ht);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Turbine Admin Demo ===\n\n");trb_init();
ps("Steam turbines...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;trb_steam(t,c,374+(i*17),359+(i*14),339+(i*10),321+(i*6),2020+(i%5));}
ps("\nGas turbines...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;trb_gas(t,c,363+(i*15),349+(i*12),331+(i*8),318+(i*5),2021+(i%4));}
ps("\nHydraulic turbines...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;trb_hydraulic(t,c,355+(i*13),341+(i*10),325+(i*7),314+(i*4),2022+(i%3));}
ps("\nTurbine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trb_accessory(t,c,347+(i*11),335+(i*9),321+(i*6),311+(i*3),2023+(i%2));}
ps("\nTurbine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trb_market(t,c,341+(i*9),330+(i*7),317+(i*5),309+(i*3),2024);}
ps("\n");trb_report();trb_state();ps("\n=== Demo Complete ===\n");return 0;}
