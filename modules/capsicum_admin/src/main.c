/* capsicum_admin: Capsicum management technology administration (v1.0)
 * Capsicum planning, capsicum execution, capsicum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cap_t;
typedef struct{int n_capp,n_cape,n_cap2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cap_state_t;
static cap_t capps[N],capss[N-2],capvss[N-4],capas[N-6],capmks[N-6]; static cap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cap_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cap_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cap_init(void){if(init)return -1;st.n_capp=0;st.n_cape=0;st.n_cap2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)capps[i].active=0;for(int i=0;i<N-2;i++)capss[i].active=0;for(int i=0;i<N-4;i++)capvss[i].active=0;for(int i=0;i<N-6;i++)capas[i].active=0;for(int i=0;i<N-6;i++)capmks[i].active=0;init=1;ps("[CAP] Capsicum initialized\n");return 0;}
int cap_planning(int t,int c,int a,int b,int d,int e,int y){return add(capps,&st.n_capp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cap_execution(int t,int c,int a,int b,int d,int e,int y){return add(capss,&st.n_cape,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cap_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(capvss,&st.n_cap2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cap_accessory(int t,int c,int a,int b,int d,int e,int y){return add(capas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cap_market(int t,int c,int a,int b,int d,int e,int y){return add(capmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cap_report(void){ps("[CAP] Cappp: ");pi(st.n_capp);ps(" PCS=");pi(st.t_f1);ps("\nCappe: ");pi(st.n_cape);ps(" PCS=");pi(st.t_f2);ps("\nCapv: ");pi(st.n_cap2);ps(" PCS=");pi(st.t_f3);ps("\nCapc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cap_state(void){ps("[CAP] Cappp=");pi(st.n_capp);ps(" Cappe=");pi(st.n_cape);ps(" Capv=");pi(st.n_cap2);ps(" Capc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Capsicum Admin Demo ===\n\n");cap_init();
ps("Capsicum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cap_planning(t,c,1226+(i*17),1215+(i*14),1195+(i*10),1177+(i*6),2020+(i%5));}
ps("\nCapsicum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cap_execution(t,c,1215+(i*15),1204+(i*12),1186+(i*8),1173+(i*5),2021+(i%4));}
ps("\nCapsicum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cap_evaluation(t,c,1207+(i*13),1196+(i*10),1180+(i*7),1169+(i*4),2022+(i%3));}
ps("\nCapsicum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cap_accessory(t,c,1199+(i*11),1190+(i*9),1176+(i*6),1166+(i*3),2023+(i%2));}
ps("\nCapsicum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cap_market(t,c,1193+(i*9),1184+(i*7),1171+(i*5),1163+(i*3),2024);}
ps("\n");cap_report();cap_state();ps("\n=== Demo Complete ===\n");return 0;}
