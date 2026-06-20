/* plasma_admin: Plasma processing technology administration (v1.0)
 * Plasma cutting, plasma spraying, plasma welding, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} plm_t;
typedef struct{int n_ct,n_sp,n_wd,n_ac,n_mt,t_f1,t_f2,t_f3,t_f4,t_f5;} plm_state_t;
static plm_t cts[N],sps[N-2],wds[N-4],acs[N-6],mts[N-6]; static plm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(plm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;plm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PLM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int plm_init(void){if(init)return -1;st.n_ct=0;st.n_sp=0;st.n_wd=0;st.n_ac=0;st.n_mt=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cts[i].active=0;for(int i=0;i<N-2;i++)sps[i].active=0;for(int i=0;i<N-4;i++)wds[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mts[i].active=0;init=1;ps("[PLM] Plasma initialized\n");return 0;}
int plm_cutting(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f1,N,t,c,a,b,d,e,y);}
int plm_spraying(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int plm_welding(int t,int c,int a,int b,int d,int e,int y){return add(wds,&st.n_wd,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int plm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int plm_market(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void plm_report(void){ps("[PLM] Ct: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f1);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f2);ps("\nWd: ");pi(st.n_wd);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMt: ");pi(st.n_mt);ps(" USD=");pi(st.t_f5);ps("\n");}
void plm_state(void){ps("[PLM] Ct=");pi(st.n_ct);ps(" Sp=");pi(st.n_sp);ps(" Wd=");pi(st.n_wd);ps(" Ac=");pi(st.n_ac);ps(" Mt=");pi(st.n_mt);ps("\n");}
int main(void){
ps("=== Plasma Admin Demo ===\n\n");plm_init();
ps("Plasma cutting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;plm_cutting(t,c,410+(i*17),399+(i*14),379+(i*10),361+(i*6),2020+(i%5));}
ps("\nPlasma spraying...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;plm_spraying(t,c,399+(i*15),388+(i*12),370+(i*8),357+(i*5),2021+(i%4));}
ps("\nPlasma welding...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;plm_welding(t,c,391+(i*13),380+(i*10),364+(i*7),353+(i*4),2022+(i%3));}
ps("\nPlasma accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plm_accessory(t,c,383+(i*11),374+(i*9),360+(i*6),350+(i*3),2023+(i%2));}
ps("\nPlasma marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plm_market(t,c,377+(i*9),368+(i*7),355+(i*5),347+(i*3),2024);}
ps("\n");plm_report();plm_state();ps("\n=== Demo Complete ===\n");return 0;}
