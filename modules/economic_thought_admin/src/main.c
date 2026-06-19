/* economic_thought_admin: Economic thought history administration (v1.0)
 * Classical school, marginal revolution, Keynesian revolution, modern schools, Marxism
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eco_t;
typedef struct{int n_cl,n_mg,n_ky,n_md,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eco_state_t;
static eco_t cls[N],mgs[N-2],kys[N-4],mds[N-6],mks[N-6]; static eco_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eco_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eco_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ECO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eco_init(void){if(init)return -1;st.n_cl=0;st.n_mg=0;st.n_ky=0;st.n_md=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cls[i].active=0;for(int i=0;i<N-2;i++)mgs[i].active=0;for(int i=0;i<N-4;i++)kys[i].active=0;for(int i=0;i<N-6;i++)mds[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ECO] Economic thought initialized\n");return 0;}
int eco_classical(int t,int c,int a,int b,int d,int e,int y){return add(cls,&st.n_cl,&st.t_f1,N,t,c,a,b,d,e,y);}
int eco_marginal(int t,int c,int a,int b,int d,int e,int y){return add(mgs,&st.n_mg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eco_keynesian(int t,int c,int a,int b,int d,int e,int y){return add(kys,&st.n_ky,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eco_modern(int t,int c,int a,int b,int d,int e,int y){return add(mds,&st.n_md,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eco_marxist(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eco_report(void){ps("[ECO] Classical: ");pi(st.n_cl);ps(" Smith=");pi(st.t_f1);ps("\nMarginal: ");pi(st.n_mg);ps(" Walras=");pi(st.t_f2);ps("\nKeynesian: ");pi(st.n_ky);ps(" ISLM=");pi(st.t_f3);ps("\nModern: ");pi(st.n_md);ps(" Monetar=");pi(st.t_f4);ps("\nMarxist: ");pi(st.n_mk);ps(" Capital=");pi(st.t_f5);ps("\n");}
void eco_state(void){ps("[ECO] Cl=");pi(st.n_cl);ps(" Mg=");pi(st.n_mg);ps(" Ky=");pi(st.n_ky);ps(" Md=");pi(st.n_md);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Economic Thought Admin Demo ===\n\n");eco_init();
ps("Classical school...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eco_classical(t,c,88+(i*17),73+(i*14),53+(i*10),36+(i*6),1776+(i*12));}
ps("\nMarginal revolution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eco_marginal(t,c,77+(i*15),63+(i*12),45+(i*8),32+(i*5),1870+(i*5));}
ps("\nKeynesian revolution...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eco_keynesian(t,c,69+(i*13),55+(i*10),39+(i*7),28+(i*4),1936+(i*8));}
ps("\nModern schools...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eco_modern(t,c,61+(i*11),49+(i*9),35+(i*6),25+(i*3),1970+(i*6));}
ps("\nMarxism...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eco_marxist(t,c,55+(i*9),44+(i*7),31+(i*5),23+(i*3),1867);}
ps("\n");eco_report();eco_state();ps("\n=== Demo Complete ===\n");return 0;}
