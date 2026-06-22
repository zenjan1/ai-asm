/* coix_admin: Coix management technology administration (v1.0)
 * Coix planning, coix execution, coix evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} coi_t;
typedef struct{int n_coip,n_coie,n_coi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} coi_state_t;
static coi_t coips[N],coiss[N-2],coivss[N-4],coias[N-6],coimks[N-6]; static coi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(coi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;coi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[COI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int coi_init(void){if(init)return -1;st.n_coip=0;st.n_coie=0;st.n_coi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)coips[i].active=0;for(int i=0;i<N-2;i++)coiss[i].active=0;for(int i=0;i<N-4;i++)coivss[i].active=0;for(int i=0;i<N-6;i++)coias[i].active=0;for(int i=0;i<N-6;i++)coimks[i].active=0;init=1;ps("[COI] Coix initialized\n");return 0;}
int coi_planning(int t,int c,int a,int b,int d,int e,int y){return add(coips,&st.n_coip,&st.t_f1,N,t,c,a,b,d,e,y);}
int coi_execution(int t,int c,int a,int b,int d,int e,int y){return add(coiss,&st.n_coie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int coi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(coivss,&st.n_coi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int coi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(coias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int coi_market(int t,int c,int a,int b,int d,int e,int y){return add(coimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void coi_report(void){ps("[COI] Coipp: ");pi(st.n_coip);ps(" PCS=");pi(st.t_f1);ps("\nCoie: ");pi(st.n_coie);ps(" PCS=");pi(st.t_f2);ps("\nCoiv: ");pi(st.n_coi2);ps(" PCS=");pi(st.t_f3);ps("\nCoic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void coi_state(void){ps("[COI] Coipp=");pi(st.n_coip);ps(" Coie=");pi(st.n_coie);ps(" Coiv=");pi(st.n_coi2);ps(" Coic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Coix Admin Demo ===\n\n");coi_init();
ps("Coix planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;coi_planning(t,c,1132+(i*17),1121+(i*14),1101+(i*10),1083+(i*6),2020+(i%5));}
ps("\nCoix execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;coi_execution(t,c,1121+(i*15),1110+(i*12),1092+(i*8),1079+(i*5),2021+(i%4));}
ps("\nCoix evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;coi_evaluation(t,c,1113+(i*13),1102+(i*10),1086+(i*7),1075+(i*4),2022+(i%3));}
ps("\nCoix accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;coi_accessory(t,c,1105+(i*11),1096+(i*9),1082+(i*6),1072+(i*3),2023+(i%2));}
ps("\nCoix marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;coi_market(t,c,1099+(i*9),1090+(i*7),1077+(i*5),1069+(i*3),2024);}
ps("\n");coi_report();coi_state();ps("\n=== Demo Complete ===\n");return 0;}
