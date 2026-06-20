/* gear_admin: Gear technology administration (v1.0)
 * Cylindrical gear, bevel gear, special gear, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ger_t;
typedef struct{int n_cy,n_bv,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ger_state_t;
static ger_t cys[N],bvs[N-2],sps[N-4],acs[N-6],mks[N-6]; static ger_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ger_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ger_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ger_init(void){if(init)return -1;st.n_cy=0;st.n_bv=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cys[i].active=0;for(int i=0;i<N-2;i++)bvs[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GER] Gear initialized\n");return 0;}
int ger_cylindrical(int t,int c,int a,int b,int d,int e,int y){return add(cys,&st.n_cy,&st.t_f1,N,t,c,a,b,d,e,y);}
int ger_bevel(int t,int c,int a,int b,int d,int e,int y){return add(bvs,&st.n_bv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ger_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ger_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ger_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ger_report(void){ps("[GER] Cy: ");pi(st.n_cy);ps(" PCS=");pi(st.t_f1);ps("\nBv: ");pi(st.n_bv);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ger_state(void){ps("[GER] Cy=");pi(st.n_cy);ps(" Bv=");pi(st.n_bv);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gear Admin Demo ===\n\n");ger_init();
ps("Cylindrical gears...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ger_cylindrical(t,c,378+(i*17),363+(i*14),343+(i*10),325+(i*6),2020+(i%5));}
ps("\nBevel gears...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ger_bevel(t,c,367+(i*15),353+(i*12),335+(i*8),322+(i*5),2021+(i%4));}
ps("\nSpecial gears...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ger_special(t,c,359+(i*13),345+(i*10),329+(i*7),318+(i*4),2022+(i%3));}
ps("\nGear accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ger_accessory(t,c,351+(i*11),339+(i*9),325+(i*6),315+(i*3),2023+(i%2));}
ps("\nGear marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ger_market(t,c,345+(i*9),334+(i*7),321+(i*5),313+(i*3),2024);}
ps("\n");ger_report();ger_state();ps("\n=== Demo Complete ===\n");return 0;}
