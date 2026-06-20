/* fruittech_admin: Fruit technology administration (v1.0)
 * Fruit planting, fruit processing, juice production, dried fruits, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} frt_t;
typedef struct{int n_fr,n_fp,n_jc,n_df,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} frt_state_t;
static frt_t frs[N],fps[N-2],jcs[N-4],dfs[N-6],mks[N-6]; static frt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(frt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;frt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FRT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int frt_init(void){if(init)return -1;st.n_fr=0;st.n_fp=0;st.n_jc=0;st.n_df=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)frs[i].active=0;for(int i=0;i<N-2;i++)fps[i].active=0;for(int i=0;i<N-4;i++)jcs[i].active=0;for(int i=0;i<N-6;i++)dfs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FRT] Fruitttech initialized\n");return 0;}
int frt_plant(int t,int c,int a,int b,int d,int e,int y){return add(frs,&st.n_fr,&st.t_f1,N,t,c,a,b,d,e,y);}
int frt_process(int t,int c,int a,int b,int d,int e,int y){return add(fps,&st.n_fp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int frt_juice(int t,int c,int a,int b,int d,int e,int y){return add(jcs,&st.n_jc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int frt_dried(int t,int c,int a,int b,int d,int e,int y){return add(dfs,&st.n_df,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int frt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void frt_report(void){ps("[FRT] Plant: ");pi(st.n_fr);ps(" ha=");pi(st.t_f1);ps("\nProc: ");pi(st.n_fp);ps(" kg=");pi(st.t_f2);ps("\nJuice: ");pi(st.n_jc);ps(" L=");pi(st.t_f3);ps("\nDried: ");pi(st.n_df);ps(" kg=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void frt_state(void){ps("[FRT] Fr=");pi(st.n_fr);ps(" Fp=");pi(st.n_fp);ps(" Jc=");pi(st.n_jc);ps(" Df=");pi(st.n_df);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fruit Tech Admin Demo ===\n\n");frt_init();
ps("Fruit planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;frt_plant(t,c,190+(i*17),175+(i*14),155+(i*10),137+(i*6),2020+(i%5));}
ps("\nFruit processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;frt_process(t,c,179+(i*15),165+(i*12),147+(i*8),134+(i*5),2021+(i%4));}
ps("\nJuice production...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;frt_juice(t,c,171+(i*13),157+(i*10),141+(i*7),130+(i*4),2022+(i%3));}
ps("\nDried fruits...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frt_dried(t,c,163+(i*11),151+(i*9),137+(i*6),127+(i*3),2023+(i%2));}
ps("\nFruit marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frt_market(t,c,157+(i*9),146+(i*7),133+(i*5),125+(i*3),2024);}
ps("\n");frt_report();frt_state();ps("\n=== Demo Complete ===\n");return 0;}
