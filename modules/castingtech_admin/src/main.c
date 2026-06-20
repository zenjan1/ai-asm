/* castingtech_admin: Casting technology administration (v1.0)
 * Sand casting, precision casting, die casting, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cst_t;
typedef struct{int n_sd,n_pc,n_dc,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cst_state_t;
static cst_t sds[N],pcs[N-2],dcs[N-4],acs[N-6],mks[N-6]; static cst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cst_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cst_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cst_init(void){if(init)return -1;st.n_sd=0;st.n_pc=0;st.n_dc=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sds[i].active=0;for(int i=0;i<N-2;i++)pcs[i].active=0;for(int i=0;i<N-4;i++)dcs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CST] Castingtech initialized\n");return 0;}
int cst_sand(int t,int c,int a,int b,int d,int e,int y){return add(sds,&st.n_sd,&st.t_f1,N,t,c,a,b,d,e,y);}
int cst_precision(int t,int c,int a,int b,int d,int e,int y){return add(pcs,&st.n_pc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cst_die(int t,int c,int a,int b,int d,int e,int y){return add(dcs,&st.n_dc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cst_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cst_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cst_report(void){ps("[CST] Sd: ");pi(st.n_sd);ps(" PCS=");pi(st.t_f1);ps("\nPc: ");pi(st.n_pc);ps(" PCS=");pi(st.t_f2);ps("\nDc: ");pi(st.n_dc);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cst_state(void){ps("[CST] Sd=");pi(st.n_sd);ps(" Pc=");pi(st.n_pc);ps(" Dc=");pi(st.n_dc);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Casting Tech Admin Demo ===\n\n");cst_init();
ps("Sand casting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cst_sand(t,c,268+(i*17),253+(i*14),233+(i*10),215+(i*6),2020+(i%5));}
ps("\nPrecision casting...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cst_precision(t,c,257+(i*15),243+(i*12),225+(i*8),212+(i*5),2021+(i%4));}
ps("\nDie casting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cst_die(t,c,249+(i*13),235+(i*10),219+(i*7),208+(i*4),2022+(i%3));}
ps("\nCasting accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cst_accessory(t,c,241+(i*11),229+(i*9),215+(i*6),205+(i*3),2023+(i%2));}
ps("\nCasting marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cst_market(t,c,235+(i*9),224+(i*7),211+(i*5),203+(i*3),2024);}
ps("\n");cst_report();cst_state();ps("\n=== Demo Complete ===\n");return 0;}
