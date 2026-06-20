/* energytech_admin: Energy technology administration (v1.0)
 * Smart grid, energy storage, renewable generation, energy trading, energy efficiency
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eng_t;
typedef struct{int n_gr,n_st,n_re,n_tr,n_ef,t_f1,t_f2,t_f3,t_f4,t_f5;} eng_state_t;
static eng_t grs[N],sts[N-2],res[N-4],trs[N-6],efs[N-6]; static eng_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eng_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eng_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ENG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eng_init(void){if(init)return -1;st.n_gr=0;st.n_st=0;st.n_re=0;st.n_tr=0;st.n_ef=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)grs[i].active=0;for(int i=0;i<N-2;i++)sts[i].active=0;for(int i=0;i<N-4;i++)res[i].active=0;for(int i=0;i<N-6;i++)trs[i].active=0;for(int i=0;i<N-6;i++)efs[i].active=0;init=1;ps("[ENG] Energytech initialized\n");return 0;}
int eng_grid(int t,int c,int a,int b,int d,int e,int y){return add(grs,&st.n_gr,&st.t_f1,N,t,c,a,b,d,e,y);}
int eng_storage(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eng_renewable(int t,int c,int a,int b,int d,int e,int y){return add(res,&st.n_re,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eng_trading(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eng_efficiency(int t,int c,int a,int b,int d,int e,int y){return add(efs,&st.n_ef,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eng_report(void){ps("[ENG] Grid: ");pi(st.n_gr);ps(" MW=");pi(st.t_f1);ps("\nStore: ");pi(st.n_st);ps(" MWh=");pi(st.t_f2);ps("\nRenew: ");pi(st.n_re);ps(" GWh=");pi(st.t_f3);ps("\nTrade: ");pi(st.n_tr);ps(" USD=");pi(st.t_f4);ps("\nEff: ");pi(st.n_ef);ps(" Pct=");pi(st.t_f5);ps("\n");}
void eng_state(void){ps("[ENG] Gr=");pi(st.n_gr);ps(" St=");pi(st.n_st);ps(" Re=");pi(st.n_re);ps(" Tr=");pi(st.n_tr);ps(" Ef=");pi(st.n_ef);ps("\n");}
int main(void){
ps("=== Energy Tech Admin Demo ===\n\n");eng_init();
ps("Smart grid...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eng_grid(t,c,154+(i*17),139+(i*14),119+(i*10),102+(i*6),2020+(i%5));}
ps("\nEnergy storage...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eng_storage(t,c,143+(i*15),129+(i*12),111+(i*8),98+(i*5),2021+(i%4));}
ps("\nRenewable generation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eng_renewable(t,c,135+(i*13),121+(i*10),105+(i*7),94+(i*4),2022+(i%3));}
ps("\nEnergy trading...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eng_trading(t,c,127+(i*11),115+(i*9),101+(i*6),91+(i*3),2023+(i%2));}
ps("\nEnergy efficiency...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eng_efficiency(t,c,121+(i*9),110+(i*7),97+(i*5),89+(i*3),2024);}
ps("\n");eng_report();eng_state();ps("\n=== Demo Complete ===\n");return 0;}
