/* network_economics_admin: Network economics administration (v1.0)
 * Network effects, network structure, pricing strategy, competition policy, digital networks
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} net_t;
typedef struct{int n_ne,n_ns,n_ps,n_cp,n_dn,t_f1,t_f2,t_f3,t_f4,t_f5;} net_state_t;
static net_t nes[N],nss[N-2],pss[N-4],cps[N-6],dns[N-6]; static net_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(net_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;net_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NET] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int net_init(void){if(init)return -1;st.n_ne=0;st.n_ns=0;st.n_ps=0;st.n_cp=0;st.n_dn=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nes[i].active=0;for(int i=0;i<N-2;i++)nss[i].active=0;for(int i=0;i<N-4;i++)pss[i].active=0;for(int i=0;i<N-6;i++)cps[i].active=0;for(int i=0;i<N-6;i++)dns[i].active=0;init=1;ps("[NET] Network economics initialized\n");return 0;}
int net_effect(int t,int c,int a,int b,int d,int e,int y){return add(nes,&st.n_ne,&st.t_f1,N,t,c,a,b,d,e,y);}
int net_structure(int t,int c,int a,int b,int d,int e,int y){return add(nss,&st.n_ns,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int net_pricing(int t,int c,int a,int b,int d,int e,int y){return add(pss,&st.n_ps,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int net_compete(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int net_digital(int t,int c,int a,int b,int d,int e,int y){return add(dns,&st.n_dn,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void net_report(void){ps("[NET] Effect: ");pi(st.n_ne);ps(" Direct=");pi(st.t_f1);ps("\nStruct: ");pi(st.n_ns);ps(" SmallW=");pi(st.t_f2);ps("\nPricing: ");pi(st.n_ps);ps(" Access=");pi(st.t_f3);ps("\nCompete: ");pi(st.n_cp);ps(" Neutral=");pi(st.t_f4);ps("\nDigital: ");pi(st.n_dn);ps(" Cloud=");pi(st.t_f5);ps("\n");}
void net_state(void){ps("[NET] Ne=");pi(st.n_ne);ps(" Ns=");pi(st.n_ns);ps(" Ps=");pi(st.n_ps);ps(" Cp=");pi(st.n_cp);ps(" Dn=");pi(st.n_dn);ps("\n");}
int main(void){
ps("=== Network Economics Admin Demo ===\n\n");net_init();
ps("Network effects...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;net_effect(t,c,96+(i*17),81+(i*14),61+(i*10),44+(i*6),2020+(i%5));}
ps("\nNetwork structure...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;net_structure(t,c,85+(i*15),71+(i*12),53+(i*8),40+(i*5),2021+(i%4));}
ps("\nPricing strategy...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;net_pricing(t,c,77+(i*13),63+(i*10),47+(i*7),36+(i*4),2022+(i%3));}
ps("\nCompetition policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;net_compete(t,c,69+(i*11),57+(i*9),43+(i*6),33+(i*3),2023+(i%2));}
ps("\nDigital networks...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;net_digital(t,c,63+(i*9),52+(i*7),39+(i*5),31+(i*3),2024);}
ps("\n");net_report();net_state();ps("\n=== Demo Complete ===\n");return 0;}
