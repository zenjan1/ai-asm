/* tweedia_admin: Tweedia management technology administration (v1.0)
 * Tweedia planning, tweedia execution, tweedia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} twe_t;
typedef struct{int n_twep,n_twee,n_twe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} twe_state_t;
static twe_t tweps[N],twess[N-2],twevss[N-4],tweac[N-6],twemks[N-6]; static twe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(twe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;twe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TWE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int twe_init(void){if(init)return -1;st.n_twep=0;st.n_twee=0;st.n_twe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tweps[i].active=0;for(int i=0;i<N-2;i++)twess[i].active=0;for(int i=0;i<N-4;i++)twevss[i].active=0;for(int i=0;i<N-6;i++)tweac[i].active=0;for(int i=0;i<N-6;i++)twemks[i].active=0;init=1;ps("[TWE] Tweedia initialized\n");return 0;}
int twe_planning(int t,int c,int a,int b,int d,int e,int y){return add(tweps,&st.n_twep,&st.t_f1,N,t,c,a,b,d,e,y);}
int twe_execution(int t,int c,int a,int b,int d,int e,int y){return add(twess,&st.n_twee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int twe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(twevss,&st.n_twe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int twe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tweac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int twe_market(int t,int c,int a,int b,int d,int e,int y){return add(twemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void twe_report(void){ps("[TWE] Twep: ");pi(st.n_twep);ps(" PCS=");pi(st.t_f1);ps("\nTwee: ");pi(st.n_twee);ps(" PCS=");pi(st.t_f2);ps("\nTwev: ");pi(st.n_twe2);ps(" PCS=");pi(st.t_f3);ps("\nTwec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void twe_state(void){ps("[TWE] Twep=");pi(st.n_twep);ps(" Twee=");pi(st.n_twee);ps(" Twev=");pi(st.n_twe2);ps(" Twec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tweedia Admin Demo ===\n\n");twe_init();
ps("Tweedia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;twe_planning(t,c,952+(i*17),941+(i*14),921+(i*10),903+(i*6),2020+(i%5));}
ps("\nTweedia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;twe_execution(t,c,941+(i*15),930+(i*12),912+(i*8),899+(i*5),2021+(i%4));}
ps("\nTweedia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;twe_evaluation(t,c,933+(i*13),922+(i*10),906+(i*7),895+(i*4),2022+(i%3));}
ps("\nTweedia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;twe_accessory(t,c,925+(i*11),916+(i*9),902+(i*6),892+(i*3),2023+(i%2));}
ps("\nTweedia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;twe_market(t,c,919+(i*9),910+(i*7),897+(i*5),889+(i*3),2024);}
ps("\n");twe_report();twe_state();ps("\n=== Demo Complete ===\n");return 0;}
