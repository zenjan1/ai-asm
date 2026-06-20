/* tofutech_admin: Tofu technology administration (v1.0)
 * Soy milk production, tofu making, soy products, fermented soy, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tft_t;
typedef struct{int n_sm,n_tf,n_sp,n_fm,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tft_state_t;
static tft_t sms[N],tfs[N-2],sps[N-4],fms[N-6],mks[N-6]; static tft_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tft_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tft_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TFT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tft_init(void){if(init)return -1;st.n_sm=0;st.n_tf=0;st.n_sp=0;st.n_fm=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sms[i].active=0;for(int i=0;i<N-2;i++)tfs[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)fms[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TFT] Tofutech initialized\n");return 0;}
int tft_milk(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f1,N,t,c,a,b,d,e,y);}
int tft_tofu(int t,int c,int a,int b,int d,int e,int y){return add(tfs,&st.n_tf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tft_product(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tft_ferment(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tft_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tft_report(void){ps("[TFT] Milk: ");pi(st.n_sm);ps(" L=");pi(st.t_f1);ps("\nTofu: ");pi(st.n_tf);ps(" kg=");pi(st.t_f2);ps("\nProd: ");pi(st.n_sp);ps(" kg=");pi(st.t_f3);ps("\nFerm: ");pi(st.n_fm);ps(" kg=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tft_state(void){ps("[TFT] Sm=");pi(st.n_sm);ps(" Tf=");pi(st.n_tf);ps(" Sp=");pi(st.n_sp);ps(" Fm=");pi(st.n_fm);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tofu Tech Admin Demo ===\n\n");tft_init();
ps("Soy milk...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tft_milk(t,c,188+(i*17),173+(i*14),153+(i*10),135+(i*6),2020+(i%5));}
ps("\nTofu making...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tft_tofu(t,c,177+(i*15),163+(i*12),145+(i*8),132+(i*5),2021+(i%4));}
ps("\nSoy products...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tft_product(t,c,169+(i*13),155+(i*10),139+(i*7),128+(i*4),2022+(i%3));}
ps("\nFermented soy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tft_ferment(t,c,161+(i*11),149+(i*9),135+(i*6),125+(i*3),2023+(i%2));}
ps("\nTofu marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tft_market(t,c,155+(i*9),144+(i*7),131+(i*5),123+(i*3),2024);}
ps("\n");tft_report();tft_state();ps("\n=== Demo Complete ===\n");return 0;}
