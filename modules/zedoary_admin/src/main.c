/* zedoary_admin: Zedoary management technology administration (v1.0)
 * Zedoary planning, zedoary execution, zedoary evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zed_t;
typedef struct{int n_zedp,n_zede,n_zed2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zed_state_t;
static zed_t zedps[N],zedss[N-2],zedvss[N-4],zedas[N-6],zedmks[N-6]; static zed_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zed_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zed_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZED] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zed_init(void){if(init)return -1;st.n_zedp=0;st.n_zede=0;st.n_zed2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zedps[i].active=0;for(int i=0;i<N-2;i++)zedss[i].active=0;for(int i=0;i<N-4;i++)zedvss[i].active=0;for(int i=0;i<N-6;i++)zedas[i].active=0;for(int i=0;i<N-6;i++)zedmks[i].active=0;init=1;ps("[ZED] Zedoary initialized\n");return 0;}
int zed_planning(int t,int c,int a,int b,int d,int e,int y){return add(zedps,&st.n_zedp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zed_execution(int t,int c,int a,int b,int d,int e,int y){return add(zedss,&st.n_zede,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zed_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zedvss,&st.n_zed2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zed_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zedas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zed_market(int t,int c,int a,int b,int d,int e,int y){return add(zedmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zed_report(void){ps("[ZED] Zedp: ");pi(st.n_zedp);ps(" PCS=");pi(st.t_f1);ps("\nZede: ");pi(st.n_zede);ps(" PCS=");pi(st.t_f2);ps("\nZedv: ");pi(st.n_zed2);ps(" PCS=");pi(st.t_f3);ps("\nZedc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zed_state(void){ps("[ZED] Zedp=");pi(st.n_zedp);ps(" Zede=");pi(st.n_zede);ps(" Zedv=");pi(st.n_zed2);ps(" Zedc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zedoary Admin Demo ===\n\n");zed_init();
ps("Zedoary planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zed_planning(t,c,965+(i*17),954+(i*14),934+(i*10),916+(i*6),2020+(i%5));}
ps("\nZedoary execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zed_execution(t,c,954+(i*15),943+(i*12),925+(i*8),912+(i*5),2021+(i%4));}
ps("\nZedoary evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zed_evaluation(t,c,946+(i*13),935+(i*10),919+(i*7),908+(i*4),2022+(i%3));}
ps("\nZedoary accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zed_accessory(t,c,938+(i*11),929+(i*9),915+(i*6),905+(i*3),2023+(i%2));}
ps("\nZedoary marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zed_market(t,c,932+(i*9),923+(i*7),910+(i*5),902+(i*3),2024);}
ps("\n");zed_report();zed_state();ps("\n=== Demo Complete ===\n");return 0;}
