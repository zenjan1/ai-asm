/* bolttech_admin: Bolt technology administration (v1.0)
 * Hex bolt, socket head bolt, anchor bolt, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} blt_t;
typedef struct{int n_hx,n_sk,n_an,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} blt_state_t;
static blt_t hxs[N],sks[N-2],ans[N-4],acs[N-6],mks[N-6]; static blt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;blt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BLT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int blt_init(void){if(init)return -1;st.n_hx=0;st.n_sk=0;st.n_an=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hxs[i].active=0;for(int i=0;i<N-2;i++)sks[i].active=0;for(int i=0;i<N-4;i++)ans[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BLT] Bolttech initialized\n");return 0;}
int blt_hex(int t,int c,int a,int b,int d,int e,int y){return add(hxs,&st.n_hx,&st.t_f1,N,t,c,a,b,d,e,y);}
int blt_socket(int t,int c,int a,int b,int d,int e,int y){return add(sks,&st.n_sk,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int blt_anchor(int t,int c,int a,int b,int d,int e,int y){return add(ans,&st.n_an,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int blt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int blt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void blt_report(void){ps("[BLT] Hx: ");pi(st.n_hx);ps(" PCS=");pi(st.t_f1);ps("\nSk: ");pi(st.n_sk);ps(" PCS=");pi(st.t_f2);ps("\nAn: ");pi(st.n_an);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void blt_state(void){ps("[BLT] Hx=");pi(st.n_hx);ps(" Sk=");pi(st.n_sk);ps(" An=");pi(st.n_an);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bolt Tech Admin Demo ===\n\n");blt_init();
ps("Hex bolts...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;blt_hex(t,c,361+(i*17),346+(i*14),326+(i*10),308+(i*6),2020+(i%5));}
ps("\nSocket head bolts...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;blt_socket(t,c,350+(i*15),336+(i*12),318+(i*8),305+(i*5),2021+(i%4));}
ps("\nAnchor bolts...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;blt_anchor(t,c,342+(i*13),328+(i*10),312+(i*7),301+(i*4),2022+(i%3));}
ps("\nBolt accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blt_accessory(t,c,334+(i*11),322+(i*9),308+(i*6),298+(i*3),2023+(i%2));}
ps("\nBolt marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blt_market(t,c,328+(i*9),317+(i*7),304+(i*5),296+(i*3),2024);}
ps("\n");blt_report();blt_state();ps("\n=== Demo Complete ===\n");return 0;}
