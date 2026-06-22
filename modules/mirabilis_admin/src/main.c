/* mirabilis_admin: Mirabilis management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mira_t;
typedef struct{int n_miraop,n_mira,n_mira2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mira_state_t;
static mira_t miros[N],mirae[N-2],mira2[N-4],mirac[N-6],miram[N-6]; static mira_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mira_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mira_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MIRA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mira_init(void){if(init)return -1;st.n_miraop=0;st.n_mira=0;st.n_mira2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)miros[i].active=0;for(int i=0;i<N-2;i++)mirae[i].active=0;for(int i=0;i<N-4;i++)mira2[i].active=0;for(int i=0;i<N-6;i++)mirac[i].active=0;for(int i=0;i<N-6;i++)miram[i].active=0;init=1;ps("[MIRA] Mirabilis initialized\n");return 0;}
int mira_planning(int t,int c,int a,int b,int d,int e,int y){return add(miros,&st.n_miraop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mira_execution(int t,int c,int a,int b,int d,int e,int y){return add(mirae,&st.n_mira,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mira_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mira2,&st.n_mira2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mira_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mirac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mira_market(int t,int c,int a,int b,int d,int e,int y){return add(miram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mira_report(void){ps("[MIRA] Mirap: ");pi(st.n_miraop);ps(" PCS=");pi(st.t_f1);ps("\nMirae: ");pi(st.n_mira);ps(" PCS=");pi(st.t_f2);ps("\nMira2: ");pi(st.n_mira2);ps(" PCS=");pi(st.t_f3);ps("\nMra: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mira_state(void){ps("[MIRA] Mirap=");pi(st.n_miraop);ps(" Mira=");pi(st.n_mira);ps(" Ra2=");pi(st.n_mira2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mirabilis Admin Demo ===\n\n");mira_init();
ps("Mirabilis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mira_planning(t,c,1574-(i*17),1563-(i*14),1543-(i*10),1525-(i*6),2020+(i%5));}
ps("\nMirabilis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mira_execution(t,c,1563-(i*15),1552-(i*12),1534-(i*8),1521-(i*5),2021+(i%4));}
ps("\nMirabilis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mira_evaluation(t,c,1555-(i*13),1544-(i*10),1528-(i*7),1517-(i*4),2022+(i%3));}
ps("\nMirabilis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mira_accessory(t,c,1547-(i*11),1538-(i*9),1524-(i*6),1514-(i*3),2023+(i%2));}
ps("\nMirabilis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mira_market(t,c,1541-(i*9),1532-(i*7),1519-(i*5),1511-(i*3),2024);}
ps("\n");mira_report();mira_state();ps("\n=== Demo Complete ===\n");return 0;}
