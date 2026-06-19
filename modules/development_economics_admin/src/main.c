/* development_economics_admin: Development economics administration (v1.0)
 * Development theory, poverty/inequality, human capital, structural transformation, development policy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dev_t;
typedef struct{int n_dt,n_pi,n_hc,n_st,n_dp,t_f1,t_f2,t_f3,t_f4,t_f5;} dev_state_t;
static dev_t dts[N],pis[N-2],hcs[N-4],sts[N-6],dps[N-6]; static dev_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dev_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dev_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DEV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dev_init(void){if(init)return -1;st.n_dt=0;st.n_pi=0;st.n_hc=0;st.n_st=0;st.n_dp=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dts[i].active=0;for(int i=0;i<N-2;i++)pis[i].active=0;for(int i=0;i<N-4;i++)hcs[i].active=0;for(int i=0;i<N-6;i++)sts[i].active=0;for(int i=0;i<N-6;i++)dps[i].active=0;init=1;ps("[DEV] Development economics initialized\n");return 0;}
int dev_theory(int t,int c,int a,int b,int d,int e,int y){return add(dts,&st.n_dt,&st.t_f1,N,t,c,a,b,d,e,y);}
int dev_poverty(int t,int c,int a,int b,int d,int e,int y){return add(pis,&st.n_pi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dev_humancap(int t,int c,int a,int b,int d,int e,int y){return add(hcs,&st.n_hc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dev_structural(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dev_policy(int t,int c,int a,int b,int d,int e,int y){return add(dps,&st.n_dp,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dev_report(void){ps("[DEV] Theory: ");pi(st.n_dt);ps(" Growth=");pi(st.t_f1);ps("\nPoverty: ");pi(st.n_pi);ps(" Gini=");pi(st.t_f2);ps("\nHumanCap: ");pi(st.n_hc);ps(" Edu=");pi(st.t_f3);ps("\nStructural: ");pi(st.n_st);ps(" Urban=");pi(st.t_f4);ps("\nPolicy: ");pi(st.n_dp);ps(" Aid=");pi(st.t_f5);ps("\n");}
void dev_state(void){ps("[DEV] Dt=");pi(st.n_dt);ps(" Pi=");pi(st.n_pi);ps(" Hc=");pi(st.n_hc);ps(" St=");pi(st.n_st);ps(" Dp=");pi(st.n_dp);ps("\n");}
int main(void){
ps("=== Development Economics Admin Demo ===\n\n");dev_init();
ps("Development theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dev_theory(t,c,85+(i*17),70+(i*14),50+(i*10),33+(i*6),2020+(i%5));}
ps("\nPoverty and inequality...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dev_poverty(t,c,74+(i*15),60+(i*12),42+(i*8),29+(i*5),2021+(i%4));}
ps("\nHuman capital...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dev_humancap(t,c,66+(i*13),52+(i*10),36+(i*7),25+(i*4),2022+(i%3));}
ps("\nStructural transformation...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dev_structural(t,c,58+(i*11),46+(i*9),32+(i*6),22+(i*3),2023+(i%2));}
ps("\nDevelopment policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dev_policy(t,c,52+(i*9),41+(i*7),28+(i*5),20+(i*3),2024);}
ps("\n");dev_report();dev_state();ps("\n=== Demo Complete ===\n");return 0;}
