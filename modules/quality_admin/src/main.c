/* quality_admin: Quality management technology administration (v1.0)
 * Quality control, quality assurance, quality improvement, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} qtx_t;
typedef struct{int n_ct,n_as,n_im,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} qtx_state_t;
static qtx_t cts[N],ass[N-2],ims[N-4],acs[N-6],mks[N-6]; static qtx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(qtx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;qtx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[QTX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int qtx_init(void){if(init)return -1;st.n_ct=0;st.n_as=0;st.n_im=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cts[i].active=0;for(int i=0;i<N-2;i++)ass[i].active=0;for(int i=0;i<N-4;i++)ims[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[QTX] Quality initialized\n");return 0;}
int qtx_control(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f1,N,t,c,a,b,d,e,y);}
int qtx_assurance(int t,int c,int a,int b,int d,int e,int y){return add(ass,&st.n_as,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int qtx_improvement(int t,int c,int a,int b,int d,int e,int y){return add(ims,&st.n_im,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int qtx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int qtx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void qtx_report(void){ps("[QTX] Ct: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f1);ps("\nAs: ");pi(st.n_as);ps(" PCS=");pi(st.t_f2);ps("\nIm: ");pi(st.n_im);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void qtx_state(void){ps("[QTX] Ct=");pi(st.n_ct);ps(" As=");pi(st.n_as);ps(" Im=");pi(st.n_im);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Quality Admin Demo ===\n\n");qtx_init();
ps("Quality control...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;qtx_control(t,c,439+(i*17),428+(i*14),408+(i*10),390+(i*6),2020+(i%5));}
ps("\nQuality assurance...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;qtx_assurance(t,c,428+(i*15),417+(i*12),399+(i*8),386+(i*5),2021+(i%4));}
ps("\nQuality improvement...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;qtx_improvement(t,c,420+(i*13),409+(i*10),393+(i*7),382+(i*4),2022+(i%3));}
ps("\nQuality accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qtx_accessory(t,c,412+(i*11),403+(i*9),389+(i*6),379+(i*3),2023+(i%2));}
ps("\nQuality marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qtx_market(t,c,406+(i*9),397+(i*7),384+(i*5),376+(i*3),2024);}
ps("\n");qtx_report();qtx_state();ps("\n=== Demo Complete ===\n");return 0;}
