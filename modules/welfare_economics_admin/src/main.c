/* welfare_economics_admin: Welfare economics administration (v1.0)
 * Social welfare, Pareto efficiency, social choice, inequality measurement, redistribution policy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wlf_t;
typedef struct{int n_sw,n_pe,n_sc,n_im,n_rp,t_f1,t_f2,t_f3,t_f4,t_f5;} wlf_state_t;
static wlf_t sws[N],pes[N-2],scs[N-4],ims[N-6],rps[N-6]; static wlf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wlf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wlf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WLF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wlf_init(void){if(init)return -1;st.n_sw=0;st.n_pe=0;st.n_sc=0;st.n_im=0;st.n_rp=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sws[i].active=0;for(int i=0;i<N-2;i++)pes[i].active=0;for(int i=0;i<N-4;i++)scs[i].active=0;for(int i=0;i<N-6;i++)ims[i].active=0;for(int i=0;i<N-6;i++)rps[i].active=0;init=1;ps("[WLF] Welfare economics initialized\n");return 0;}
int wlf_welfare(int t,int c,int a,int b,int d,int e,int y){return add(sws,&st.n_sw,&st.t_f1,N,t,c,a,b,d,e,y);}
int wlf_pareto(int t,int c,int a,int b,int d,int e,int y){return add(pes,&st.n_pe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wlf_choice(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wlf_inequal(int t,int c,int a,int b,int d,int e,int y){return add(ims,&st.n_im,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wlf_redist(int t,int c,int a,int b,int d,int e,int y){return add(rps,&st.n_rp,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wlf_report(void){ps("[WLF] Welfare: ");pi(st.n_sw);ps(" Util=");pi(st.t_f1);ps("\nPareto: ");pi(st.n_pe);ps(" Efficien=");pi(st.t_f2);ps("\nChoice: ");pi(st.n_sc);ps(" Arrow=");pi(st.t_f3);ps("\nInequal: ");pi(st.n_im);ps(" Gini=");pi(st.t_f4);ps("\nRedist: ");pi(st.n_rp);ps(" Tax=");pi(st.t_f5);ps("\n");}
void wlf_state(void){ps("[WLF] Sw=");pi(st.n_sw);ps(" Pe=");pi(st.n_pe);ps(" Sc=");pi(st.n_sc);ps(" Im=");pi(st.n_im);ps(" Rp=");pi(st.n_rp);ps("\n");}
int main(void){
ps("=== Welfare Economics Admin Demo ===\n\n");wlf_init();
ps("Social welfare...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wlf_welfare(t,c,107+(i*17),92+(i*14),72+(i*10),55+(i*6),2020+(i%5));}
ps("\nPareto efficiency...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wlf_pareto(t,c,96+(i*15),82+(i*12),64+(i*8),51+(i*5),2021+(i%4));}
ps("\nSocial choice...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wlf_choice(t,c,88+(i*13),74+(i*10),58+(i*7),47+(i*4),2022+(i%3));}
ps("\nInequality measurement...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wlf_inequal(t,c,80+(i*11),68+(i*9),54+(i*6),44+(i*3),2023+(i%2));}
ps("\nRedistribution policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wlf_redist(t,c,74+(i*9),63+(i*7),50+(i*5),42+(i*3),2024);}
ps("\n");wlf_report();wlf_state();ps("\n=== Demo Complete ===\n");return 0;}
