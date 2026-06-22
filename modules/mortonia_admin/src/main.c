/* mortonia_admin: Mortonia management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mort_t;
typedef struct{int n_mortop,n_mort,n_mort2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mort_state_t;
static mort_t mortos[N],morte[N-2],mort2[N-4],mortac[N-6],mortam[N-6]; static mort_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mort_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mort_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MORT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mort_init(void){if(init)return -1;st.n_mortop=0;st.n_mort=0;st.n_mort2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mortos[i].active=0;for(int i=0;i<N-2;i++)morte[i].active=0;for(int i=0;i<N-4;i++)mort2[i].active=0;for(int i=0;i<N-6;i++)mortac[i].active=0;for(int i=0;i<N-6;i++)mortam[i].active=0;init=1;ps("[MORT] Mortonia initialized\n");return 0;}
int mort_planning(int t,int c,int a,int b,int d,int e,int y){return add(mortos,&st.n_mortop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mort_execution(int t,int c,int a,int b,int d,int e,int y){return add(morte,&st.n_mort,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mort_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mort2,&st.n_mort2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mort_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mortac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mort_market(int t,int c,int a,int b,int d,int e,int y){return add(mortam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mort_report(void){ps("[MORT] Mortp: ");pi(st.n_mortop);ps(" PCS=");pi(st.t_f1);ps("\nMorte: ");pi(st.n_mort);ps(" PCS=");pi(st.t_f2);ps("\nMort2: ");pi(st.n_mort2);ps(" PCS=");pi(st.t_f3);ps("\nMrt: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mort_state(void){ps("[MORT] Mortp=");pi(st.n_mortop);ps(" Mort=");pi(st.n_mort);ps(" Rt2=");pi(st.n_mort2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mortonia Admin Demo ===\n\n");mort_init();
ps("Mortonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mort_planning(t,c,1580-(i*17),1569-(i*14),1549-(i*10),1531-(i*6),2020+(i%5));}
ps("\nMortonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mort_execution(t,c,1569-(i*15),1558-(i*12),1540-(i*8),1527-(i*5),2021+(i%4));}
ps("\nMortonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mort_evaluation(t,c,1561-(i*13),1550-(i*10),1534-(i*7),1523-(i*4),2022+(i%3));}
ps("\nMortonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mort_accessory(t,c,1553-(i*11),1544-(i*9),1530-(i*6),1520-(i*3),2023+(i%2));}
ps("\nMortonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mort_market(t,c,1547-(i*9),1538-(i*7),1525-(i*5),1517-(i*3),2024);}
ps("\n");mort_report();mort_state();ps("\n=== Demo Complete ===\n");return 0;}
