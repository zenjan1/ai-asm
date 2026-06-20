/* thread_admin: Thread machining technology administration (v1.0)
 * Thread turning, thread milling, thread rolling, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} trd_t;
typedef struct{int n_tn,n_ml,n_rl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} trd_state_t;
static trd_t tns[N],mls[N-2],rls[N-4],acs[N-6],mks[N-6]; static trd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(trd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;trd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int trd_init(void){if(init)return -1;st.n_tn=0;st.n_ml=0;st.n_rl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tns[i].active=0;for(int i=0;i<N-2;i++)mls[i].active=0;for(int i=0;i<N-4;i++)rls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TRD] Thread initialized\n");return 0;}
int trd_turning(int t,int c,int a,int b,int d,int e,int y){return add(tns,&st.n_tn,&st.t_f1,N,t,c,a,b,d,e,y);}
int trd_milling(int t,int c,int a,int b,int d,int e,int y){return add(mls,&st.n_ml,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int trd_rolling(int t,int c,int a,int b,int d,int e,int y){return add(rls,&st.n_rl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int trd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int trd_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void trd_report(void){ps("[TRD] Tn: ");pi(st.n_tn);ps(" PCS=");pi(st.t_f1);ps("\nMl: ");pi(st.n_ml);ps(" PCS=");pi(st.t_f2);ps("\nRl: ");pi(st.n_rl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void trd_state(void){ps("[TRD] Tn=");pi(st.n_tn);ps(" Ml=");pi(st.n_ml);ps(" Rl=");pi(st.n_rl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Thread Admin Demo ===\n\n");trd_init();
ps("Thread turning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;trd_turning(t,c,406+(i*17),391+(i*14),371+(i*10),353+(i*6),2020+(i%5));}
ps("\nThread milling...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;trd_milling(t,c,395+(i*15),381+(i*12),363+(i*8),350+(i*5),2021+(i%4));}
ps("\nThread rolling...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;trd_rolling(t,c,387+(i*13),373+(i*10),357+(i*7),346+(i*4),2022+(i%3));}
ps("\nThread accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trd_accessory(t,c,379+(i*11),367+(i*9),353+(i*6),343+(i*3),2023+(i%2));}
ps("\nThread marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trd_market(t,c,373+(i*9),362+(i*7),349+(i*5),341+(i*3),2024);}
ps("\n");trd_report();trd_state();ps("\n=== Demo Complete ===\n");return 0;}
