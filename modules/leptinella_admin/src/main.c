/* leptinella_admin: Leptinella management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lept_t;
typedef struct{int n_leptop,n_lept,n_lept2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lept_state_t;
static lept_t leptos[N],lepte[N-2],lept2[N-4],leptac[N-6],leptam[N-6]; static lept_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(lept_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lept_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEPT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lept_init(void){if(init)return -1;st.n_leptop=0;st.n_lept=0;st.n_lept2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)leptos[i].active=0;for(int i=0;i<N-2;i++)lepte[i].active=0;for(int i=0;i<N-4;i++)lept2[i].active=0;for(int i=0;i<N-6;i++)leptac[i].active=0;for(int i=0;i<N-6;i++)leptam[i].active=0;init=1;ps("[LEPT] Leptinella initialized\n");return 0;}
int lept_planning(int t,int c,int a,int b,int d,int e,int y){return add(leptos,&st.n_leptop,&st.t_f1,N,t,c,a,b,d,e,y);}
int lept_execution(int t,int c,int a,int b,int d,int e,int y){return add(lepte,&st.n_lept,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lept_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lept2,&st.n_lept2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lept_accessory(int t,int c,int a,int b,int d,int e,int y){return add(leptac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lept_market(int t,int c,int a,int b,int d,int e,int y){return add(leptam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lept_report(void){ps("[LEPT] Leptp: ");pi(st.n_leptop);ps(" PCS=");pi(st.t_f1);ps("\nLepte: ");pi(st.n_lept);ps(" PCS=");pi(st.t_f2);ps("\nLept2: ");pi(st.n_lept2);ps(" PCS=");pi(st.t_f3);ps("\nLpt: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lept_state(void){ps("[LEPT] Leptp=");pi(st.n_leptop);ps(" Lept=");pi(st.n_lept);ps(" Lpt2=");pi(st.n_lept2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Leptinella Admin Demo ===\n\n");lept_init();
ps("Leptinella planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lept_planning(t,c,1555-(i*17),1544-(i*14),1524-(i*10),1506-(i*6),2020+(i%5));}
ps("\nLeptinella execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lept_execution(t,c,1544-(i*15),1533-(i*12),1515-(i*8),1502-(i*5),2021+(i%4));}
ps("\nLeptinella evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lept_evaluation(t,c,1536-(i*13),1525-(i*10),1509-(i*7),1498-(i*4),2022+(i%3));}
ps("\nLeptinella accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lept_accessory(t,c,1528-(i*11),1519-(i*9),1505-(i*6),1495-(i*3),2023+(i%2));}
ps("\nLeptinella marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lept_market(t,c,1522-(i*9),1513-(i*7),1500-(i*5),1492-(i*3),2024);}
ps("\n");lept_report();lept_state();ps("\n=== Demo Complete ===\n");return 0;}
