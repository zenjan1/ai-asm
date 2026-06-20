/* asset_admin: Asset management technology administration (v1.0)
 * Asset procurement, asset operations, asset disposal, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} asx_t;
typedef struct{int n_ap,n_ao,n_ad,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} asx_state_t;
static asx_t aps[N],aos[N-2],ads[N-4],acs[N-6],mks[N-6]; static asx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;asx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ASX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int asx_init(void){if(init)return -1;st.n_ap=0;st.n_ao=0;st.n_ad=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aps[i].active=0;for(int i=0;i<N-2;i++)aos[i].active=0;for(int i=0;i<N-4;i++)ads[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ASX] Asset initialized\n");return 0;}
int asx_procurement(int t,int c,int a,int b,int d,int e,int y){return add(aps,&st.n_ap,&st.t_f1,N,t,c,a,b,d,e,y);}
int asx_operations(int t,int c,int a,int b,int d,int e,int y){return add(aos,&st.n_ao,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int asx_disposal(int t,int c,int a,int b,int d,int e,int y){return add(ads,&st.n_ad,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int asx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int asx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void asx_report(void){ps("[ASX] Ap: ");pi(st.n_ap);ps(" PCS=");pi(st.t_f1);ps("\nAo: ");pi(st.n_ao);ps(" PCS=");pi(st.t_f2);ps("\nAd: ");pi(st.n_ad);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void asx_state(void){ps("[ASX] Ap=");pi(st.n_ap);ps(" Ao=");pi(st.n_ao);ps(" Ad=");pi(st.n_ad);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Asset Admin Demo ===\n\n");asx_init();
ps("Asset procurement...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;asx_procurement(t,c,449+(i*17),438+(i*14),418+(i*10),400+(i*6),2020+(i%5));}
ps("\nAsset operations...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;asx_operations(t,c,438+(i*15),427+(i*12),409+(i*8),396+(i*5),2021+(i%4));}
ps("\nAsset disposal...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;asx_disposal(t,c,430+(i*13),419+(i*10),403+(i*7),392+(i*4),2022+(i%3));}
ps("\nAsset accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asx_accessory(t,c,422+(i*11),413+(i*9),399+(i*6),389+(i*3),2023+(i%2));}
ps("\nAsset marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asx_market(t,c,416+(i*9),407+(i*7),394+(i*5),386+(i*3),2024);}
ps("\n");asx_report();asx_state();ps("\n=== Demo Complete ===\n");return 0;}
