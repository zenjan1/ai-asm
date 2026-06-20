/* logistech_admin: Logistics technology administration (v1.0)
 * Warehousing, transportation, distribution, supply chain, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lgt_t;
typedef struct{int n_wh,n_tr,n_ds,n_sc,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lgt_state_t;
static lgt_t whs[N],trs[N-2],dss[N-4],scs[N-6],mks[N-6]; static lgt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lgt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lgt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LGT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lgt_init(void){if(init)return -1;st.n_wh=0;st.n_tr=0;st.n_ds=0;st.n_sc=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)whs[i].active=0;for(int i=0;i<N-2;i++)trs[i].active=0;for(int i=0;i<N-4;i++)dss[i].active=0;for(int i=0;i<N-6;i++)scs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[LGT] Logistech initialized\n");return 0;}
int lgt_warehouse(int t,int c,int a,int b,int d,int e,int y){return add(whs,&st.n_wh,&st.t_f1,N,t,c,a,b,d,e,y);}
int lgt_transport(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lgt_distribute(int t,int c,int a,int b,int d,int e,int y){return add(dss,&st.n_ds,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lgt_supply(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lgt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lgt_report(void){ps("[LGT] Wh: ");pi(st.n_wh);ps(" PCS=");pi(st.t_f1);ps("\nTr: ");pi(st.n_tr);ps(" Ton=");pi(st.t_f2);ps("\nDs: ");pi(st.n_ds);ps(" PCS=");pi(st.t_f3);ps("\nSc: ");pi(st.n_sc);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lgt_state(void){ps("[LGT] Wh=");pi(st.n_wh);ps(" Tr=");pi(st.n_tr);ps(" Ds=");pi(st.n_ds);ps(" Sc=");pi(st.n_sc);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Logistics Tech Admin Demo ===\n\n");lgt_init();
ps("Warehousing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lgt_warehouse(t,c,240+(i*17),225+(i*14),205+(i*10),187+(i*6),2020+(i%5));}
ps("\nTransportation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lgt_transport(t,c,229+(i*15),215+(i*12),197+(i*8),184+(i*5),2021+(i%4));}
ps("\nDistribution...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lgt_distribute(t,c,221+(i*13),207+(i*10),191+(i*7),180+(i*4),2022+(i%3));}
ps("\nSupply chain...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lgt_supply(t,c,213+(i*11),201+(i*9),187+(i*6),177+(i*3),2023+(i%2));}
ps("\nLogistics marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lgt_market(t,c,207+(i*9),196+(i*7),183+(i*5),175+(i*3),2024);}
ps("\n");lgt_report();lgt_state();ps("\n=== Demo Complete ===\n");return 0;}
