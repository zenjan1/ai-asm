/* alehoof_admin: Alehoof management technology administration (v1.0)
 * Alehoof planning, alehoof execution, alehoof evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ah_t;
typedef struct{int n_ahp,n_ah_e,n_ah_v,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ah_state_t;
static ah_t ahps[N],ahss[N-2],ahvss[N-4],ahas[N-6],ahmks[N-6]; static ah_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ah_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ah_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AHF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ah_init(void){if(init)return -1;st.n_ahp=0;st.n_ah_e=0;st.n_ah_v=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ahps[i].active=0;for(int i=0;i<N-2;i++)ahss[i].active=0;for(int i=0;i<N-4;i++)ahvss[i].active=0;for(int i=0;i<N-6;i++)ahas[i].active=0;for(int i=0;i<N-6;i++)ahmks[i].active=0;init=1;ps("[AHF] Alehoof initialized\n");return 0;}
int ah_planning(int t,int c,int a,int b,int d,int e,int y){return add(ahps,&st.n_ahp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ah_execution(int t,int c,int a,int b,int d,int e,int y){return add(ahss,&st.n_ah_e,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ah_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ahvss,&st.n_ah_v,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ah_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ahas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ah_market(int t,int c,int a,int b,int d,int e,int y){return add(ahmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ah_report(void){ps("[AHF] Ahp: ");pi(st.n_ahp);ps(" PCS=");pi(st.t_f1);ps("\nAh-e: ");pi(st.n_ah_e);ps(" PCS=");pi(st.t_f2);ps("\nAh-v: ");pi(st.n_ah_v);ps(" PCS=");pi(st.t_f3);ps("\nAh-c: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ah_state(void){ps("[AHF] Ahp=");pi(st.n_ahp);ps(" Ah-e=");pi(st.n_ah_e);ps(" Ah-v=");pi(st.n_ah_v);ps(" Ah-c=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Alehoof Admin Demo ===\n\n");ah_init();
ps("Alehoof planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ah_planning(t,c,691+(i*17),680+(i*14),660+(i*10),642+(i*6),2020+(i%5));}
ps("\nAlehoof execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ah_execution(t,c,680+(i*15),669+(i*12),651+(i*8),638+(i*5),2021+(i%4));}
ps("\nAlehoof evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ah_evaluation(t,c,672+(i*13),661+(i*10),645+(i*7),634+(i*4),2022+(i%3));}
ps("\nAlehoof accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ah_accessory(t,c,664+(i*11),655+(i*9),641+(i*6),631+(i*3),2023+(i%2));}
ps("\nAlehoof marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ah_market(t,c,658+(i*9),649+(i*7),636+(i*5),628+(i*3),2024);}
ps("\n");ah_report();ah_state();ps("\n=== Demo Complete ===\n");return 0;}
