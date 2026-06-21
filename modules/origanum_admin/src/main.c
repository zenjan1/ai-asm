/* origanum_admin: Origanum management technology administration (v1.0)
 * Origanum planning, origanum execution, origanum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ori_t;
typedef struct{int n_orip,n_orie,n_ori2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ori_state_t;
static ori_t orips[N],oriss[N-2],orivss[N-4],orias[N-6],orimks[N-6]; static ori_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ori_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ori_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ORI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ori_init(void){if(init)return -1;st.n_orip=0;st.n_orie=0;st.n_ori2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)orips[i].active=0;for(int i=0;i<N-2;i++)oriss[i].active=0;for(int i=0;i<N-4;i++)orivss[i].active=0;for(int i=0;i<N-6;i++)orias[i].active=0;for(int i=0;i<N-6;i++)orimks[i].active=0;init=1;ps("[ORI] Origanum initialized\n");return 0;}
int ori_planning(int t,int c,int a,int b,int d,int e,int y){return add(orips,&st.n_orip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ori_execution(int t,int c,int a,int b,int d,int e,int y){return add(oriss,&st.n_orie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ori_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(orivss,&st.n_ori2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ori_accessory(int t,int c,int a,int b,int d,int e,int y){return add(orias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ori_market(int t,int c,int a,int b,int d,int e,int y){return add(orimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ori_report(void){ps("[ORI] orip: ");pi(st.n_orip);ps(" PCS=");pi(st.t_f1);ps("\norie: ");pi(st.n_orie);ps(" PCS=");pi(st.t_f2);ps("\noriv: ");pi(st.n_ori2);ps(" PCS=");pi(st.t_f3);ps("\noric: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ori_state(void){ps("[ORI] orip=");pi(st.n_orip);ps(" orie=");pi(st.n_orie);ps(" oriv=");pi(st.n_ori2);ps(" oric=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Origanum Admin Demo ===\n\n");ori_init();
ps("Origanum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ori_planning(t,c,1078+(i*17),1067+(i*14),1047+(i*10),1029+(i*6),2020+(i%5));}
ps("\nOriganum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ori_execution(t,c,1067+(i*15),1056+(i*12),1038+(i*8),1025+(i*5),2021+(i%4));}
ps("\nOriganum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ori_evaluation(t,c,1059+(i*13),1048+(i*10),1032+(i*7),1021+(i*4),2022+(i%3));}
ps("\nOriganum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ori_accessory(t,c,1051+(i*11),1042+(i*9),1028+(i*6),1018+(i*3),2023+(i%2));}
ps("\nOriganum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ori_market(t,c,1045+(i*9),1036+(i*7),1023+(i*5),1015+(i*3),2024);}
ps("\n");ori_report();ori_state();ps("\n=== Demo Complete ===\n");return 0;}
