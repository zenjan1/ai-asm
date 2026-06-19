/* spatial_economics_admin: Spatial economics administration (v1.0)
 * Spatial theory, urban space, regional economy, spatial policy, globalization space
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spc_t;
typedef struct{int n_st,n_us,n_re,n_sp,n_gs,t_f1,t_f2,t_f3,t_f4,t_f5;} spc_state_t;
static spc_t sts[N],uss[N-2],res[N-4],sps[N-6],gss[N-6]; static spc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spc_init(void){if(init)return -1;st.n_st=0;st.n_us=0;st.n_re=0;st.n_sp=0;st.n_gs=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sts[i].active=0;for(int i=0;i<N-2;i++)uss[i].active=0;for(int i=0;i<N-4;i++)res[i].active=0;for(int i=0;i<N-6;i++)sps[i].active=0;for(int i=0;i<N-6;i++)gss[i].active=0;init=1;ps("[SPC] Spatial economics initialized\n");return 0;}
int spc_theory(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f1,N,t,c,a,b,d,e,y);}
int spc_urban(int t,int c,int a,int b,int d,int e,int y){return add(uss,&st.n_us,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spc_regional(int t,int c,int a,int b,int d,int e,int y){return add(res,&st.n_re,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spc_policy(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spc_global(int t,int c,int a,int b,int d,int e,int y){return add(gss,&st.n_gs,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spc_report(void){ps("[SPC] Theory: ");pi(st.n_st);ps(" Location=");pi(st.t_f1);ps("\nUrban: ");pi(st.n_us);ps(" CBD=");pi(st.t_f2);ps("\nRegional: ");pi(st.n_re);ps(" Divide=");pi(st.t_f3);ps("\nPolicy: ");pi(st.n_sp);ps(" Plan=");pi(st.t_f4);ps("\nGlobal: ");pi(st.n_gs);ps(" City=");pi(st.t_f5);ps("\n");}
void spc_state(void){ps("[SPC] St=");pi(st.n_st);ps(" Us=");pi(st.n_us);ps(" Re=");pi(st.n_re);ps(" Sp=");pi(st.n_sp);ps(" Gs=");pi(st.n_gs);ps("\n");}
int main(void){
ps("=== Spatial Economics Admin Demo ===\n\n");spc_init();
ps("Spatial theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spc_theory(t,c,95+(i*17),80+(i*14),60+(i*10),43+(i*6),2020+(i%5));}
ps("\nUrban space...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spc_urban(t,c,84+(i*15),70+(i*12),52+(i*8),39+(i*5),2021+(i%4));}
ps("\nRegional economy...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spc_regional(t,c,76+(i*13),62+(i*10),46+(i*7),35+(i*4),2022+(i%3));}
ps("\nSpatial policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spc_policy(t,c,68+(i*11),56+(i*9),42+(i*6),32+(i*3),2023+(i%2));}
ps("\nGlobalization space...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spc_global(t,c,62+(i*9),51+(i*7),38+(i*5),30+(i*3),2024);}
ps("\n");spc_report();spc_state();ps("\n=== Demo Complete ===\n");return 0;}
