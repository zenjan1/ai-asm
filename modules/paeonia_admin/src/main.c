/* paeonia_admin: Paeonia management technology administration (v1.0)
 * Paeonia planning, paeonia execution, paeonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pae_t;
typedef struct{int n_paep,n_paee,n_pae2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pae_state_t;
static pae_t paeps[N],paess[N-2],paevss[N-4],paeas[N-6],paemks[N-6]; static pae_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pae_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pae_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PAE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pae_init(void){if(init)return -1;st.n_paep=0;st.n_paee=0;st.n_pae2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)paeps[i].active=0;for(int i=0;i<N-2;i++)paess[i].active=0;for(int i=0;i<N-4;i++)paevss[i].active=0;for(int i=0;i<N-6;i++)paeas[i].active=0;for(int i=0;i<N-6;i++)paemks[i].active=0;init=1;ps("[PAE] Paeonia initialized\n");return 0;}
int pae_planning(int t,int c,int a,int b,int d,int e,int y){return add(paeps,&st.n_paep,&st.t_f1,N,t,c,a,b,d,e,y);}
int pae_execution(int t,int c,int a,int b,int d,int e,int y){return add(paess,&st.n_paee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pae_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(paevss,&st.n_pae2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pae_accessory(int t,int c,int a,int b,int d,int e,int y){return add(paeas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pae_market(int t,int c,int a,int b,int d,int e,int y){return add(paemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pae_report(void){ps("[PAE] paep: ");pi(st.n_paep);ps(" PCS=");pi(st.t_f1);ps("\npaee: ");pi(st.n_paee);ps(" PCS=");pi(st.t_f2);ps("\npaev: ");pi(st.n_pae2);ps(" PCS=");pi(st.t_f3);ps("\npaec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pae_state(void){ps("[PAE] paep=");pi(st.n_paep);ps(" paee=");pi(st.n_paee);ps(" paev=");pi(st.n_pae2);ps(" paec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Paeonia Admin Demo ===\n\n");pae_init();
ps("Paeonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pae_planning(t,c,1081+(i*17),1070+(i*14),1050+(i*10),1032+(i*6),2020+(i%5));}
ps("\nPaeonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pae_execution(t,c,1070+(i*15),1059+(i*12),1041+(i*8),1028+(i*5),2021+(i%4));}
ps("\nPaeonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pae_evaluation(t,c,1062+(i*13),1051+(i*10),1035+(i*7),1024+(i*4),2022+(i%3));}
ps("\nPaeonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pae_accessory(t,c,1054+(i*11),1045+(i*9),1031+(i*6),1021+(i*3),2023+(i%2));}
ps("\nPaeonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pae_market(t,c,1048+(i*9),1039+(i*7),1026+(i*5),1018+(i*3),2024);}
ps("\n");pae_report();pae_state();ps("\n=== Demo Complete ===\n");return 0;}
