/* talinum_admin: Talinum management technology administration (v1.0)
 * Talinum planning, talinum execution, talinum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tal_t;
typedef struct{int n_talp,n_tale,n_tal2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tal_state_t;
static tal_t talps[N],talss[N-2],talvss[N-4],talas[N-6],talmks[N-6]; static tal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tal_init(void){if(init)return -1;st.n_talp=0;st.n_tale=0;st.n_tal2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)talps[i].active=0;for(int i=0;i<N-2;i++)talss[i].active=0;for(int i=0;i<N-4;i++)talvss[i].active=0;for(int i=0;i<N-6;i++)talas[i].active=0;for(int i=0;i<N-6;i++)talmks[i].active=0;init=1;ps("[TAL] Talinum initialized\n");return 0;}
int tal_planning(int t,int c,int a,int b,int d,int e,int y){return add(talps,&st.n_talp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tal_execution(int t,int c,int a,int b,int d,int e,int y){return add(talss,&st.n_tale,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(talvss,&st.n_tal2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(talas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tal_market(int t,int c,int a,int b,int d,int e,int y){return add(talmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tal_report(void){ps("[TAL] Talpp: ");pi(st.n_talp);ps(" PCS=");pi(st.t_f1);ps("\nTale: ");pi(st.n_tale);ps(" PCS=");pi(st.t_f2);ps("\nTalv: ");pi(st.n_tal2);ps(" PCS=");pi(st.t_f3);ps("\nTalc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tal_state(void){ps("[TAL] Talpp=");pi(st.n_talp);ps(" Tale=");pi(st.n_tale);ps(" Talv=");pi(st.n_tal2);ps(" Talc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Talinum Admin Demo ===\n\n");tal_init();
ps("Talinum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tal_planning(t,c,1195+(i*17),1184+(i*14),1164+(i*10),1146+(i*6),2020+(i%5));}
ps("\nTalinum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tal_execution(t,c,1184+(i*15),1173+(i*12),1155+(i*8),1142+(i*5),2021+(i%4));}
ps("\nTalinum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tal_evaluation(t,c,1176+(i*13),1165+(i*10),1149+(i*7),1138+(i*4),2022+(i%3));}
ps("\nTalinum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tal_accessory(t,c,1168+(i*11),1159+(i*9),1145+(i*6),1135+(i*3),2023+(i%2));}
ps("\nTalinum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tal_market(t,c,1162+(i*9),1153+(i*7),1140+(i*5),1132+(i*3),2024);}
ps("\n");tal_report();tal_state();ps("\n=== Demo Complete ===\n");return 0;}
