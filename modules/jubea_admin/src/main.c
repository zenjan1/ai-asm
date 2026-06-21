/* jubea_admin: Jubea management technology administration (v1.0)
 * Jubea planning, jubea execution, jubea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jub_t;
typedef struct{int n_jubp,n_jube,n_jub2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jub_state_t;
static jub_t jubps[N],jubss[N-2],jubvss[N-4],jubas[N-6],jubmks[N-6]; static jub_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jub_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jub_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JUB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jub_init(void){if(init)return -1;st.n_jubp=0;st.n_jube=0;st.n_jub2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jubps[i].active=0;for(int i=0;i<N-2;i++)jubss[i].active=0;for(int i=0;i<N-4;i++)jubvss[i].active=0;for(int i=0;i<N-6;i++)jubas[i].active=0;for(int i=0;i<N-6;i++)jubmks[i].active=0;init=1;ps("[JUB] Jubea initialized\n");return 0;}
int jub_planning(int t,int c,int a,int b,int d,int e,int y){return add(jubps,&st.n_jubp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jub_execution(int t,int c,int a,int b,int d,int e,int y){return add(jubss,&st.n_jube,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jub_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jubvss,&st.n_jub2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jub_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jubas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jub_market(int t,int c,int a,int b,int d,int e,int y){return add(jubmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jub_report(void){ps("[JUB] Jubp: ");pi(st.n_jubp);ps(" PCS=");pi(st.t_f1);ps("\nJube: ");pi(st.n_jube);ps(" PCS=");pi(st.t_f2);ps("\nJubv: ");pi(st.n_jub2);ps(" PCS=");pi(st.t_f3);ps("\nJubc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jub_state(void){ps("[JUB] Jubp=");pi(st.n_jubp);ps(" Jube=");pi(st.n_jube);ps(" Jubv=");pi(st.n_jub2);ps(" Jubc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jubea Admin Demo ===\n\n");jub_init();
ps("Jubea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jub_planning(t,c,1031+(i*17),1020+(i*14),1000+(i*10),982+(i*6),2020+(i%5));}
ps("\nJubea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jub_execution(t,c,1020+(i*15),1009+(i*12),991+(i*8),978+(i*5),2021+(i%4));}
ps("\nJubea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jub_evaluation(t,c,1012+(i*13),1001+(i*10),985+(i*7),974+(i*4),2022+(i%3));}
ps("\nJubea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jub_accessory(t,c,1004+(i*11),995+(i*9),981+(i*6),971+(i*3),2023+(i%2));}
ps("\nJubea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jub_market(t,c,998+(i*9),989+(i*7),976+(i*5),968+(i*3),2024);}
ps("\n");jub_report();jub_state();ps("\n=== Demo Complete ===\n");return 0;}
