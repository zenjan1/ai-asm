/* pteris_admin: Pteris management technology administration (v1.0)
 * Pteris planning, pteris execution, pteris evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pte_t;
typedef struct{int n_ptep,n_ptee,n_pte2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pte_state_t;
static pte_t pteps[N],ptess[N-2],ptevss[N-4],pteas[N-6],ptemks[N-6]; static pte_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pte_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pte_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PTE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pte_init(void){if(init)return -1;st.n_ptep=0;st.n_ptee=0;st.n_pte2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pteps[i].active=0;for(int i=0;i<N-2;i++)ptess[i].active=0;for(int i=0;i<N-4;i++)ptevss[i].active=0;for(int i=0;i<N-6;i++)pteas[i].active=0;for(int i=0;i<N-6;i++)ptemks[i].active=0;init=1;ps("[PTE] Pteris initialized\n");return 0;}
int pte_planning(int t,int c,int a,int b,int d,int e,int y){return add(pteps,&st.n_ptep,&st.t_f1,N,t,c,a,b,d,e,y);}
int pte_execution(int t,int c,int a,int b,int d,int e,int y){return add(ptess,&st.n_ptee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pte_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ptevss,&st.n_pte2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pte_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pteas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pte_market(int t,int c,int a,int b,int d,int e,int y){return add(ptemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pte_report(void){ps("[PTE] ptep: ");pi(st.n_ptep);ps(" PCS=");pi(st.t_f1);ps("\nptee: ");pi(st.n_ptee);ps(" PCS=");pi(st.t_f2);ps("\nptev: ");pi(st.n_pte2);ps(" PCS=");pi(st.t_f3);ps("\nptec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pte_state(void){ps("[PTE] ptep=");pi(st.n_ptep);ps(" ptee=");pi(st.n_ptee);ps(" ptev=");pi(st.n_pte2);ps(" ptec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pteris Admin Demo ===\n\n");pte_init();
ps("Pteris planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pte_planning(t,c,1088+(i*17),1077+(i*14),1057+(i*10),1039+(i*6),2020+(i%5));}
ps("\nPteris execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pte_execution(t,c,1077+(i*15),1066+(i*12),1048+(i*8),1035+(i*5),2021+(i%4));}
ps("\nPteris evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pte_evaluation(t,c,1069+(i*13),1058+(i*10),1042+(i*7),1031+(i*4),2022+(i%3));}
ps("\nPteris accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pte_accessory(t,c,1061+(i*11),1052+(i*9),1038+(i*6),1028+(i*3),2023+(i%2));}
ps("\nPteris marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pte_market(t,c,1055+(i*9),1046+(i*7),1033+(i*5),1025+(i*3),2024);}
ps("\n");pte_report();pte_state();ps("\n=== Demo Complete ===\n");return 0;}
