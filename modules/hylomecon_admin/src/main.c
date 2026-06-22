/* hylomecon_admin: Hylomecon management technology administration (v1.0)
 * Hylomecon planning, hylomecon execution, hylomecon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hyl_t;
typedef struct{int n_hylp,n_hyle,n_hyl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hyl_state_t;
static hyl_t hylps[N],hylss[N-2],hylvss[N-4],hylas[N-6],hylmks[N-6]; static hyl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hyl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hyl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hyl_init(void){if(init)return -1;st.n_hylp=0;st.n_hyle=0;st.n_hyl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hylps[i].active=0;for(int i=0;i<N-2;i++)hylss[i].active=0;for(int i=0;i<N-4;i++)hylvss[i].active=0;for(int i=0;i<N-6;i++)hylas[i].active=0;for(int i=0;i<N-6;i++)hylmks[i].active=0;init=1;ps("[HYL] Hylomecon initialized\n");return 0;}
int hyl_planning(int t,int c,int a,int b,int d,int e,int y){return add(hylps,&st.n_hylp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hyl_execution(int t,int c,int a,int b,int d,int e,int y){return add(hylss,&st.n_hyle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hyl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hylvss,&st.n_hyl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hyl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hylas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hyl_market(int t,int c,int a,int b,int d,int e,int y){return add(hylmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hyl_report(void){ps("[HYL] Hylpp: ");pi(st.n_hylp);ps(" PCS=");pi(st.t_f1);ps("\nHyle: ");pi(st.n_hyle);ps(" PCS=");pi(st.t_f2);ps("\nHylv: ");pi(st.n_hyl2);ps(" PCS=");pi(st.t_f3);ps("\nHylc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hyl_state(void){ps("[HYL] Hylpp=");pi(st.n_hylp);ps(" Hyle=");pi(st.n_hyle);ps(" Hylv=");pi(st.n_hyl2);ps(" Hylc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hylomecon Admin Demo ===\n\n");hyl_init();
ps("Hylomecon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hyl_planning(t,c,1149+(i*17),1138+(i*14),1118+(i*10),1100+(i*6),2020+(i%5));}
ps("\nHylomecon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hyl_execution(t,c,1138+(i*15),1127+(i*12),1109+(i*8),1096+(i*5),2021+(i%4));}
ps("\nHylomecon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hyl_evaluation(t,c,1130+(i*13),1119+(i*10),1103+(i*7),1092+(i*4),2022+(i%3));}
ps("\nHylomecon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyl_accessory(t,c,1122+(i*11),1113+(i*9),1099+(i*6),1089+(i*3),2023+(i%2));}
ps("\nHylomecon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyl_market(t,c,1116+(i*9),1107+(i*7),1094+(i*5),1086+(i*3),2024);}
ps("\n");hyl_report();hyl_state();ps("\n=== Demo Complete ===\n");return 0;}
