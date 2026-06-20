/* conveytech_admin: Conveyor technology administration (v1.0)
 * Belt conveyors, roller conveyors, chain conveyors, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cnv_t;
typedef struct{int n_bt,n_rl,n_ch,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cnv_state_t;
static cnv_t bts[N],rls[N-2],chs[N-4],acs[N-6],mks[N-6]; static cnv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cnv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cnv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CNV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cnv_init(void){if(init)return -1;st.n_bt=0;st.n_rl=0;st.n_ch=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bts[i].active=0;for(int i=0;i<N-2;i++)rls[i].active=0;for(int i=0;i<N-4;i++)chs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CNV] Conveytech initialized\n");return 0;}
int cnv_belt(int t,int c,int a,int b,int d,int e,int y){return add(bts,&st.n_bt,&st.t_f1,N,t,c,a,b,d,e,y);}
int cnv_roller(int t,int c,int a,int b,int d,int e,int y){return add(rls,&st.n_rl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cnv_chain(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cnv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cnv_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cnv_report(void){ps("[CNV] Bt: ");pi(st.n_bt);ps(" PCS=");pi(st.t_f1);ps("\nRl: ");pi(st.n_rl);ps(" PCS=");pi(st.t_f2);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cnv_state(void){ps("[CNV] Bt=");pi(st.n_bt);ps(" Rl=");pi(st.n_rl);ps(" Ch=");pi(st.n_ch);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Conveyor Tech Admin Demo ===\n\n");cnv_init();
ps("Belt conveyors...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cnv_belt(t,c,257+(i*17),242+(i*14),222+(i*10),204+(i*6),2020+(i%5));}
ps("\nRoller conveyors...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cnv_roller(t,c,246+(i*15),232+(i*12),214+(i*8),201+(i*5),2021+(i%4));}
ps("\nChain conveyors...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cnv_chain(t,c,238+(i*13),224+(i*10),208+(i*7),197+(i*4),2022+(i%3));}
ps("\nConveyor accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cnv_accessory(t,c,230+(i*11),218+(i*9),204+(i*6),194+(i*3),2023+(i%2));}
ps("\nConveyor marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cnv_market(t,c,224+(i*9),213+(i*7),200+(i*5),192+(i*3),2024);}
ps("\n");cnv_report();cnv_state();ps("\n=== Demo Complete ===\n");return 0;}
