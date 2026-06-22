/* exochorda_admin: Exochorda management technology administration (v1.0)
 * Exochorda planning, exochorda execution, exochorda evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} exo_t;
typedef struct{int n_exop,n_exoe,n_exo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} exo_state_t;
static exo_t exops[N],exoss[N-2],exovss[N-4],exoas[N-6],exomks[N-6]; static exo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(exo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;exo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EXO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int exo_init(void){if(init)return -1;st.n_exop=0;st.n_exoe=0;st.n_exo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)exops[i].active=0;for(int i=0;i<N-2;i++)exoss[i].active=0;for(int i=0;i<N-4;i++)exovss[i].active=0;for(int i=0;i<N-6;i++)exoas[i].active=0;for(int i=0;i<N-6;i++)exomks[i].active=0;init=1;ps("[EXO] Exochorda initialized\n");return 0;}
int exo_planning(int t,int c,int a,int b,int d,int e,int y){return add(exops,&st.n_exop,&st.t_f1,N,t,c,a,b,d,e,y);}
int exo_execution(int t,int c,int a,int b,int d,int e,int y){return add(exoss,&st.n_exoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int exo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(exovss,&st.n_exo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int exo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(exoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int exo_market(int t,int c,int a,int b,int d,int e,int y){return add(exomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void exo_report(void){ps("[EXO] Exopp: ");pi(st.n_exop);ps(" PCS=");pi(st.t_f1);ps("\nExoe: ");pi(st.n_exoe);ps(" PCS=");pi(st.t_f2);ps("\nExov: ");pi(st.n_exo2);ps(" PCS=");pi(st.t_f3);ps("\nExoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void exo_state(void){ps("[EXO] Exopp=");pi(st.n_exop);ps(" Exoe=");pi(st.n_exoe);ps(" Exov=");pi(st.n_exo2);ps(" Exoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Exochorda Admin Demo ===\n\n");exo_init();
ps("Exochorda planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;exo_planning(t,c,1142+(i*17),1131+(i*14),1111+(i*10),1093+(i*6),2020+(i%5));}
ps("\nExochorda execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;exo_execution(t,c,1131+(i*15),1120+(i*12),1102+(i*8),1089+(i*5),2021+(i%4));}
ps("\nExochorda evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;exo_evaluation(t,c,1123+(i*13),1112+(i*10),1096+(i*7),1085+(i*4),2022+(i%3));}
ps("\nExochorda accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;exo_accessory(t,c,1115+(i*11),1106+(i*9),1092+(i*6),1082+(i*3),2023+(i%2));}
ps("\nExochorda marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;exo_market(t,c,1109+(i*9),1100+(i*7),1087+(i*5),1079+(i*3),2024);}
ps("\n");exo_report();exo_state();ps("\n=== Demo Complete ===\n");return 0;}
