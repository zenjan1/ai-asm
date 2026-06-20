/* mixertech_admin: Mixer technology administration (v1.0)
 * Stirrers, mixers, dispersers, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mxr_t;
typedef struct{int n_st,n_mx,n_dp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mxr_state_t;
static mxr_t sts[N],mxs[N-2],dps[N-4],acs[N-6],mks[N-6]; static mxr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mxr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mxr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MXR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mxr_init(void){if(init)return -1;st.n_st=0;st.n_mx=0;st.n_dp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sts[i].active=0;for(int i=0;i<N-2;i++)mxs[i].active=0;for(int i=0;i<N-4;i++)dps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MXR] Mixertech initialized\n");return 0;}
int mxr_stirrer(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f1,N,t,c,a,b,d,e,y);}
int mxr_mixer(int t,int c,int a,int b,int d,int e,int y){return add(mxs,&st.n_mx,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mxr_disperser(int t,int c,int a,int b,int d,int e,int y){return add(dps,&st.n_dp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mxr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mxr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mxr_report(void){ps("[MXR] St: ");pi(st.n_st);ps(" PCS=");pi(st.t_f1);ps("\nMx: ");pi(st.n_mx);ps(" PCS=");pi(st.t_f2);ps("\nDp: ");pi(st.n_dp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mxr_state(void){ps("[MXR] St=");pi(st.n_st);ps(" Mx=");pi(st.n_mx);ps(" Dp=");pi(st.n_dp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mixer Tech Admin Demo ===\n\n");mxr_init();
ps("Stirrers...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mxr_stirrer(t,c,263+(i*17),248+(i*14),228+(i*10),210+(i*6),2020+(i%5));}
ps("\nMixers...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mxr_mixer(t,c,252+(i*15),238+(i*12),220+(i*8),207+(i*5),2021+(i%4));}
ps("\nDispersers...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mxr_disperser(t,c,244+(i*13),230+(i*10),214+(i*7),203+(i*4),2022+(i%3));}
ps("\nMixer accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mxr_accessory(t,c,236+(i*11),224+(i*9),210+(i*6),200+(i*3),2023+(i%2));}
ps("\nMixer marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mxr_market(t,c,230+(i*9),219+(i*7),206+(i*5),198+(i*3),2024);}
ps("\n");mxr_report();mxr_state();ps("\n=== Demo Complete ===\n");return 0;}
