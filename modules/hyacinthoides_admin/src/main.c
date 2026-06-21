/* hyacinthoides_admin: Hyacinthoides management technology administration (v1.0)
 * Hyacinthoides planning, hyacinthoides execution, hyacinthoides evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ha_t;
typedef struct{int n_hyp,n_hye,n_hyv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ha_state_t;
static ha_t hyps[N],hyss[N-2],hyvss[N-4],hyas[N-6],hymks[N-6]; static ha_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ha_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ha_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hyc_init(void){if(init)return -1;st.n_hyp=0;st.n_hye=0;st.n_hyv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hyps[i].active=0;for(int i=0;i<N-2;i++)hyss[i].active=0;for(int i=0;i<N-4;i++)hyvss[i].active=0;for(int i=0;i<N-6;i++)hyas[i].active=0;for(int i=0;i<N-6;i++)hymks[i].active=0;init=1;ps("[HYC] Hyacinthoides initialized\n");return 0;}
int hyc_planning(int t,int c,int a,int b,int d,int e,int y){return add(hyps,&st.n_hyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hyc_execution(int t,int c,int a,int b,int d,int e,int y){return add(hyss,&st.n_hye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hyc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hyvss,&st.n_hyv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hyc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hyc_market(int t,int c,int a,int b,int d,int e,int y){return add(hymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hyc_report(void){ps("[HYC] Hyp: ");pi(st.n_hyp);ps(" PCS=");pi(st.t_f1);ps("\nHye: ");pi(st.n_hye);ps(" PCS=");pi(st.t_f2);ps("\nHyv: ");pi(st.n_hyv);ps(" PCS=");pi(st.t_f3);ps("\nHyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hyc_state(void){ps("[HYC] Hyp=");pi(st.n_hyp);ps(" Hye=");pi(st.n_hye);ps(" Hyv=");pi(st.n_hyv);ps(" Hyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hyacinthoides Admin Demo ===\n\n");hyc_init();
ps("Hyacinthoides planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hyc_planning(t,c,788+(i*17),777+(i*14),757+(i*10),739+(i*6),2020+(i%5));}
ps("\nHyacinthoides execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hyc_execution(t,c,777+(i*15),766+(i*12),748+(i*8),735+(i*5),2021+(i%4));}
ps("\nHyacinthoides evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hyc_evaluation(t,c,769+(i*13),758+(i*10),744+(i*7),733+(i*4),2022+(i%3));}
ps("\nHyacinthoides accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyc_accessory(t,c,761+(i*11),752+(i*9),738+(i*6),728+(i*3),2023+(i%2));}
ps("\nHyacinthoides marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyc_market(t,c,755+(i*9),746+(i*7),733+(i*5),725+(i*3),2024);}
ps("\n");hyc_report();hyc_state();ps("\n=== Demo Complete ===\n");return 0;}
