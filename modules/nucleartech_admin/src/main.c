/* nucleartech_admin: Nuclear technology administration (v1.0)
 * Nuclear fuel, nuclear power, nuclear application, nuclear safety, nuclear waste
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nct_t;
typedef struct{int n_fu,n_pw,n_ap,n_sf,n_ws,t_f1,t_f2,t_f3,t_f4,t_f5;} nct_state_t;
static nct_t fus[N],pws[N-2],aps[N-4],sfs[N-6],wss[N-6]; static nct_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nct_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nct_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NCT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nct_init(void){if(init)return -1;st.n_fu=0;st.n_pw=0;st.n_ap=0;st.n_sf=0;st.n_ws=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fus[i].active=0;for(int i=0;i<N-2;i++)pws[i].active=0;for(int i=0;i<N-4;i++)aps[i].active=0;for(int i=0;i<N-6;i++)sfs[i].active=0;for(int i=0;i<N-6;i++)wss[i].active=0;init=1;ps("[NCT] Nucleartech initialized\n");return 0;}
int nct_fuel(int t,int c,int a,int b,int d,int e,int y){return add(fus,&st.n_fu,&st.t_f1,N,t,c,a,b,d,e,y);}
int nct_power(int t,int c,int a,int b,int d,int e,int y){return add(pws,&st.n_pw,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nct_apply(int t,int c,int a,int b,int d,int e,int y){return add(aps,&st.n_ap,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nct_safety(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nct_waste(int t,int c,int a,int b,int d,int e,int y){return add(wss,&st.n_ws,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nct_report(void){ps("[NCT] Fuel: ");pi(st.n_fu);ps(" U=");pi(st.t_f1);ps("\nPwr: ");pi(st.n_pw);ps(" MWe=");pi(st.t_f2);ps("\nApp: ");pi(st.n_ap);ps(" CI=");pi(st.t_f3);ps("\nSafe: ");pi(st.n_sf);ps(" mSv=");pi(st.t_f4);ps("\nWaste: ");pi(st.n_ws);ps(" HLW=");pi(st.t_f5);ps("\n");}
void nct_state(void){ps("[NCT] Fu=");pi(st.n_fu);ps(" Pw=");pi(st.n_pw);ps(" Ap=");pi(st.n_ap);ps(" Sf=");pi(st.n_sf);ps(" Ws=");pi(st.n_ws);ps("\n");}
int main(void){
ps("=== Nuclear Tech Admin Demo ===\n\n");nct_init();
ps("Nuclear fuel...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nct_fuel(t,c,166+(i*17),151+(i*14),131+(i*10),114+(i*6),2020+(i%5));}
ps("\nNuclear power...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nct_power(t,c,155+(i*15),141+(i*12),123+(i*8),110+(i*5),2021+(i%4));}
ps("\nNuclear application...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nct_apply(t,c,147+(i*13),133+(i*10),117+(i*7),106+(i*4),2022+(i%3));}
ps("\nNuclear safety...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nct_safety(t,c,139+(i*11),127+(i*9),113+(i*6),103+(i*3),2023+(i%2));}
ps("\nNuclear waste...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nct_waste(t,c,133+(i*9),122+(i*7),109+(i*5),101+(i*3),2024);}
ps("\n");nct_report();nct_state();ps("\n=== Demo Complete ===\n");return 0;}
