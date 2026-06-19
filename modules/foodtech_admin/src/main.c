/* foodtech_admin: Food technology administration (v1.0)
 * Smart farming, food traceability, smart processing, cold chain logistics, nutrition health
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fdt_t;
typedef struct{int n_fm,n_tr,n_pr,n_cc,n_nh,t_f1,t_f2,t_f3,t_f4,t_f5;} fdt_state_t;
static fdt_t fms[N],trs[N-2],prs[N-4],ccs[N-6],nhs[N-6]; static fdt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fdt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fdt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FDT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fdt_init(void){if(init)return -1;st.n_fm=0;st.n_tr=0;st.n_pr=0;st.n_cc=0;st.n_nh=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fms[i].active=0;for(int i=0;i<N-2;i++)trs[i].active=0;for(int i=0;i<N-4;i++)prs[i].active=0;for(int i=0;i<N-6;i++)ccs[i].active=0;for(int i=0;i<N-6;i++)nhs[i].active=0;init=1;ps("[FDT] Foodtech initialized\n");return 0;}
int fdt_farming(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f1,N,t,c,a,b,d,e,y);}
int fdt_trace(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fdt_process(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fdt_coldchain(int t,int c,int a,int b,int d,int e,int y){return add(ccs,&st.n_cc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fdt_nutrition(int t,int c,int a,int b,int d,int e,int y){return add(nhs,&st.n_nh,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fdt_report(void){ps("[FDT] Farm: ");pi(st.n_fm);ps(" IoT=");pi(st.t_f1);ps("\nTrace: ");pi(st.n_tr);ps(" BC=");pi(st.t_f2);ps("\nProc: ");pi(st.n_pr);ps(" AI=");pi(st.t_f3);ps("\nCold: ");pi(st.n_cc);ps(" TC=");pi(st.t_f4);ps("\nNutr: ");pi(st.n_nh);ps(" Diet=");pi(st.t_f5);ps("\n");}
void fdt_state(void){ps("[FDT] Fm=");pi(st.n_fm);ps(" Tr=");pi(st.n_tr);ps(" Pr=");pi(st.n_pr);ps(" Cc=");pi(st.n_cc);ps(" Nh=");pi(st.n_nh);ps("\n");}
int main(void){
ps("=== Food Tech Admin Demo ===\n\n");fdt_init();
ps("Smart farming...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fdt_farming(t,c,148+(i*17),133+(i*14),113+(i*10),96+(i*6),2020+(i%5));}
ps("\nFood traceability...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fdt_trace(t,c,137+(i*15),123+(i*12),105+(i*8),92+(i*5),2021+(i%4));}
ps("\nSmart processing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fdt_process(t,c,129+(i*13),115+(i*10),99+(i*7),88+(i*4),2022+(i%3));}
ps("\nCold chain logistics...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fdt_coldchain(t,c,121+(i*11),109+(i*9),95+(i*6),85+(i*3),2023+(i%2));}
ps("\nNutrition health...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fdt_nutrition(t,c,115+(i*9),104+(i*7),91+(i*5),83+(i*3),2024);}
ps("\n");fdt_report();fdt_state();ps("\n=== Demo Complete ===\n");return 0;}
