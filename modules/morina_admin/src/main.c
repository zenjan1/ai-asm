/* morina_admin: Morina management technology administration (v1.0)
 * Morina planning, morina execution, morina evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mor_t;
typedef struct{int n_morp,n_more,n_mor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mor_state_t;
static mor_t morps[N],morss[N-2],morvss[N-4],moras[N-6],mormks[N-6]; static mor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mor_init(void){if(init)return -1;st.n_morp=0;st.n_more=0;st.n_mor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)morps[i].active=0;for(int i=0;i<N-2;i++)morss[i].active=0;for(int i=0;i<N-4;i++)morvss[i].active=0;for(int i=0;i<N-6;i++)moras[i].active=0;for(int i=0;i<N-6;i++)mormks[i].active=0;init=1;ps("[MOR] Morina initialized\n");return 0;}
int mor_planning(int t,int c,int a,int b,int d,int e,int y){return add(morps,&st.n_morp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mor_execution(int t,int c,int a,int b,int d,int e,int y){return add(morss,&st.n_more,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(morvss,&st.n_mor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(moras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mor_market(int t,int c,int a,int b,int d,int e,int y){return add(mormks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mor_report(void){ps("[MOR] Morpp: ");pi(st.n_morp);ps(" PCS=");pi(st.t_f1);ps("\nMore: ");pi(st.n_more);ps(" PCS=");pi(st.t_f2);ps("\nMorv: ");pi(st.n_mor2);ps(" PCS=");pi(st.t_f3);ps("\nMorc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mor_state(void){ps("[MOR] Morpp=");pi(st.n_morp);ps(" More=");pi(st.n_more);ps(" Morv=");pi(st.n_mor2);ps(" Morc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Morina Admin Demo ===\n\n");mor_init();
ps("Morina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mor_planning(t,c,1160+(i*17),1149+(i*14),1129+(i*10),1111+(i*6),2020+(i%5));}
ps("\nMorina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mor_execution(t,c,1149+(i*15),1138+(i*12),1120+(i*8),1107+(i*5),2021+(i%4));}
ps("\nMorina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mor_evaluation(t,c,1141+(i*13),1130+(i*10),1114+(i*7),1103+(i*4),2022+(i%3));}
ps("\nMorina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mor_accessory(t,c,1133+(i*11),1124+(i*9),1110+(i*6),1100+(i*3),2023+(i%2));}
ps("\nMorina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mor_market(t,c,1127+(i*9),1118+(i*7),1105+(i*5),1097+(i*3),2024);}
ps("\n");mor_report();mor_state();ps("\n=== Demo Complete ===\n");return 0;}
