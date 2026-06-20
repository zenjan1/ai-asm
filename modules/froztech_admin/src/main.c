/* froztech_admin: Frozen food technology administration (v1.0)
 * Frozen vegetables, frozen meat, frozen seafood, frozen pastry, ice cream
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} frz_t;
typedef struct{int n_fv,n_fm,n_fs,n_fp,n_ic,t_f1,t_f2,t_f3,t_f4,t_f5;} frz_state_t;
static frz_t fvs[N],fms[N-2],fss[N-4],fps[N-6],ics[N-6]; static frz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(frz_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;frz_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FRZ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int frz_init(void){if(init)return -1;st.n_fv=0;st.n_fm=0;st.n_fs=0;st.n_fp=0;st.n_ic=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fvs[i].active=0;for(int i=0;i<N-2;i++)fms[i].active=0;for(int i=0;i<N-4;i++)fss[i].active=0;for(int i=0;i<N-6;i++)fps[i].active=0;for(int i=0;i<N-6;i++)ics[i].active=0;init=1;ps("[FRZ] Froztech initialized\n");return 0;}
int frz_vegetable(int t,int c,int a,int b,int d,int e,int y){return add(fvs,&st.n_fv,&st.t_f1,N,t,c,a,b,d,e,y);}
int frz_meat(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int frz_seafood(int t,int c,int a,int b,int d,int e,int y){return add(fss,&st.n_fs,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int frz_pastry(int t,int c,int a,int b,int d,int e,int y){return add(fps,&st.n_fp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int frz_icecream(int t,int c,int a,int b,int d,int e,int y){return add(ics,&st.n_ic,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void frz_report(void){ps("[FRZ] Veg: ");pi(st.n_fv);ps(" kg=");pi(st.t_f1);ps("\nMeat: ");pi(st.n_fm);ps(" kg=");pi(st.t_f2);ps("\nSea: ");pi(st.n_fs);ps(" kg=");pi(st.t_f3);ps("\nPastry: ");pi(st.n_fp);ps(" PCS=");pi(st.t_f4);ps("\nIce: ");pi(st.n_ic);ps(" L=");pi(st.t_f5);ps("\n");}
void frz_state(void){ps("[FRZ] Fv=");pi(st.n_fv);ps(" Fm=");pi(st.n_fm);ps(" Fs=");pi(st.n_fs);ps(" Fp=");pi(st.n_fp);ps(" Ic=");pi(st.n_ic);ps("\n");}
int main(void){
ps("=== Frozen Tech Admin Demo ===\n\n");frz_init();
ps("Frozen vegetables...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;frz_vegetable(t,c,186+(i*17),171+(i*14),151+(i*10),133+(i*6),2020+(i%5));}
ps("\nFrozen meat...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;frz_meat(t,c,175+(i*15),161+(i*12),143+(i*8),130+(i*5),2021+(i%4));}
ps("\nFrozen seafood...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;frz_seafood(t,c,167+(i*13),153+(i*10),137+(i*7),126+(i*4),2022+(i%3));}
ps("\nFrozen pastry...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frz_pastry(t,c,159+(i*11),147+(i*9),133+(i*6),123+(i*3),2023+(i%2));}
ps("\nIce cream...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frz_icecream(t,c,153+(i*9),142+(i*7),129+(i*5),121+(i*3),2024);}
ps("\n");frz_report();frz_state();ps("\n=== Demo Complete ===\n");return 0;}
