/* nyssa_admin: Nyssa management technology administration (v1.0)
 * Nyssa planning, nyssa execution, nyssa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nys_t;
typedef struct{int n_nysp,n_nyse,n_nys2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nys_state_t;
static nys_t nysps[N],nysss[N-2],nysvss[N-4],nysas[N-6],nysmks[N-6]; static nys_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nys_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nys_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NYS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nys_init(void){if(init)return -1;st.n_nysp=0;st.n_nyse=0;st.n_nys2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nysps[i].active=0;for(int i=0;i<N-2;i++)nysss[i].active=0;for(int i=0;i<N-4;i++)nysvss[i].active=0;for(int i=0;i<N-6;i++)nysas[i].active=0;for(int i=0;i<N-6;i++)nysmks[i].active=0;init=1;ps("[NYS] Nyssa initialized\n");return 0;}
int nys_planning(int t,int c,int a,int b,int d,int e,int y){return add(nysps,&st.n_nysp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nys_execution(int t,int c,int a,int b,int d,int e,int y){return add(nysss,&st.n_nyse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nys_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nysvss,&st.n_nys2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nys_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nysas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nys_market(int t,int c,int a,int b,int d,int e,int y){return add(nysmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nys_report(void){ps("[NYS] Nyssp: ");pi(st.n_nysp);ps(" PCS=");pi(st.t_f1);ps("\nNysse: ");pi(st.n_nyse);ps(" PCS=");pi(st.t_f2);ps("\nNyssv: ");pi(st.n_nys2);ps(" PCS=");pi(st.t_f3);ps("\nNyssc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nys_state(void){ps("[NYS] Nyssp=");pi(st.n_nysp);ps(" Nysse=");pi(st.n_nyse);ps(" Nyssv=");pi(st.n_nys2);ps(" Nyssc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nyssa Admin Demo ===\n\n");nys_init();
ps("Nyssa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nys_planning(t,c,1071+(i*17),1060+(i*14),1040+(i*10),1022+(i*6),2020+(i%5));}
ps("\nNyssa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nys_execution(t,c,1060+(i*15),1049+(i*12),1031+(i*8),1018+(i*5),2021+(i%4));}
ps("\nNyssa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nys_evaluation(t,c,1052+(i*13),1041+(i*10),1025+(i*7),1014+(i*4),2022+(i%3));}
ps("\nNyssa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nys_accessory(t,c,1044+(i*11),1035+(i*9),1021+(i*6),1011+(i*3),2023+(i%2));}
ps("\nNyssa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nys_market(t,c,1038+(i*9),1029+(i*7),1016+(i*5),1008+(i*3),2024);}
ps("\n");nys_report();nys_state();ps("\n=== Demo Complete ===\n");return 0;}
