/* gasket_admin: Gasket/seal technology administration (v1.0)
 * Non-metallic gaskets, metallic gaskets, semi-metallic gaskets, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gkx_t;
typedef struct{int n_nm,n_mt,n_sm,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gkx_state_t;
static gkx_t nms[N],mts[N-2],sms[N-4],acs[N-6],mks[N-6]; static gkx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gkx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gkx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GKX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gkx_init(void){if(init)return -1;st.n_nm=0;st.n_mt=0;st.n_sm=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nms[i].active=0;for(int i=0;i<N-2;i++)mts[i].active=0;for(int i=0;i<N-4;i++)sms[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GKX] Gasket initialized\n");return 0;}
int gkx_nonmetallic(int t,int c,int a,int b,int d,int e,int y){return add(nms,&st.n_nm,&st.t_f1,N,t,c,a,b,d,e,y);}
int gkx_metallic(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gkx_semimetallic(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gkx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gkx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gkx_report(void){ps("[GKX] Nm: ");pi(st.n_nm);ps(" PCS=");pi(st.t_f1);ps("\nMt: ");pi(st.n_mt);ps(" PCS=");pi(st.t_f2);ps("\nSm: ");pi(st.n_sm);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gkx_state(void){ps("[GKX] Nm=");pi(st.n_nm);ps(" Mt=");pi(st.n_mt);ps(" Sm=");pi(st.n_sm);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gasket Admin Demo ===\n\n");gkx_init();
ps("Non-metallic gaskets...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gkx_nonmetallic(t,c,419+(i*17),408+(i*14),388+(i*10),370+(i*6),2020+(i%5));}
ps("\nMetallic gaskets...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gkx_metallic(t,c,408+(i*15),397+(i*12),379+(i*8),366+(i*5),2021+(i%4));}
ps("\nSemi-metallic gaskets...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gkx_semimetallic(t,c,400+(i*13),389+(i*10),373+(i*7),362+(i*4),2022+(i%3));}
ps("\nSealing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gkx_accessory(t,c,392+(i*11),383+(i*9),369+(i*6),359+(i*3),2023+(i%2));}
ps("\nSealing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gkx_market(t,c,386+(i*9),377+(i*7),364+(i*5),356+(i*3),2024);}
ps("\n");gkx_report();gkx_state();ps("\n=== Demo Complete ===\n");return 0;}
