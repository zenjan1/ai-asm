/* vanilla_admin: Vanilla management technology administration (v1.0)
 * Vanilla planning, vanilla execution, vanilla evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} van_t;
typedef struct{int n_vanp,n_vane,n_van2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} van_state_t;
static van_t vanps[N],vanss[N-2],vanvss[N-4],vanas[N-6],vanmks[N-6]; static van_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(van_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;van_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int van_init(void){if(init)return -1;st.n_vanp=0;st.n_vane=0;st.n_van2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vanps[i].active=0;for(int i=0;i<N-2;i++)vanss[i].active=0;for(int i=0;i<N-4;i++)vanvss[i].active=0;for(int i=0;i<N-6;i++)vanas[i].active=0;for(int i=0;i<N-6;i++)vanmks[i].active=0;init=1;ps("[VAN] Vanilla initialized\n");return 0;}
int van_planning(int t,int c,int a,int b,int d,int e,int y){return add(vanps,&st.n_vanp,&st.t_f1,N,t,c,a,b,d,e,y);}
int van_execution(int t,int c,int a,int b,int d,int e,int y){return add(vanss,&st.n_vane,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int van_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vanvss,&st.n_van2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int van_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vanas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int van_market(int t,int c,int a,int b,int d,int e,int y){return add(vanmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void van_report(void){ps("[VAN] Vanp: ");pi(st.n_vanp);ps(" PCS=");pi(st.t_f1);ps("\nVane: ");pi(st.n_vane);ps(" PCS=");pi(st.t_f2);ps("\nVanv: ");pi(st.n_van2);ps(" PCS=");pi(st.t_f3);ps("\nVanc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void van_state(void){ps("[VAN] Vanp=");pi(st.n_vanp);ps(" Vane=");pi(st.n_vane);ps(" Vanv=");pi(st.n_van2);ps(" Vanc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vanilla Admin Demo ===\n\n");van_init();
ps("Vanilla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;van_planning(t,c,957+(i*17),946+(i*14),926+(i*10),908+(i*6),2020+(i%5));}
ps("\nVanilla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;van_execution(t,c,946+(i*15),935+(i*12),917+(i*8),904+(i*5),2021+(i%4));}
ps("\nVanilla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;van_evaluation(t,c,938+(i*13),927+(i*10),911+(i*7),900+(i*4),2022+(i%3));}
ps("\nVanilla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;van_accessory(t,c,930+(i*11),921+(i*9),907+(i*6),897+(i*3),2023+(i%2));}
ps("\nVanilla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;van_market(t,c,924+(i*9),915+(i*7),902+(i*5),894+(i*3),2024);}
ps("\n");van_report();van_state();ps("\n=== Demo Complete ===\n");return 0;}
