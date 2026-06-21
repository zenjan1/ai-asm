/* ledderia_admin: Ledderia management technology administration (v1.0)
 * Ledderia planning, ledderia execution, ledderia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} led_t;
typedef struct{int n_ledp,n_lede,n_led2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} led_state_t;
static led_t ledps[N],ledss[N-2],ledvss[N-4],ledas[N-6],ledmks[N-6]; static led_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(led_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;led_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LED] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int led_init(void){if(init)return -1;st.n_ledp=0;st.n_lede=0;st.n_led2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ledps[i].active=0;for(int i=0;i<N-2;i++)ledss[i].active=0;for(int i=0;i<N-4;i++)ledvss[i].active=0;for(int i=0;i<N-6;i++)ledas[i].active=0;for(int i=0;i<N-6;i++)ledmks[i].active=0;init=1;ps("[LED] Ledderia initialized\n");return 0;}
int led_planning(int t,int c,int a,int b,int d,int e,int y){return add(ledps,&st.n_ledp,&st.t_f1,N,t,c,a,b,d,e,y);}
int led_execution(int t,int c,int a,int b,int d,int e,int y){return add(ledss,&st.n_lede,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int led_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ledvss,&st.n_led2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int led_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ledas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int led_market(int t,int c,int a,int b,int d,int e,int y){return add(ledmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void led_report(void){ps("[LED] Ledp: ");pi(st.n_ledp);ps(" PCS=");pi(st.t_f1);ps("\nLede: ");pi(st.n_lede);ps(" PCS=");pi(st.t_f2);ps("\nLedv: ");pi(st.n_led2);ps(" PCS=");pi(st.t_f3);ps("\nLedc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void led_state(void){ps("[LED] Ledp=");pi(st.n_ledp);ps(" Lede=");pi(st.n_lede);ps(" Ledv=");pi(st.n_led2);ps(" Ledc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ledderia Admin Demo ===\n\n");led_init();
ps("Ledderia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;led_planning(t,c,1049+(i*17),1038+(i*14),1018+(i*10),1000+(i*6),2020+(i%5));}
ps("\nLedderia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;led_execution(t,c,1038+(i*15),1027+(i*12),1009+(i*8),996+(i*5),2021+(i%4));}
ps("\nLedderia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;led_evaluation(t,c,1030+(i*13),1019+(i*10),1003+(i*7),992+(i*4),2022+(i%3));}
ps("\nLedderia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;led_accessory(t,c,1022+(i*11),1013+(i*9),999+(i*6),989+(i*3),2023+(i%2));}
ps("\nLedderia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;led_market(t,c,1016+(i*9),1007+(i*7),994+(i*5),986+(i*3),2024);}
ps("\n");led_report();led_state();ps("\n=== Demo Complete ===\n");return 0;}
