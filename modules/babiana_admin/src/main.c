/* babiana_admin: Babiana management technology administration (v1.0)
 * Babiana planning, babiana execution, babiana evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} babi_t;
typedef struct{int n_babip,n_babi,n_babi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} babi_state_t;
static babi_t babis[N],babie[N-2],babi2[N-4],babiac[N-6],babiam[N-6]; static babi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(babi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;babi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BABI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int babi_init(void){if(init)return -1;st.n_babip=0;st.n_babi=0;st.n_babi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)babis[i].active=0;for(int i=0;i<N-2;i++)babie[i].active=0;for(int i=0;i<N-4;i++)babi2[i].active=0;for(int i=0;i<N-6;i++)babiac[i].active=0;for(int i=0;i<N-6;i++)babiam[i].active=0;init=1;ps("[BABI] Babiana initialized\n");return 0;}
int babi_planning(int t,int c,int a,int b,int d,int e,int y){return add(babis,&st.n_babip,&st.t_f1,N,t,c,a,b,d,e,y);}
int babi_execution(int t,int c,int a,int b,int d,int e,int y){return add(babie,&st.n_babi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int babi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(babi2,&st.n_babi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int babi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(babiac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int babi_market(int t,int c,int a,int b,int d,int e,int y){return add(babiam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void babi_report(void){ps("[BABI] Bip: ");pi(st.n_babip);ps(" PCS=");pi(st.t_f1);ps("\nBie: ");pi(st.n_babi);ps(" PCS=");pi(st.t_f2);ps("\nBi2: ");pi(st.n_babi2);ps(" PCS=");pi(st.t_f3);ps("\nBiac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void babi_state(void){ps("[BABI] Bip=");pi(st.n_babip);ps(" Bie=");pi(st.n_babi);ps(" Bi2=");pi(st.n_babi2);ps(" Biac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Babiana Admin Demo ===\n\n");babi_init();
ps("Babiana planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;babi_planning(t,c,1426+(i*17),1415+(i*14),1395+(i*10),1377+(i*6),2020+(i%5));}
ps("\nBabiana execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;babi_execution(t,c,1415+(i*15),1404+(i*12),1386+(i*8),1373+(i*5),2021+(i%4));}
ps("\nBabiana evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;babi_evaluation(t,c,1407+(i*13),1396+(i*10),1380+(i*7),1369+(i*4),2022+(i%3));}
ps("\nBabiana accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;babi_accessory(t,c,1399+(i*11),1390+(i*9),1376+(i*6),1366+(i*3),2023+(i%2));}
ps("\nBabiana marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;babi_market(t,c,1393+(i*9),1384+(i*7),1371+(i*5),1363+(i*3),2024);}
ps("\n");babi_report();babi_state();ps("\n=== Demo Complete ===\n");return 0;}
