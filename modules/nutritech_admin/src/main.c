/* nutritech_admin: Nutrition technology administration (v1.0)
 * Nutritional supplements, health food, functional food, organic food, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nrt_t;
typedef struct{int n_ns,n_hf,n_ff,n_of,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nrt_state_t;
static nrt_t nss[N],hfs[N-2],ffs[N-4],ofs[N-6],mks[N-6]; static nrt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nrt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nrt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NRT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nrt_init(void){if(init)return -1;st.n_ns=0;st.n_hf=0;st.n_ff=0;st.n_of=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nss[i].active=0;for(int i=0;i<N-2;i++)hfs[i].active=0;for(int i=0;i<N-4;i++)ffs[i].active=0;for(int i=0;i<N-6;i++)ofs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[NRT] Nutritech initialized\n");return 0;}
int nrt_supplement(int t,int c,int a,int b,int d,int e,int y){return add(nss,&st.n_ns,&st.t_f1,N,t,c,a,b,d,e,y);}
int nrt_health(int t,int c,int a,int b,int d,int e,int y){return add(hfs,&st.n_hf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nrt_functional(int t,int c,int a,int b,int d,int e,int y){return add(ffs,&st.n_ff,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nrt_organic(int t,int c,int a,int b,int d,int e,int y){return add(ofs,&st.n_of,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nrt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nrt_report(void){ps("[NRT] Supp: ");pi(st.n_ns);ps(" SKU=");pi(st.t_f1);ps("\nHealth: ");pi(st.n_hf);ps(" SKU=");pi(st.t_f2);ps("\nFunc: ");pi(st.n_ff);ps(" SKU=");pi(st.t_f3);ps("\nOrganic: ");pi(st.n_of);ps(" SKU=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nrt_state(void){ps("[NRT] Ns=");pi(st.n_ns);ps(" Hf=");pi(st.n_hf);ps(" Ff=");pi(st.n_ff);ps(" Of=");pi(st.n_of);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nutrition Tech Admin Demo ===\n\n");nrt_init();
ps("Nutritional supplements...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nrt_supplement(t,c,192+(i*17),177+(i*14),157+(i*10),139+(i*6),2020+(i%5));}
ps("\nHealth food...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nrt_health(t,c,181+(i*15),167+(i*12),149+(i*8),136+(i*5),2021+(i%4));}
ps("\nFunctional food...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nrt_functional(t,c,173+(i*13),159+(i*10),143+(i*7),132+(i*4),2022+(i%3));}
ps("\nOrganic food...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nrt_organic(t,c,165+(i*11),153+(i*9),139+(i*6),129+(i*3),2023+(i%2));}
ps("\nNutrition marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nrt_market(t,c,159+(i*9),148+(i*7),135+(i*5),127+(i*3),2024);}
ps("\n");nrt_report();nrt_state();ps("\n=== Demo Complete ===\n");return 0;}
