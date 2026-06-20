/* honeysuckle_admin: Honeysuckle management technology administration (v1.0)
 * Honeysuckle planning, honeysuckle execution, honeysuckle evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hon_t;
typedef struct{int n_hop,n_hoe,n_hov,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hon_state_t;
static hon_t hops[N],hoes[N-2],hovs[N-4],acs[N-6],mks[N-6]; static hon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hon_init(void){if(init)return -1;st.n_hop=0;st.n_hoe=0;st.n_hov=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hops[i].active=0;for(int i=0;i<N-2;i++)hoes[i].active=0;for(int i=0;i<N-4;i++)hovs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HON] Honeysuckle initialized\n");return 0;}
int hon_planning(int t,int c,int a,int b,int d,int e,int y){return add(hops,&st.n_hop,&st.t_f1,N,t,c,a,b,d,e,y);}
int hon_execution(int t,int c,int a,int b,int d,int e,int y){return add(hoes,&st.n_hoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hon_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hovs,&st.n_hov,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hon_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hon_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hon_report(void){ps("[HON] Hop: ");pi(st.n_hop);ps(" PCS=");pi(st.t_f1);ps("\nHoe: ");pi(st.n_hoe);ps(" PCS=");pi(st.t_f2);ps("\nHov: ");pi(st.n_hov);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hon_state(void){ps("[HON] Hop=");pi(st.n_hop);ps(" Hoe=");pi(st.n_hoe);ps(" Hov=");pi(st.n_hov);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Honeysuckle Admin Demo ===\n\n");hon_init();
ps("Honeysuckle planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hon_planning(t,c,587+(i*17),576+(i*14),556+(i*10),538+(i*6),2020+(i%5));}
ps("\nHoneysuckle execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hon_execution(t,c,576+(i*15),565+(i*12),547+(i*8),534+(i*5),2021+(i%4));}
ps("\nHoneysuckle evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hon_evaluation(t,c,568+(i*13),557+(i*10),541+(i*7),530+(i*4),2022+(i%3));}
ps("\nHoneysuckle accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hon_accessory(t,c,560+(i*11),551+(i*9),537+(i*6),527+(i*3),2023+(i%2));}
ps("\nHoneysuckle marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hon_market(t,c,554+(i*9),545+(i*7),532+(i*5),524+(i*3),2024);}
ps("\n");hon_report();hon_state();ps("\n=== Demo Complete ===\n");return 0;}
