/* supply_admin: Supply chain technology administration (v1.0)
 * Supplier management, supply chain collaboration, supply chain finance, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spx_t;
typedef struct{int n_sm,n_sc,n_fn,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spx_state_t;
static spx_t sms[N],scs[N-2],fns[N-4],acs[N-6],mks[N-6]; static spx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spx_init(void){if(init)return -1;st.n_sm=0;st.n_sc=0;st.n_fn=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sms[i].active=0;for(int i=0;i<N-2;i++)scs[i].active=0;for(int i=0;i<N-4;i++)fns[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SPX] Supply initialized\n");return 0;}
int spx_supplier(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f1,N,t,c,a,b,d,e,y);}
int spx_collaboration(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spx_finance(int t,int c,int a,int b,int d,int e,int y){return add(fns,&st.n_fn,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spx_report(void){ps("[SPX] Sm: ");pi(st.n_sm);ps(" PCS=");pi(st.t_f1);ps("\nSc: ");pi(st.n_sc);ps(" PCS=");pi(st.t_f2);ps("\nFn: ");pi(st.n_fn);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void spx_state(void){ps("[SPX] Sm=");pi(st.n_sm);ps(" Sc=");pi(st.n_sc);ps(" Fn=");pi(st.n_fn);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Supply Admin Demo ===\n\n");spx_init();
ps("Supplier management...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spx_supplier(t,c,436+(i*17),425+(i*14),405+(i*10),387+(i*6),2020+(i%5));}
ps("\nSupply chain collaboration...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spx_collaboration(t,c,425+(i*15),414+(i*12),396+(i*8),383+(i*5),2021+(i%4));}
ps("\nSupply chain finance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spx_finance(t,c,417+(i*13),406+(i*10),390+(i*7),379+(i*4),2022+(i%3));}
ps("\nSupply chain accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spx_accessory(t,c,409+(i*11),400+(i*9),386+(i*6),376+(i*3),2023+(i%2));}
ps("\nSupply chain marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spx_market(t,c,403+(i*9),394+(i*7),381+(i*5),373+(i*3),2024);}
ps("\n");spx_report();spx_state();ps("\n=== Demo Complete ===\n");return 0;}
