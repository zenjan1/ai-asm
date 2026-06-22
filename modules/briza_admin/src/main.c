/* briza_admin: Briza management technology administration (v1.0)
 * Briza planning, briza execution, briza evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} briz_t;
typedef struct{int n_brizp,n_briz,n_briz2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} briz_state_t;
static briz_t brizs[N],brize[N-2],briz2[N-4],brizac[N-6],brizam[N-6]; static briz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(briz_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;briz_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BRIZ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int briz_init(void){if(init)return -1;st.n_brizp=0;st.n_briz=0;st.n_briz2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brizs[i].active=0;for(int i=0;i<N-2;i++)brize[i].active=0;for(int i=0;i<N-4;i++)briz2[i].active=0;for(int i=0;i<N-6;i++)brizac[i].active=0;for(int i=0;i<N-6;i++)brizam[i].active=0;init=1;ps("[BRIZ] Briza initialized\n");return 0;}
int briz_planning(int t,int c,int a,int b,int d,int e,int y){return add(brizs,&st.n_brizp,&st.t_f1,N,t,c,a,b,d,e,y);}
int briz_execution(int t,int c,int a,int b,int d,int e,int y){return add(brize,&st.n_briz,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int briz_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(briz2,&st.n_briz2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int briz_accessory(int t,int c,int a,int b,int d,int e,int y){return add(brizac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int briz_market(int t,int c,int a,int b,int d,int e,int y){return add(brizam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void briz_report(void){ps("[BRIZ] Zip: ");pi(st.n_brizp);ps(" PCS=");pi(st.t_f1);ps("\nZie: ");pi(st.n_briz);ps(" PCS=");pi(st.t_f2);ps("\nZi2: ");pi(st.n_briz2);ps(" PCS=");pi(st.t_f3);ps("\nZiac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void briz_state(void){ps("[BRIZ] Zip=");pi(st.n_brizp);ps(" Zi=");pi(st.n_briz);ps(" Zi2=");pi(st.n_briz2);ps(" Ziac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Briza Admin Demo ===\n\n");briz_init();
ps("Briza planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;briz_planning(t,c,1443+(i*17),1432+(i*14),1412+(i*10),1394+(i*6),2020+(i%5));}
ps("\nBriza execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;briz_execution(t,c,1432+(i*15),1421+(i*12),1403+(i*8),1390+(i*5),2021+(i%4));}
ps("\nBriza evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;briz_evaluation(t,c,1424+(i*13),1413+(i*10),1397+(i*7),1386+(i*4),2022+(i%3));}
ps("\nBriza accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;briz_accessory(t,c,1416+(i*11),1407+(i*9),1393+(i*6),1383+(i*3),2023+(i%2));}
ps("\nBriza marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;briz_market(t,c,1410+(i*9),1401+(i*7),1388+(i*5),1380+(i*3),2024);}
ps("\n");briz_report();briz_state();ps("\n=== Demo Complete ===\n");return 0;}
