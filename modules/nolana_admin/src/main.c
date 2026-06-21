/* nolana_admin: Nolana management technology administration (v1.0)
 * Nolana planning, nolana execution, nolana evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nol_t;
typedef struct{int n_nolp,n_nole,n_nol2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nol_state_t;
static nol_t nolps[N],nolss[N-2],nolvss[N-4],nolas[N-6],nolmks[N-6]; static nol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nol_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nol_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NOL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nol_init(void){if(init)return -1;st.n_nolp=0;st.n_nole=0;st.n_nol2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nolps[i].active=0;for(int i=0;i<N-2;i++)nolss[i].active=0;for(int i=0;i<N-4;i++)nolvss[i].active=0;for(int i=0;i<N-6;i++)nolas[i].active=0;for(int i=0;i<N-6;i++)nolmks[i].active=0;init=1;ps("[NOL] Nolana initialized\n");return 0;}
int nol_planning(int t,int c,int a,int b,int d,int e,int y){return add(nolps,&st.n_nolp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nol_execution(int t,int c,int a,int b,int d,int e,int y){return add(nolss,&st.n_nole,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nol_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nolvss,&st.n_nol2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nol_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nolas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nol_market(int t,int c,int a,int b,int d,int e,int y){return add(nolmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nol_report(void){ps("[NOL] NOLp: ");pi(st.n_nolp);ps(" PCS=");pi(st.t_f1);ps("\nNole: ");pi(st.n_nole);ps(" PCS=");pi(st.t_f2);ps("\nNolv: ");pi(st.n_nol2);ps(" PCS=");pi(st.t_f3);ps("\nNOLc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nol_state(void){ps("[NOL] NOLp=");pi(st.n_nolp);ps(" Nole=");pi(st.n_nole);ps(" Nolv=");pi(st.n_nol2);ps(" NOLc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nolana Admin Demo ===\n\n");nol_init();
ps("Nolana planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nol_planning(t,c,1066+(i*17),1055+(i*14),1035+(i*10),1017+(i*6),2020+(i%5));}
ps("\nNolana execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nol_execution(t,c,1055+(i*15),1044+(i*12),1026+(i*8),1013+(i*5),2021+(i%4));}
ps("\nNolana evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nol_evaluation(t,c,1047+(i*13),1036+(i*10),1020+(i*7),1009+(i*4),2022+(i%3));}
ps("\nNolana accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nol_accessory(t,c,1039+(i*11),1030+(i*9),1016+(i*6),1006+(i*3),2023+(i%2));}
ps("\nNolana marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nol_market(t,c,1033+(i*9),1024+(i*7),1011+(i*5),1003+(i*3),2024);}
ps("\n");nol_report();nol_state();ps("\n=== Demo Complete ===\n");return 0;}
