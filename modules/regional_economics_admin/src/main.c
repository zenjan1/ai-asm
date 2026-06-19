/* regional_economics_admin: Regional economics administration system (v1.0)
 * Regional theory, spatial economics, regional policy, city clusters, regional competition
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} reg_t;
typedef struct{int n_rt,n_se,n_rp,n_cc,n_rc,t_f1,t_f2,t_f3,t_f4,t_f5;} reg_state_t;
static reg_t rts[N],ses[N-2],rps[N-4],ccs[N-6],rcs[N-6]; static reg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(reg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;reg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[REG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int reg_init(void){if(init)return -1;st.n_rt=0;st.n_se=0;st.n_rp=0;st.n_cc=0;st.n_rc=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rts[i].active=0;for(int i=0;i<N-2;i++)ses[i].active=0;for(int i=0;i<N-4;i++)rps[i].active=0;for(int i=0;i<N-6;i++)ccs[i].active=0;for(int i=0;i<N-6;i++)rcs[i].active=0;init=1;ps("[REG] Regional economics initialized\n");return 0;}
int reg_theory(int t,int c,int a,int b,int d,int e,int y){return add(rts,&st.n_rt,&st.t_f1,N,t,c,a,b,d,e,y);}
int reg_spatial(int t,int c,int a,int b,int d,int e,int y){return add(ses,&st.n_se,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int reg_policy(int t,int c,int a,int b,int d,int e,int y){return add(rps,&st.n_rp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int reg_cluster(int t,int c,int a,int b,int d,int e,int y){return add(ccs,&st.n_cc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int reg_compete(int t,int c,int a,int b,int d,int e,int y){return add(rcs,&st.n_rc,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void reg_report(void){ps("[REG] Theory: ");pi(st.n_rt);ps(" Location=");pi(st.t_f1);ps("\nSpatial: ");pi(st.n_se);ps(" Transport=");pi(st.t_f2);ps("\nPolicy: ");pi(st.n_rp);ps(" Transfer=");pi(st.t_f3);ps("\nCluster: ");pi(st.n_cc);ps(" Metro=");pi(st.t_f4);ps("\nCompete: ");pi(st.n_rc);ps(" Biz=");pi(st.t_f5);ps("\n");}
void reg_state(void){ps("[REG] Rt=");pi(st.n_rt);ps(" Se=");pi(st.n_se);ps(" Rp=");pi(st.n_rp);ps(" Cc=");pi(st.n_cc);ps(" Rc=");pi(st.n_rc);ps("\n");}
int main(void){
ps("=== Regional Economics Admin Demo ===\n\n");reg_init();
ps("Regional theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;reg_theory(t,c,84+(i*17),70+(i*14),50+(i*10),33+(i*6),2020+(i%5));}
ps("\nSpatial economics...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;reg_spatial(t,c,74+(i*15),60+(i*12),42+(i*8),29+(i*5),2021+(i%4));}
ps("\nRegional policy...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;reg_policy(t,c,66+(i*13),52+(i*10),36+(i*7),25+(i*4),2022+(i%3));}
ps("\nCity clusters...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;reg_cluster(t,c,58+(i*11),46+(i*9),32+(i*6),22+(i*3),2023+(i%2));}
ps("\nRegional competition...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;reg_compete(t,c,52+(i*9),41+(i*7),28+(i*5),20+(i*3),2024);}
ps("\n");reg_report();reg_state();ps("\n=== Demo Complete ===\n");return 0;}
