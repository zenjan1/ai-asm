/* valentini_admin: Valentini management technology administration (v1.0)
 * Valentini planning, valentini execution, valentini evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} val_t;
typedef struct{int n_valp,n_vale,n_val2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} val_state_t;
static val_t valps[N],valss[N-2],valvss[N-4],valas[N-6],valmks[N-6]; static val_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(val_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;val_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int val_init(void){if(init)return -1;st.n_valp=0;st.n_vale=0;st.n_val2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)valps[i].active=0;for(int i=0;i<N-2;i++)valss[i].active=0;for(int i=0;i<N-4;i++)valvss[i].active=0;for(int i=0;i<N-6;i++)valas[i].active=0;for(int i=0;i<N-6;i++)valmks[i].active=0;init=1;ps("[VAL] Valentini initialized\n");return 0;}
int val_planning(int t,int c,int a,int b,int d,int e,int y){return add(valps,&st.n_valp,&st.t_f1,N,t,c,a,b,d,e,y);}
int val_execution(int t,int c,int a,int b,int d,int e,int y){return add(valss,&st.n_vale,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int val_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(valvss,&st.n_val2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int val_accessory(int t,int c,int a,int b,int d,int e,int y){return add(valas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int val_market(int t,int c,int a,int b,int d,int e,int y){return add(valmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void val_report(void){ps("[VAL] Valpp: ");pi(st.n_valp);ps(" PCS=");pi(st.t_f1);ps("\nVale: ");pi(st.n_vale);ps(" PCS=");pi(st.t_f2);ps("\nValv: ");pi(st.n_val2);ps(" PCS=");pi(st.t_f3);ps("\nValc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void val_state(void){ps("[VAL] Valpp=");pi(st.n_valp);ps(" Vale=");pi(st.n_vale);ps(" Valv=");pi(st.n_val2);ps(" Valc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Valentini Admin Demo ===\n\n");val_init();
ps("Valentini planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;val_planning(t,c,1201+(i*17),1190+(i*14),1170+(i*10),1152+(i*6),2020+(i%5));}
ps("\nValentini execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;val_execution(t,c,1190+(i*15),1179+(i*12),1161+(i*8),1148+(i*5),2021+(i%4));}
ps("\nValentini evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;val_evaluation(t,c,1182+(i*13),1171+(i*10),1155+(i*7),1144+(i*4),2022+(i%3));}
ps("\nValentini accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;val_accessory(t,c,1174+(i*11),1165+(i*9),1151+(i*6),1141+(i*3),2023+(i%2));}
ps("\nValentini marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;val_market(t,c,1168+(i*9),1159+(i*7),1146+(i*5),1138+(i*3),2024);}
ps("\n");val_report();val_state();ps("\n=== Demo Complete ===\n");return 0;}
