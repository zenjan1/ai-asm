/* rehmannia_admin: Rehmannia management technology administration (v1.0)
 * Rehmannia planning, rehmannia execution, rehmannia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} reh_t;
typedef struct{int n_rehp,n_rehe,n_reh2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} reh_state_t;
static reh_t rehps[N],rehss[N-2],rehvss[N-4],rehas[N-6],rehmks[N-6]; static reh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(reh_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;reh_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[REH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int reh_init(void){if(init)return -1;st.n_rehp=0;st.n_rehe=0;st.n_reh2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rehps[i].active=0;for(int i=0;i<N-2;i++)rehss[i].active=0;for(int i=0;i<N-4;i++)rehvss[i].active=0;for(int i=0;i<N-6;i++)rehas[i].active=0;for(int i=0;i<N-6;i++)rehmks[i].active=0;init=1;ps("[REH] Rehmannia initialized\n");return 0;}
int reh_planning(int t,int c,int a,int b,int d,int e,int y){return add(rehps,&st.n_rehp,&st.t_f1,N,t,c,a,b,d,e,y);}
int reh_execution(int t,int c,int a,int b,int d,int e,int y){return add(rehss,&st.n_rehe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int reh_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rehvss,&st.n_reh2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int reh_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rehas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int reh_market(int t,int c,int a,int b,int d,int e,int y){return add(rehmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void reh_report(void){ps("[REH] Rehpp: ");pi(st.n_rehp);ps(" PCS=");pi(st.t_f1);ps("\nRehe: ");pi(st.n_rehe);ps(" PCS=");pi(st.t_f2);ps("\nRehv: ");pi(st.n_reh2);ps(" PCS=");pi(st.t_f3);ps("\nRehc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void reh_state(void){ps("[REH] Rehpp=");pi(st.n_rehp);ps(" Rehe=");pi(st.n_rehe);ps(" Rehv=");pi(st.n_reh2);ps(" Rehc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rehmannia Admin Demo ===\n\n");reh_init();
ps("Rehmannia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;reh_planning(t,c,1178+(i*17),1167+(i*14),1147+(i*10),1129+(i*6),2020+(i%5));}
ps("\nRehmannia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;reh_execution(t,c,1167+(i*15),1156+(i*12),1138+(i*8),1125+(i*5),2021+(i%4));}
ps("\nRehmannia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;reh_evaluation(t,c,1159+(i*13),1148+(i*10),1132+(i*7),1121+(i*4),2022+(i%3));}
ps("\nRehmannia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;reh_accessory(t,c,1151+(i*11),1142+(i*9),1128+(i*6),1118+(i*3),2023+(i%2));}
ps("\nRehmannia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;reh_market(t,c,1145+(i*9),1136+(i*7),1123+(i*5),1115+(i*3),2024);}
ps("\n");reh_report();reh_state();ps("\n=== Demo Complete ===\n");return 0;}
