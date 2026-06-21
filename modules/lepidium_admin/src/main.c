/* lepidium_admin: Lepidium management technology administration (v1.0)
 * Lepidium planning, lepidium execution, lepidium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lep_t;
typedef struct{int n_lepp,n_lepe,n_lep2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lep_state_t;
static lep_t lepps[N],lepss[N-2],lepvss[N-4],leapas[N-6],lepmks[N-6]; static lep_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lep_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lep_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LEP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lep_init(void){if(init)return -1;st.n_lepp=0;st.n_lepe=0;st.n_lep2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lepps[i].active=0;for(int i=0;i<N-2;i++)lepss[i].active=0;for(int i=0;i<N-4;i++)lepvss[i].active=0;for(int i=0;i<N-6;i++)leapas[i].active=0;for(int i=0;i<N-6;i++)lepmks[i].active=0;init=1;ps("[LEP] Lepidium initialized\n");return 0;}
int lep_planning(int t,int c,int a,int b,int d,int e,int y){return add(lepps,&st.n_lepp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lep_execution(int t,int c,int a,int b,int d,int e,int y){return add(lepss,&st.n_lepe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lep_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lepvss,&st.n_lep2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lep_accessory(int t,int c,int a,int b,int d,int e,int y){return add(leapas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lep_market(int t,int c,int a,int b,int d,int e,int y){return add(lepmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lep_report(void){ps("[LEP] Lepp: ");pi(st.n_lepp);ps(" PCS=");pi(st.t_f1);ps("\nLepe: ");pi(st.n_lepe);ps(" PCS=");pi(st.t_f2);ps("\nLepv: ");pi(st.n_lep2);ps(" PCS=");pi(st.t_f3);ps("\nLepc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lep_state(void){ps("[LEP] Lepp=");pi(st.n_lepp);ps(" Lepe=");pi(st.n_lepe);ps(" Lepv=");pi(st.n_lep2);ps(" Lepc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lepidium Admin Demo ===\n\n");lep_init();
ps("Lepidium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lep_planning(t,c,1052+(i*17),1041+(i*14),1021+(i*10),1003+(i*6),2020+(i%5));}
ps("\nLepidium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lep_execution(t,c,1041+(i*15),1030+(i*12),1012+(i*8),999+(i*5),2021+(i%4));}
ps("\nLepidium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lep_evaluation(t,c,1033+(i*13),1022+(i*10),1006+(i*7),995+(i*4),2022+(i%3));}
ps("\nLepidium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lep_accessory(t,c,1025+(i*11),1016+(i*9),1002+(i*6),992+(i*3),2023+(i%2));}
ps("\nLepidium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lep_market(t,c,1019+(i*9),1010+(i*7),997+(i*5),989+(i*3),2024);}
ps("\n");lep_report();lep_state();ps("\n=== Demo Complete ===\n");return 0;}
