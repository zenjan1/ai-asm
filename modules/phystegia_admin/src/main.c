/* phystegia_admin: Phystegia management technology administration (v1.0)
 * Phystegia planning, phystegia execution, phystegia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} phy_t;
typedef struct{int n_phyp,n_phye,n_phy2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} phy_state_t;
static phy_t phyps[N],physs[N-2],phyvss[N-4],phyas[N-6],phymks[N-6]; static phy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(phy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;phy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PHY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int phy_init(void){if(init)return -1;st.n_phyp=0;st.n_phye=0;st.n_phy2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)phyps[i].active=0;for(int i=0;i<N-2;i++)physs[i].active=0;for(int i=0;i<N-4;i++)phyvss[i].active=0;for(int i=0;i<N-6;i++)phyas[i].active=0;for(int i=0;i<N-6;i++)phymks[i].active=0;init=1;ps("[PHY] Phystegia initialized\n");return 0;}
int phy_planning(int t,int c,int a,int b,int d,int e,int y){return add(phyps,&st.n_phyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int phy_execution(int t,int c,int a,int b,int d,int e,int y){return add(physs,&st.n_phye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int phy_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(phyvss,&st.n_phy2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int phy_accessory(int t,int c,int a,int b,int d,int e,int y){return add(phyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int phy_market(int t,int c,int a,int b,int d,int e,int y){return add(phymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void phy_report(void){ps("[PHY] Phypp: ");pi(st.n_phyp);ps(" PCS=");pi(st.t_f1);ps("\nPhye: ");pi(st.n_phye);ps(" PCS=");pi(st.t_f2);ps("\nPhyv: ");pi(st.n_phy2);ps(" PCS=");pi(st.t_f3);ps("\nPhyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void phy_state(void){ps("[PHY] Phypp=");pi(st.n_phyp);ps(" Phye=");pi(st.n_phye);ps(" Phyv=");pi(st.n_phy2);ps(" Phyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Phystegia Admin Demo ===\n\n");phy_init();
ps("Phystegia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;phy_planning(t,c,1168+(i*17),1157+(i*14),1137+(i*10),1119+(i*6),2020+(i%5));}
ps("\nPhystegia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;phy_execution(t,c,1157+(i*15),1146+(i*12),1128+(i*8),1115+(i*5),2021+(i%4));}
ps("\nPhystegia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;phy_evaluation(t,c,1149+(i*13),1138+(i*10),1122+(i*7),1111+(i*4),2022+(i%3));}
ps("\nPhystegia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phy_accessory(t,c,1141+(i*11),1132+(i*9),1118+(i*6),1108+(i*3),2023+(i%2));}
ps("\nPhystegia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phy_market(t,c,1135+(i*9),1126+(i*7),1113+(i*5),1105+(i*3),2024);}
ps("\n");phy_report();phy_state();ps("\n=== Demo Complete ===\n");return 0;}
