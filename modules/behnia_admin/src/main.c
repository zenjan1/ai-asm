/* behnia_admin: Behnia management technology administration (v1.0)
 * Behnia planning, behnia execution, behnia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} behn_t;
typedef struct{int n_behnp,n_behn,n_behn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} behn_state_t;
static behn_t behns[N],behne[N-2],behn2[N-4],behnac[N-6],behnam[N-6]; static behn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(behn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;behn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BEHN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int behn_init(void){if(init)return -1;st.n_behnp=0;st.n_behn=0;st.n_behn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)behns[i].active=0;for(int i=0;i<N-2;i++)behne[i].active=0;for(int i=0;i<N-4;i++)behn2[i].active=0;for(int i=0;i<N-6;i++)behnac[i].active=0;for(int i=0;i<N-6;i++)behnam[i].active=0;init=1;ps("[BEHN] Behnia initialized\n");return 0;}
int behn_planning(int t,int c,int a,int b,int d,int e,int y){return add(behns,&st.n_behnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int behn_execution(int t,int c,int a,int b,int d,int e,int y){return add(behne,&st.n_behn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int behn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(behn2,&st.n_behn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int behn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(behnac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int behn_market(int t,int c,int a,int b,int d,int e,int y){return add(behnam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void behn_report(void){ps("[BEHN] Hnp: ");pi(st.n_behnp);ps(" PCS=");pi(st.t_f1);ps("\nHne: ");pi(st.n_behn);ps(" PCS=");pi(st.t_f2);ps("\nHn2: ");pi(st.n_behn2);ps(" PCS=");pi(st.t_f3);ps("\nHnac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void behn_state(void){ps("[BEHN] Hnp=");pi(st.n_behnp);ps(" Hne=");pi(st.n_behn);ps(" Hn2=");pi(st.n_behn2);ps(" Hnac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Behnia Admin Demo ===\n\n");behn_init();
ps("Behnia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;behn_planning(t,c,1432+(i*17),1421+(i*14),1401+(i*10),1383+(i*6),2020+(i%5));}
ps("\nBehnia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;behn_execution(t,c,1421+(i*15),1410+(i*12),1392+(i*8),1379+(i*5),2021+(i%4));}
ps("\nBehnia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;behn_evaluation(t,c,1413+(i*13),1402+(i*10),1386+(i*7),1375+(i*4),2022+(i%3));}
ps("\nBehnia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;behn_accessory(t,c,1405+(i*11),1396+(i*9),1382+(i*6),1372+(i*3),2023+(i%2));}
ps("\nBehnia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;behn_market(t,c,1399+(i*9),1390+(i*7),1377+(i*5),1369+(i*3),2024);}
ps("\n");behn_report();behn_state();ps("\n=== Demo Complete ===\n");return 0;}
