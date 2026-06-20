/* convey_admin: Conveying technology administration (v1.0)
 * Belt conveyors, chain conveyors, roller conveyors, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cvy_t;
typedef struct{int n_bt,n_ch,n_rl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cvy_state_t;
static cvy_t bts[N],chs[N-2],rls[N-4],acs[N-6],mks[N-6]; static cvy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cvy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cvy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CVY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cvy_init(void){if(init)return -1;st.n_bt=0;st.n_ch=0;st.n_rl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bts[i].active=0;for(int i=0;i<N-2;i++)chs[i].active=0;for(int i=0;i<N-4;i++)rls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CVY] Convey initialized\n");return 0;}
int cvy_belt(int t,int c,int a,int b,int d,int e,int y){return add(bts,&st.n_bt,&st.t_f1,N,t,c,a,b,d,e,y);}
int cvy_chain(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cvy_roller(int t,int c,int a,int b,int d,int e,int y){return add(rls,&st.n_rl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cvy_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cvy_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cvy_report(void){ps("[CVY] Bt: ");pi(st.n_bt);ps(" PCS=");pi(st.t_f1);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f2);ps("\nRl: ");pi(st.n_rl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cvy_state(void){ps("[CVY] Bt=");pi(st.n_bt);ps(" Ch=");pi(st.n_ch);ps(" Rl=");pi(st.n_rl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Convey Admin Demo ===\n\n");cvy_init();
ps("Belt conveyors...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cvy_belt(t,c,433+(i*17),422+(i*14),402+(i*10),384+(i*6),2020+(i%5));}
ps("\nChain conveyors...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cvy_chain(t,c,422+(i*15),411+(i*12),393+(i*8),380+(i*5),2021+(i%4));}
ps("\nRoller conveyors...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cvy_roller(t,c,414+(i*13),403+(i*10),387+(i*7),376+(i*4),2022+(i%3));}
ps("\nConveying accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cvy_accessory(t,c,406+(i*11),397+(i*9),383+(i*6),373+(i*3),2023+(i%2));}
ps("\nConveying marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cvy_market(t,c,400+(i*9),391+(i*7),378+(i*5),370+(i*3),2024);}
ps("\n");cvy_report();cvy_state();ps("\n=== Demo Complete ===\n");return 0;}
