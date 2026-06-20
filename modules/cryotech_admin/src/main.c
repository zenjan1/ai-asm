/* cryotech_admin: Cryogenic technology administration (v1.0)
 * Cryogenic refrigeration, storage, transfer, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cry_t;
typedef struct{int n_rf,n_st,n_tr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cry_state_t;
static cry_t rfs[N],sts[N-2],trs[N-4],acs[N-6],mks[N-6]; static cry_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cry_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cry_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cry_init(void){if(init)return -1;st.n_rf=0;st.n_st=0;st.n_tr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rfs[i].active=0;for(int i=0;i<N-2;i++)sts[i].active=0;for(int i=0;i<N-4;i++)trs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CRY] Cryotech initialized\n");return 0;}
int cry_refrigeration(int t,int c,int a,int b,int d,int e,int y){return add(rfs,&st.n_rf,&st.t_f1,N,t,c,a,b,d,e,y);}
int cry_storage(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cry_transfer(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cry_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cry_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cry_report(void){ps("[CRY] Rf: ");pi(st.n_rf);ps(" PCS=");pi(st.t_f1);ps("\nSt: ");pi(st.n_st);ps(" PCS=");pi(st.t_f2);ps("\nTr: ");pi(st.n_tr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cry_state(void){ps("[CRY] Rf=");pi(st.n_rf);ps(" St=");pi(st.n_st);ps(" Tr=");pi(st.n_tr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cryogenic Tech Admin Demo ===\n\n");cry_init();
ps("Cryogenic refrigeration...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cry_refrigeration(t,c,310+(i*17),295+(i*14),275+(i*10),257+(i*6),2020+(i%5));}
ps("\nCryogenic storage...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cry_storage(t,c,299+(i*15),285+(i*12),267+(i*8),254+(i*5),2021+(i%4));}
ps("\nCryogenic transfer...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cry_transfer(t,c,291+(i*13),277+(i*10),261+(i*7),250+(i*4),2022+(i%3));}
ps("\nCryogenic accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cry_accessory(t,c,283+(i*11),271+(i*9),257+(i*6),247+(i*3),2023+(i%2));}
ps("\nCryogenic marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cry_market(t,c,277+(i*9),266+(i*7),253+(i*5),245+(i*3),2024);}
ps("\n");cry_report();cry_state();ps("\n=== Demo Complete ===\n");return 0;}
