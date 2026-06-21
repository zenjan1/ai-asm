/* urticaria_admin: Urticaria management technology administration (v1.0)
 * Urticaria planning, urticaria execution, urticaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} urc_t;
typedef struct{int n_urp,n_ure,n_urv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} urc_state_t;
static urc_t urcps[N],urcss[N-2],urcvss[N-4],urcas[N-6],urcmks[N-6]; static urc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(urc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;urc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[URC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int urc_init(void){if(init)return -1;st.n_urp=0;st.n_ure=0;st.n_urv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)urcps[i].active=0;for(int i=0;i<N-2;i++)urcss[i].active=0;for(int i=0;i<N-4;i++)urcvss[i].active=0;for(int i=0;i<N-6;i++)urcas[i].active=0;for(int i=0;i<N-6;i++)urcmks[i].active=0;init=1;ps("[URC] Urticaria initialized\n");return 0;}
int urc_planning(int t,int c,int a,int b,int d,int e,int y){return add(urcps,&st.n_urp,&st.t_f1,N,t,c,a,b,d,e,y);}
int urc_execution(int t,int c,int a,int b,int d,int e,int y){return add(urcss,&st.n_ure,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int urc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(urcvss,&st.n_urv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int urc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(urcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int urc_market(int t,int c,int a,int b,int d,int e,int y){return add(urcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void urc_report(void){ps("[URC] Urp: ");pi(st.n_urp);ps(" PCS=");pi(st.t_f1);ps("\nUre: ");pi(st.n_ure);ps(" PCS=");pi(st.t_f2);ps("\nUrv: ");pi(st.n_urv);ps(" PCS=");pi(st.t_f3);ps("\nUrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void urc_state(void){ps("[URC] Urp=");pi(st.n_urp);ps(" Ure=");pi(st.n_ure);ps(" Urv=");pi(st.n_urv);ps(" Urc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Urticaria Admin Demo ===\n\n");urc_init();
ps("Urticaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;urc_planning(t,c,872+(i*17),861+(i*14),841+(i*10),823+(i*6),2020+(i%5));}
ps("\nUrticaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;urc_execution(t,c,861+(i*15),850+(i*12),832+(i*8),819+(i*5),2021+(i%4));}
ps("\nUrticaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;urc_evaluation(t,c,853+(i*13),842+(i*10),826+(i*7),815+(i*4),2022+(i%3));}
ps("\nUrticaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;urc_accessory(t,c,845+(i*11),836+(i*9),822+(i*6),812+(i*3),2023+(i%2));}
ps("\nUrticaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;urc_market(t,c,839+(i*9),830+(i*7),817+(i*5),809+(i*3),2024);}
ps("\n");urc_report();urc_state();ps("\n=== Demo Complete ===\n");return 0;}
