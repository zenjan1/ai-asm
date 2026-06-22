/* podocarpus_admin: Podocarpus management technology administration (v1.0)
 * Podocarpus planning, podocarpus execution, podocarpus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pod_t;
typedef struct{int n_podp,n_pode,n_pod2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pod_state_t;
static pod_t podps[N],podss[N-2],podvss[N-4],podas[N-6],podmks[N-6]; static pod_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pod_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pod_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[POD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pod_init(void){if(init)return -1;st.n_podp=0;st.n_pode=0;st.n_pod2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)podps[i].active=0;for(int i=0;i<N-2;i++)podss[i].active=0;for(int i=0;i<N-4;i++)podvss[i].active=0;for(int i=0;i<N-6;i++)podas[i].active=0;for(int i=0;i<N-6;i++)podmks[i].active=0;init=1;ps("[POD] Podocarpus initialized\n");return 0;}
int pod_planning(int t,int c,int a,int b,int d,int e,int y){return add(podps,&st.n_podp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pod_execution(int t,int c,int a,int b,int d,int e,int y){return add(podss,&st.n_pode,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pod_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(podvss,&st.n_pod2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pod_accessory(int t,int c,int a,int b,int d,int e,int y){return add(podas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pod_market(int t,int c,int a,int b,int d,int e,int y){return add(podmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pod_report(void){ps("[POD] Podpp: ");pi(st.n_podp);ps(" PCS=");pi(st.t_f1);ps("\nPode: ");pi(st.n_pode);ps(" PCS=");pi(st.t_f2);ps("\nPodv: ");pi(st.n_pod2);ps(" PCS=");pi(st.t_f3);ps("\nPodc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pod_state(void){ps("[POD] Podpp=");pi(st.n_podp);ps(" Pode=");pi(st.n_pode);ps(" Podv=");pi(st.n_pod2);ps(" Podc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Podocarpus Admin Demo ===\n\n");pod_init();
ps("Podocarpus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pod_planning(t,c,1170+(i*17),1159+(i*14),1139+(i*10),1121+(i*6),2020+(i%5));}
ps("\nPodocarpus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pod_execution(t,c,1159+(i*15),1148+(i*12),1130+(i*8),1117+(i*5),2021+(i%4));}
ps("\nPodocarpus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pod_evaluation(t,c,1151+(i*13),1140+(i*10),1124+(i*7),1113+(i*4),2022+(i%3));}
ps("\nPodocarpus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pod_accessory(t,c,1143+(i*11),1134+(i*9),1120+(i*6),1110+(i*3),2023+(i%2));}
ps("\nPodocarpus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pod_market(t,c,1137+(i*9),1128+(i*7),1115+(i*5),1107+(i*3),2024);}
ps("\n");pod_report();pod_state();ps("\n=== Demo Complete ===\n");return 0;}
