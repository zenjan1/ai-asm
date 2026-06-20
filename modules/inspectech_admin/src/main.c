/* inspectech_admin: Inspection technology administration (v1.0)
 * NDT, online inspection, chemical inspection, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} isp_t;
typedef struct{int n_nd,n_on,n_ch,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} isp_state_t;
static isp_t nds[N],ons[N-2],chs[N-4],acs[N-6],mks[N-6]; static isp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(isp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;isp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ISP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int isp_init(void){if(init)return -1;st.n_nd=0;st.n_on=0;st.n_ch=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nds[i].active=0;for(int i=0;i<N-2;i++)ons[i].active=0;for(int i=0;i<N-4;i++)chs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ISP] Inspectech initialized\n");return 0;}
int isp_ndt(int t,int c,int a,int b,int d,int e,int y){return add(nds,&st.n_nd,&st.t_f1,N,t,c,a,b,d,e,y);}
int isp_online(int t,int c,int a,int b,int d,int e,int y){return add(ons,&st.n_on,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int isp_chemical(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int isp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int isp_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void isp_report(void){ps("[ISP] Nd: ");pi(st.n_nd);ps(" PCS=");pi(st.t_f1);ps("\nOn: ");pi(st.n_on);ps(" PCS=");pi(st.t_f2);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void isp_state(void){ps("[ISP] Nd=");pi(st.n_nd);ps(" On=");pi(st.n_on);ps(" Ch=");pi(st.n_ch);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Inspection Tech Admin Demo ===\n\n");isp_init();
ps("NDT inspection...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;isp_ndt(t,c,287+(i*17),272+(i*14),252+(i*10),234+(i*6),2020+(i%5));}
ps("\nOnline inspection...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;isp_online(t,c,276+(i*15),262+(i*12),244+(i*8),231+(i*5),2021+(i%4));}
ps("\nChemical inspection...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;isp_chemical(t,c,268+(i*13),254+(i*10),238+(i*7),227+(i*4),2022+(i%3));}
ps("\nInspection accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;isp_accessory(t,c,260+(i*11),248+(i*9),234+(i*6),224+(i*3),2023+(i%2));}
ps("\nInspection marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;isp_market(t,c,254+(i*9),243+(i*7),230+(i*5),222+(i*3),2024);}
ps("\n");isp_report();isp_state();ps("\n=== Demo Complete ===\n");return 0;}
