/* syringa_admin: Syringa management technology administration (v1.0)
 * Syringa planning, syringa execution, syringa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} syri_t;
typedef struct{int n_syrip,n_syrie,n_syri2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} syri_state_t;
static syri_t syris[N],syriness[N-2],syrivss[N-4],syrias[N-6],syranks[N-6]; static syri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(syri_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;syri_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SYRI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int syri_init(void){if(init)return -1;st.n_syrip=0;st.n_syrie=0;st.n_syri2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)syris[i].active=0;for(int i=0;i<N-2;i++)syriness[i].active=0;for(int i=0;i<N-4;i++)syrivss[i].active=0;for(int i=0;i<N-6;i++)syrias[i].active=0;for(int i=0;i<N-6;i++)syranks[i].active=0;init=1;ps("[SYRI] Syringa initialized\n");return 0;}
int syri_planning(int t,int c,int a,int b,int d,int e,int y){return add(syris,&st.n_syrip,&st.t_f1,N,t,c,a,b,d,e,y);}
int syri_execution(int t,int c,int a,int b,int d,int e,int y){return add(syriness,&st.n_syrie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int syri_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(syrivss,&st.n_syri2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int syri_accessory(int t,int c,int a,int b,int d,int e,int y){return add(syrias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int syri_market(int t,int c,int a,int b,int d,int e,int y){return add(syranks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void syri_report(void){ps("[SYRI] Syrip: ");pi(st.n_syrip);ps(" PCS=");pi(st.t_f1);ps("\nSyrie: ");pi(st.n_syrie);ps(" PCS=");pi(st.t_f2);ps("\nSyri2: ");pi(st.n_syri2);ps(" PCS=");pi(st.t_f3);ps("\nSyric: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void syri_state(void){ps("[SYRI] Syrip=");pi(st.n_syrip);ps(" Syrie=");pi(st.n_syrie);ps(" Syri2=");pi(st.n_syri2);ps(" Syric=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Syringa Admin Demo ===\n\n");syri_init();
ps("Syringa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;syri_planning(t,c,1345+(i*17),1334+(i*14),1314+(i*10),1296+(i*6),2020+(i%5));}
ps("\nSyringa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;syri_execution(t,c,1334+(i*15),1323+(i*12),1305+(i*8),1292+(i*5),2021+(i%4));}
ps("\nSyringa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;syri_evaluation(t,c,1326+(i*13),1315+(i*10),1299+(i*7),1288+(i*4),2022+(i%3));}
ps("\nSyringa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;syri_accessory(t,c,1318+(i*11),1309+(i*9),1295+(i*6),1285+(i*3),2023+(i%2));}
ps("\nSyringa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;syri_market(t,c,1312+(i*9),1303+(i*7),1290+(i*5),1282+(i*3),2024);}
ps("\n");syri_report();syri_state();ps("\n=== Demo Complete ===\n");return 0;}
