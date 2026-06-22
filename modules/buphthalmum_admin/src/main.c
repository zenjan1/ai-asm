/* buphthalmum_admin: Buphthalmum management technology administration (v1.0)
 * Buphthalmum planning, buphthalmum execution, buphthalmum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bup_t;
typedef struct{int n_bupp,n_bupe,n_bup2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bup_state_t;
static bup_t bupps[N],bupss[N-2],bupvss[N-4],bupas[N-6],bupmks[N-6]; static bup_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bup_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bup_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BUP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bup_init(void){if(init)return -1;st.n_bupp=0;st.n_bupe=0;st.n_bup2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bupps[i].active=0;for(int i=0;i<N-2;i++)bupss[i].active=0;for(int i=0;i<N-4;i++)bupvss[i].active=0;for(int i=0;i<N-6;i++)bupas[i].active=0;for(int i=0;i<N-6;i++)bupmks[i].active=0;init=1;ps("[BUP] Buphthalmum initialized\n");return 0;}
int bup_planning(int t,int c,int a,int b,int d,int e,int y){return add(bupps,&st.n_bupp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bup_execution(int t,int c,int a,int b,int d,int e,int y){return add(bupss,&st.n_bupe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bup_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bupvss,&st.n_bup2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bup_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bupas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bup_market(int t,int c,int a,int b,int d,int e,int y){return add(bupmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bup_report(void){ps("[BUP] Buppp: ");pi(st.n_bupp);ps(" PCS=");pi(st.t_f1);ps("\nBuppe: ");pi(st.n_bupe);ps(" PCS=");pi(st.t_f2);ps("\nBupv: ");pi(st.n_bup2);ps(" PCS=");pi(st.t_f3);ps("\nBupc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bup_state(void){ps("[BUP] Buppp=");pi(st.n_bupp);ps(" Buppe=");pi(st.n_bupe);ps(" Bupv=");pi(st.n_bup2);ps(" Bupc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Buphthalmum Admin Demo ===\n\n");bup_init();
ps("Buphthalmum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bup_planning(t,c,1224+(i*17),1213+(i*14),1193+(i*10),1175+(i*6),2020+(i%5));}
ps("\nBuphthalmum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bup_execution(t,c,1213+(i*15),1202+(i*12),1184+(i*8),1171+(i*5),2021+(i%4));}
ps("\nBuphthalmum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bup_evaluation(t,c,1205+(i*13),1194+(i*10),1178+(i*7),1167+(i*4),2022+(i%3));}
ps("\nBuphthalmum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bup_accessory(t,c,1197+(i*11),1188+(i*9),1174+(i*6),1164+(i*3),2023+(i%2));}
ps("\nBuphthalmum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bup_market(t,c,1191+(i*9),1182+(i*7),1169+(i*5),1161+(i*3),2024);}
ps("\n");bup_report();bup_state();ps("\n=== Demo Complete ===\n");return 0;}
