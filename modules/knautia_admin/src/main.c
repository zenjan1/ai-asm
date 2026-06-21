/* knautia_admin: Knautia management technology administration (v1.0)
 * Knautia planning, knautia execution, knautia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kn_t;
typedef struct{int n_knp,n_kne,n_knv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kn_state_t;
static kn_t knps[N],knss[N-2],knvss[N-4],knas[N-6],knmks[N-6]; static kn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KNA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kn_init(void){if(init)return -1;st.n_knp=0;st.n_kne=0;st.n_knv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)knps[i].active=0;for(int i=0;i<N-2;i++)knss[i].active=0;for(int i=0;i<N-4;i++)knvss[i].active=0;for(int i=0;i<N-6;i++)knas[i].active=0;for(int i=0;i<N-6;i++)knmks[i].active=0;init=1;ps("[KNA] Knautia initialized\n");return 0;}
int kn_planning(int t,int c,int a,int b,int d,int e,int y){return add(knps,&st.n_knp,&st.t_f1,N,t,c,a,b,d,e,y);}
int kn_execution(int t,int c,int a,int b,int d,int e,int y){return add(knss,&st.n_kne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(knvss,&st.n_knv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(knas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kn_market(int t,int c,int a,int b,int d,int e,int y){return add(knmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kn_report(void){ps("[KNA] Knp: ");pi(st.n_knp);ps(" PCS=");pi(st.t_f1);ps("\nKne: ");pi(st.n_kne);ps(" PCS=");pi(st.t_f2);ps("\nKnv: ");pi(st.n_knv);ps(" PCS=");pi(st.t_f3);ps("\nKnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kn_state(void){ps("[KNA] Knp=");pi(st.n_knp);ps(" Kne=");pi(st.n_kne);ps(" Knv=");pi(st.n_knv);ps(" Knc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Knautia Admin Demo ===\n\n");kn_init();
ps("Knautia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kn_planning(t,c,717+(i*17),706+(i*14),686+(i*10),668+(i*6),2020+(i%5));}
ps("\nKnautia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kn_execution(t,c,706+(i*15),695+(i*12),677+(i*8),664+(i*5),2021+(i%4));}
ps("\nKnautia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kn_evaluation(t,c,698+(i*13),687+(i*10),671+(i*7),660+(i*4),2022+(i%3));}
ps("\nKnautia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kn_accessory(t,c,690+(i*11),681+(i*9),667+(i*6),657+(i*3),2023+(i%2));}
ps("\nKnautia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kn_market(t,c,684+(i*9),675+(i*7),662+(i*5),654+(i*3),2024);}
ps("\n");kn_report();kn_state();ps("\n=== Demo Complete ===\n");return 0;}
