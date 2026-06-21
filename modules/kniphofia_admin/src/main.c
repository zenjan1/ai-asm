/* kniphofia_admin: Kniphofia management technology administration (v1.0)
 * Kniphofia planning, kniphofia execution, kniphofia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kni_t;
typedef struct{int n_knip,n_knie,n_kni2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kni_state_t;
static kni_t knips[N],kniss[N-2],knivss[N-4],kniac[N-6],knimks[N-6]; static kni_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kni_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kni_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KNI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kni_init(void){if(init)return -1;st.n_knip=0;st.n_knie=0;st.n_kni2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)knips[i].active=0;for(int i=0;i<N-2;i++)kniss[i].active=0;for(int i=0;i<N-4;i++)knivss[i].active=0;for(int i=0;i<N-6;i++)kniac[i].active=0;for(int i=0;i<N-6;i++)knimks[i].active=0;init=1;ps("[KNI] Kniphofia initialized\n");return 0;}
int kni_planning(int t,int c,int a,int b,int d,int e,int y){return add(knips,&st.n_knip,&st.t_f1,N,t,c,a,b,d,e,y);}
int kni_execution(int t,int c,int a,int b,int d,int e,int y){return add(kniss,&st.n_knie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kni_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(knivss,&st.n_kni2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kni_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kniac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kni_market(int t,int c,int a,int b,int d,int e,int y){return add(knimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kni_report(void){ps("[KNI] Knip: ");pi(st.n_knip);ps(" PCS=");pi(st.t_f1);ps("\nKnie: ");pi(st.n_knie);ps(" PCS=");pi(st.t_f2);ps("\nKniv: ");pi(st.n_kni2);ps(" PCS=");pi(st.t_f3);ps("\nKnic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kni_state(void){ps("[KNI] Knip=");pi(st.n_knip);ps(" Knie=");pi(st.n_knie);ps(" Kniv=");pi(st.n_kni2);ps(" Knic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kniphofia Admin Demo ===\n\n");kni_init();
ps("Kniphofia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kni_planning(t,c,931+(i*17),920+(i*14),900+(i*10),882+(i*6),2020+(i%5));}
ps("\nKniphofia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kni_execution(t,c,920+(i*15),909+(i*12),891+(i*8),878+(i*5),2021+(i%4));}
ps("\nKniphofia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kni_evaluation(t,c,912+(i*13),901+(i*10),885+(i*7),874+(i*4),2022+(i%3));}
ps("\nKniphofia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kni_accessory(t,c,904+(i*11),895+(i*9),881+(i*6),871+(i*3),2023+(i%2));}
ps("\nKniphofia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kni_market(t,c,898+(i*9),889+(i*7),876+(i*5),868+(i*3),2024);}
ps("\n");kni_report();kni_state();ps("\n=== Demo Complete ===\n");return 0;}
