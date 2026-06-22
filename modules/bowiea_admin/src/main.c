/* bowiea_admin: Bowiea management technology administration (v1.0)
 * Bowiea planning, bowiea execution, bowiea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bow_t;
typedef struct{int n_bowp,n_bowe,n_bow2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bow_state_t;
static bow_t bowps[N],bowss[N-2],bowvss[N-4],bowas[N-6],bowmks[N-6]; static bow_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bow_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bow_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BOW] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bow_init(void){if(init)return -1;st.n_bowp=0;st.n_bowe=0;st.n_bow2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bowps[i].active=0;for(int i=0;i<N-2;i++)bowss[i].active=0;for(int i=0;i<N-4;i++)bowvss[i].active=0;for(int i=0;i<N-6;i++)bowas[i].active=0;for(int i=0;i<N-6;i++)bowmks[i].active=0;init=1;ps("[BOW] Bowiea initialized\n");return 0;}
int bow_planning(int t,int c,int a,int b,int d,int e,int y){return add(bowps,&st.n_bowp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bow_execution(int t,int c,int a,int b,int d,int e,int y){return add(bowss,&st.n_bowe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bow_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bowvss,&st.n_bow2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bow_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bowas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bow_market(int t,int c,int a,int b,int d,int e,int y){return add(bowmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bow_report(void){ps("[BOW] Bowpp: ");pi(st.n_bowp);ps(" PCS=");pi(st.t_f1);ps("\nBowpe: ");pi(st.n_bowe);ps(" PCS=");pi(st.t_f2);ps("\nBowv: ");pi(st.n_bow2);ps(" PCS=");pi(st.t_f3);ps("\nBowc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bow_state(void){ps("[BOW] Bowpp=");pi(st.n_bowp);ps(" Bowpe=");pi(st.n_bowe);ps(" Bowv=");pi(st.n_bow2);ps(" Bowc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bowiea Admin Demo ===\n\n");bow_init();
ps("Bowiea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bow_planning(t,c,1220+(i*17),1209+(i*14),1189+(i*10),1171+(i*6),2020+(i%5));}
ps("\nBowiea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bow_execution(t,c,1209+(i*15),1198+(i*12),1180+(i*8),1167+(i*5),2021+(i%4));}
ps("\nBowiea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bow_evaluation(t,c,1201+(i*13),1190+(i*10),1174+(i*7),1163+(i*4),2022+(i%3));}
ps("\nBowiea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bow_accessory(t,c,1193+(i*11),1184+(i*9),1170+(i*6),1160+(i*3),2023+(i%2));}
ps("\nBowiea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bow_market(t,c,1187+(i*9),1178+(i*7),1165+(i*5),1157+(i*3),2024);}
ps("\n");bow_report();bow_state();ps("\n=== Demo Complete ===\n");return 0;}
