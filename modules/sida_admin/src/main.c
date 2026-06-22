/* sida_admin: Sida management technology administration (v1.0)
 * Sida planning, sida execution, sida evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sid_t;
typedef struct{int n_sidp,n_side,n_sid2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sid_state_t;
static sid_t sidps[N],sidss[N-2],sidvss[N-4],sidas[N-6],sidmks[N-6]; static sid_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sid_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sid_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SID] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sid_init(void){if(init)return -1;st.n_sidp=0;st.n_side=0;st.n_sid2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sidps[i].active=0;for(int i=0;i<N-2;i++)sidss[i].active=0;for(int i=0;i<N-4;i++)sidvss[i].active=0;for(int i=0;i<N-6;i++)sidas[i].active=0;for(int i=0;i<N-6;i++)sidmks[i].active=0;init=1;ps("[SID] Sida initialized\n");return 0;}
int sid_planning(int t,int c,int a,int b,int d,int e,int y){return add(sidps,&st.n_sidp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sid_execution(int t,int c,int a,int b,int d,int e,int y){return add(sidss,&st.n_side,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sid_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sidvss,&st.n_sid2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sid_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sidas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sid_market(int t,int c,int a,int b,int d,int e,int y){return add(sidmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sid_report(void){ps("[SID] Sidpp: ");pi(st.n_sidp);ps(" PCS=");pi(st.t_f1);ps("\nSide: ");pi(st.n_side);ps(" PCS=");pi(st.t_f2);ps("\nSidv: ");pi(st.n_sid2);ps(" PCS=");pi(st.t_f3);ps("\nSidc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sid_state(void){ps("[SID] Sidpp=");pi(st.n_sidp);ps(" Side=");pi(st.n_side);ps(" Sidv=");pi(st.n_sid2);ps(" Sidc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sida Admin Demo ===\n\n");sid_init();
ps("Sida planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sid_planning(t,c,1191+(i*17),1180+(i*14),1160+(i*10),1142+(i*6),2020+(i%5));}
ps("\nSida execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sid_execution(t,c,1180+(i*15),1169+(i*12),1151+(i*8),1138+(i*5),2021+(i%4));}
ps("\nSida evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sid_evaluation(t,c,1172+(i*13),1161+(i*10),1145+(i*7),1134+(i*4),2022+(i%3));}
ps("\nSida accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sid_accessory(t,c,1164+(i*11),1155+(i*9),1141+(i*6),1131+(i*3),2023+(i%2));}
ps("\nSida marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sid_market(t,c,1158+(i*9),1149+(i*7),1136+(i*5),1128+(i*3),2024);}
ps("\n");sid_report();sid_state();ps("\n=== Demo Complete ===\n");return 0;}
