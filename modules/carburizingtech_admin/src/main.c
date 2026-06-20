/* carburizingtech_admin: Carburizing technology administration (v1.0)
 * Gas carburizing, liquid carburizing, solid carburizing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} crb_t;
typedef struct{int n_gs,n_lq,n_so,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} crb_state_t;
static crb_t gss[N],lqs[N-2],sos[N-4],acs[N-6],mks[N-6]; static crb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;crb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int crb_init(void){if(init)return -1;st.n_gs=0;st.n_lq=0;st.n_so=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gss[i].active=0;for(int i=0;i<N-2;i++)lqs[i].active=0;for(int i=0;i<N-4;i++)sos[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CRB] Carburizingtech initialized\n");return 0;}
int crb_gas(int t,int c,int a,int b,int d,int e,int y){return add(gss,&st.n_gs,&st.t_f1,N,t,c,a,b,d,e,y);}
int crb_liquid(int t,int c,int a,int b,int d,int e,int y){return add(lqs,&st.n_lq,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int crb_solid(int t,int c,int a,int b,int d,int e,int y){return add(sos,&st.n_so,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int crb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int crb_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void crb_report(void){ps("[CRB] Gs: ");pi(st.n_gs);ps(" PCS=");pi(st.t_f1);ps("\nLq: ");pi(st.n_lq);ps(" PCS=");pi(st.t_f2);ps("\nSo: ");pi(st.n_so);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void crb_state(void){ps("[CRB] Gs=");pi(st.n_gs);ps(" Lq=");pi(st.n_lq);ps(" So=");pi(st.n_so);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Carburizing Tech Admin Demo ===\n\n");crb_init();
ps("Gas carburizing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;crb_gas(t,c,329+(i*17),314+(i*14),294+(i*10),276+(i*6),2020+(i%5));}
ps("\nLiquid carburizing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;crb_liquid(t,c,318+(i*15),304+(i*12),286+(i*8),273+(i*5),2021+(i%4));}
ps("\nSolid carburizing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;crb_solid(t,c,310+(i*13),296+(i*10),280+(i*7),269+(i*4),2022+(i%3));}
ps("\nCarburizing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crb_accessory(t,c,302+(i*11),290+(i*9),276+(i*6),266+(i*3),2023+(i%2));}
ps("\nCarburizing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crb_market(t,c,296+(i*9),285+(i*7),272+(i*5),264+(i*3),2024);}
ps("\n");crb_report();crb_state();ps("\n=== Demo Complete ===\n");return 0;}
