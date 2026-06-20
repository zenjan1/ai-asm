/* splinetech_admin: Spline technology administration (v1.0)
 * Rectangular spline, involute spline, triangular spline, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spl_t;
typedef struct{int n_re,n_iv,n_tr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spl_state_t;
static spl_t res[N],ivs[N-2],trs[N-4],acs[N-6],mks[N-6]; static spl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spl_init(void){if(init)return -1;st.n_re=0;st.n_iv=0;st.n_tr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)res[i].active=0;for(int i=0;i<N-2;i++)ivs[i].active=0;for(int i=0;i<N-4;i++)trs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SPL] Spline tech initialized\n");return 0;}
int spl_rectangular(int t,int c,int a,int b,int d,int e,int y){return add(res,&st.n_re,&st.t_f1,N,t,c,a,b,d,e,y);}
int spl_involute(int t,int c,int a,int b,int d,int e,int y){return add(ivs,&st.n_iv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spl_triangular(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spl_report(void){ps("[SPL] Re: ");pi(st.n_re);ps(" PCS=");pi(st.t_f1);ps("\nIv: ");pi(st.n_iv);ps(" PCS=");pi(st.t_f2);ps("\nTr: ");pi(st.n_tr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void spl_state(void){ps("[SPL] Re=");pi(st.n_re);ps(" Iv=");pi(st.n_iv);ps(" Tr=");pi(st.n_tr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Spline Tech Admin Demo ===\n\n");spl_init();
ps("Rectangular splines...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spl_rectangular(t,c,355+(i*17),340+(i*14),320+(i*10),302+(i*6),2020+(i%5));}
ps("\nInvolute splines...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spl_involute(t,c,344+(i*15),330+(i*12),312+(i*8),299+(i*5),2021+(i%4));}
ps("\nTriangular splines...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spl_triangular(t,c,336+(i*13),322+(i*10),306+(i*7),295+(i*4),2022+(i%3));}
ps("\nSpline accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spl_accessory(t,c,328+(i*11),316+(i*9),302+(i*6),292+(i*3),2023+(i%2));}
ps("\nSpline marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spl_market(t,c,322+(i*9),311+(i*7),298+(i*5),290+(i*3),2024);}
ps("\n");spl_report();spl_state();ps("\n=== Demo Complete ===\n");return 0;}
