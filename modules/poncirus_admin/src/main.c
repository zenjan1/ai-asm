/* poncirus_admin: Poncirus management technology administration (v1.0)
 * Poncirus planning, poncirus execution, poncirus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pon_t;
typedef struct{int n_ponp,n_pone,n_pon2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pon_state_t;
static pon_t ponps[N],ponss[N-2],ponvss[N-4],ponas[N-6],ponmks[N-6]; static pon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pon_init(void){if(init)return -1;st.n_ponp=0;st.n_pone=0;st.n_pon2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ponps[i].active=0;for(int i=0;i<N-2;i++)ponss[i].active=0;for(int i=0;i<N-4;i++)ponvss[i].active=0;for(int i=0;i<N-6;i++)ponas[i].active=0;for(int i=0;i<N-6;i++)ponmks[i].active=0;init=1;ps("[PON] Poncirus initialized\n");return 0;}
int pon_planning(int t,int c,int a,int b,int d,int e,int y){return add(ponps,&st.n_ponp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pon_execution(int t,int c,int a,int b,int d,int e,int y){return add(ponss,&st.n_pone,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pon_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ponvss,&st.n_pon2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pon_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ponas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pon_market(int t,int c,int a,int b,int d,int e,int y){return add(ponmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pon_report(void){ps("[PON] ponp: ");pi(st.n_ponp);ps(" PCS=");pi(st.t_f1);ps("\npone: ");pi(st.n_pone);ps(" PCS=");pi(st.t_f2);ps("\nponv: ");pi(st.n_pon2);ps(" PCS=");pi(st.t_f3);ps("\nponc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pon_state(void){ps("[PON] ponp=");pi(st.n_ponp);ps(" pone=");pi(st.n_pone);ps(" ponv=");pi(st.n_pon2);ps(" ponc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Poncirus Admin Demo ===\n\n");pon_init();
ps("Poncirus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pon_planning(t,c,1086+(i*17),1075+(i*14),1055+(i*10),1037+(i*6),2020+(i%5));}
ps("\nPoncirus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pon_execution(t,c,1075+(i*15),1064+(i*12),1046+(i*8),1033+(i*5),2021+(i%4));}
ps("\nPoncirus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pon_evaluation(t,c,1067+(i*13),1056+(i*10),1040+(i*7),1029+(i*4),2022+(i%3));}
ps("\nPoncirus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pon_accessory(t,c,1059+(i*11),1050+(i*9),1036+(i*6),1026+(i*3),2023+(i%2));}
ps("\nPoncirus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pon_market(t,c,1053+(i*9),1044+(i*7),1031+(i*5),1023+(i*3),2024);}
ps("\n");pon_report();pon_state();ps("\n=== Demo Complete ===\n");return 0;}
