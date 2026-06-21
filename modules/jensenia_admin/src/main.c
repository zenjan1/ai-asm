/* jensenia_admin: Jensenia management technology administration (v1.0)
 * Jensenia planning, jensenia execution, jensenia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jen_t;
typedef struct{int n_jenp,n_jene,n_jen2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jen_state_t;
static jen_t jenps[N],jenss[N-2],jenvss[N-4],jenas[N-6],jenmks[N-6]; static jen_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jen_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jen_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JEN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jen_init(void){if(init)return -1;st.n_jenp=0;st.n_jene=0;st.n_jen2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jenps[i].active=0;for(int i=0;i<N-2;i++)jenss[i].active=0;for(int i=0;i<N-4;i++)jenvss[i].active=0;for(int i=0;i<N-6;i++)jenas[i].active=0;for(int i=0;i<N-6;i++)jenmks[i].active=0;init=1;ps("[JEN] Jensenia initialized\n");return 0;}
int jen_planning(int t,int c,int a,int b,int d,int e,int y){return add(jenps,&st.n_jenp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jen_execution(int t,int c,int a,int b,int d,int e,int y){return add(jenss,&st.n_jene,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jen_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jenvss,&st.n_jen2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jen_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jenas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jen_market(int t,int c,int a,int b,int d,int e,int y){return add(jenmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jen_report(void){ps("[JEN] Jenp: ");pi(st.n_jenp);ps(" PCS=");pi(st.t_f1);ps("\nJene: ");pi(st.n_jene);ps(" PCS=");pi(st.t_f2);ps("\nJenv: ");pi(st.n_jen2);ps(" PCS=");pi(st.t_f3);ps("\nJenc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jen_state(void){ps("[JEN] Jenp=");pi(st.n_jenp);ps(" Jene=");pi(st.n_jene);ps(" Jenv=");pi(st.n_jen2);ps(" Jenc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jensenia Admin Demo ===\n\n");jen_init();
ps("Jensenia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jen_planning(t,c,1030+(i*17),1019+(i*14),999+(i*10),981+(i*6),2020+(i%5));}
ps("\nJensenia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jen_execution(t,c,1019+(i*15),1008+(i*12),990+(i*8),977+(i*5),2021+(i%4));}
ps("\nJensenia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jen_evaluation(t,c,1011+(i*13),1000+(i*10),984+(i*7),973+(i*4),2022+(i%3));}
ps("\nJensenia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jen_accessory(t,c,1003+(i*11),994+(i*9),980+(i*6),970+(i*3),2023+(i%2));}
ps("\nJensenia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jen_market(t,c,997+(i*9),988+(i*7),975+(i*5),967+(i*3),2024);}
ps("\n");jen_report();jen_state();ps("\n=== Demo Complete ===\n");return 0;}
