/* textech_admin: Textile technology administration (v1.0)
 * Spinning, weaving, dyeing and printing, textile products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} txt_t;
typedef struct{int n_sp,n_wv,n_dp,n_tp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} txt_state_t;
static txt_t sps[N],wvs[N-2],dps[N-4],tps[N-6],mks[N-6]; static txt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(txt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;txt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TXT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int txt_init(void){if(init)return -1;st.n_sp=0;st.n_wv=0;st.n_dp=0;st.n_tp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sps[i].active=0;for(int i=0;i<N-2;i++)wvs[i].active=0;for(int i=0;i<N-4;i++)dps[i].active=0;for(int i=0;i<N-6;i++)tps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TXT] Textech initialized\n");return 0;}
int txt_spin(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f1,N,t,c,a,b,d,e,y);}
int txt_weave(int t,int c,int a,int b,int d,int e,int y){return add(wvs,&st.n_wv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int txt_dye(int t,int c,int a,int b,int d,int e,int y){return add(dps,&st.n_dp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int txt_product(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int txt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void txt_report(void){ps("[TXT] Sp: ");pi(st.n_sp);ps(" kg=");pi(st.t_f1);ps("\nWv: ");pi(st.n_wv);ps(" m=");pi(st.t_f2);ps("\nDp: ");pi(st.n_dp);ps(" m=");pi(st.t_f3);ps("\nTp: ");pi(st.n_tp);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void txt_state(void){ps("[TXT] Sp=");pi(st.n_sp);ps(" Wv=");pi(st.n_wv);ps(" Dp=");pi(st.n_dp);ps(" Tp=");pi(st.n_tp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Textile Tech Admin Demo ===\n\n");txt_init();
ps("Spinning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;txt_spin(t,c,227+(i*17),212+(i*14),192+(i*10),174+(i*6),2020+(i%5));}
ps("\nWeaving...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;txt_weave(t,c,216+(i*15),202+(i*12),184+(i*8),171+(i*5),2021+(i%4));}
ps("\nDyeing and printing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;txt_dye(t,c,208+(i*13),194+(i*10),178+(i*7),167+(i*4),2022+(i%3));}
ps("\nTextile products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;txt_product(t,c,200+(i*11),188+(i*9),174+(i*6),164+(i*3),2023+(i%2));}
ps("\nTextile marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;txt_market(t,c,194+(i*9),183+(i*7),170+(i*5),162+(i*3),2024);}
ps("\n");txt_report();txt_state();ps("\n=== Demo Complete ===\n");return 0;}
