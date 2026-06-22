/* ruellia_admin: Ruellia management technology administration (v1.0)
 * Ruellia planning, ruellia execution, ruellia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rue_t;
typedef struct{int n_ruep,n_ruee,n_rue2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rue_state_t;
static rue_t rueps[N],ruess[N-2],ruevss[N-4],rueas[N-6],ruemks[N-6]; static rue_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rue_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rue_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RUE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rue_init(void){if(init)return -1;st.n_ruep=0;st.n_ruee=0;st.n_rue2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rueps[i].active=0;for(int i=0;i<N-2;i++)ruess[i].active=0;for(int i=0;i<N-4;i++)ruevss[i].active=0;for(int i=0;i<N-6;i++)rueas[i].active=0;for(int i=0;i<N-6;i++)ruemks[i].active=0;init=1;ps("[RUE] Ruellia initialized\n");return 0;}
int rue_planning(int t,int c,int a,int b,int d,int e,int y){return add(rueps,&st.n_ruep,&st.t_f1,N,t,c,a,b,d,e,y);}
int rue_execution(int t,int c,int a,int b,int d,int e,int y){return add(ruess,&st.n_ruee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rue_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ruevss,&st.n_rue2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rue_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rueas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rue_market(int t,int c,int a,int b,int d,int e,int y){return add(ruemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rue_report(void){ps("[RUE] Ruepp: ");pi(st.n_ruep);ps(" PCS=");pi(st.t_f1);ps("\nRuee: ");pi(st.n_ruee);ps(" PCS=");pi(st.t_f2);ps("\nRuev: ");pi(st.n_rue2);ps(" PCS=");pi(st.t_f3);ps("\nRuec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rue_state(void){ps("[RUE] Ruepp=");pi(st.n_ruep);ps(" Ruee=");pi(st.n_ruee);ps(" Ruev=");pi(st.n_rue2);ps(" Ruec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ruellia Admin Demo ===\n\n");rue_init();
ps("Ruellia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rue_planning(t,c,1182+(i*17),1171+(i*14),1151+(i*10),1133+(i*6),2020+(i%5));}
ps("\nRuellia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rue_execution(t,c,1171+(i*15),1160+(i*12),1142+(i*8),1129+(i*5),2021+(i%4));}
ps("\nRuellia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rue_evaluation(t,c,1163+(i*13),1152+(i*10),1136+(i*7),1125+(i*4),2022+(i%3));}
ps("\nRuellia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rue_accessory(t,c,1155+(i*11),1146+(i*9),1132+(i*6),1122+(i*3),2023+(i%2));}
ps("\nRuellia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rue_market(t,c,1149+(i*9),1140+(i*7),1127+(i*5),1119+(i*3),2024);}
ps("\n");rue_report();rue_state();ps("\n=== Demo Complete ===\n");return 0;}
