/* trade_finance_admin: Trade finance administration (v1.0)
 * Letter of credit, guarantee business, trade financing, supply chain finance, cross-border settlement
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tfa_t;
typedef struct{int n_lc,n_gb,n_tf,n_sf,n_cs,t_f1,t_f2,t_f3,t_f4,t_f5;} tfa_state_t;
static tfa_t lcs[N],gbs[N-2],tfs[N-4],sfs[N-6],css[N-6]; static tfa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tfa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tfa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TFA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tfa_init(void){if(init)return -1;st.n_lc=0;st.n_gb=0;st.n_tf=0;st.n_sf=0;st.n_cs=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lcs[i].active=0;for(int i=0;i<N-2;i++)gbs[i].active=0;for(int i=0;i<N-4;i++)tfs[i].active=0;for(int i=0;i<N-6;i++)sfs[i].active=0;for(int i=0;i<N-6;i++)css[i].active=0;init=1;ps("[TFA] Trade finance initialized\n");return 0;}
int tfa_lc(int t,int c,int a,int b,int d,int e,int y){return add(lcs,&st.n_lc,&st.t_f1,N,t,c,a,b,d,e,y);}
int tfa_guarantee(int t,int c,int a,int b,int d,int e,int y){return add(gbs,&st.n_gb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tfa_trade(int t,int c,int a,int b,int d,int e,int y){return add(tfs,&st.n_tf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tfa_supply(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tfa_settle(int t,int c,int a,int b,int d,int e,int y){return add(css,&st.n_cs,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tfa_report(void){ps("[TFA] LC: ");pi(st.n_lc);ps(" UCP=");pi(st.t_f1);ps("\nGuar: ");pi(st.n_gb);ps(" Bond=");pi(st.t_f2);ps("\nTrade: ");pi(st.n_tf);ps(" Forf=");pi(st.t_f3);ps("\nSCF: ");pi(st.n_sf);ps(" Core=");pi(st.t_f4);ps("\nSettle: ");pi(st.n_cs);ps(" SWIFT=");pi(st.t_f5);ps("\n");}
void tfa_state(void){ps("[TFA] Lc=");pi(st.n_lc);ps(" Gb=");pi(st.n_gb);ps(" Tf=");pi(st.n_tf);ps(" Sf=");pi(st.n_sf);ps(" Cs=");pi(st.n_cs);ps("\n");}
int main(void){
ps("=== Trade Finance Admin Demo ===\n\n");tfa_init();
ps("Letter of credit...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tfa_lc(t,c,129+(i*17),114+(i*14),94+(i*10),77+(i*6),2020+(i%5));}
ps("\nGuarantee business...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tfa_guarantee(t,c,118+(i*15),104+(i*12),86+(i*8),73+(i*5),2021+(i%4));}
ps("\nTrade financing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tfa_trade(t,c,110+(i*13),96+(i*10),80+(i*7),69+(i*4),2022+(i%3));}
ps("\nSupply chain finance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tfa_supply(t,c,102+(i*11),90+(i*9),76+(i*6),66+(i*3),2023+(i%2));}
ps("\nCross-border settlement...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tfa_settle(t,c,96+(i*9),85+(i*7),72+(i*5),64+(i*3),2024);}
ps("\n");tfa_report();tfa_state();ps("\n=== Demo Complete ===\n");return 0;}
