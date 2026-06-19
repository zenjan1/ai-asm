/* information_economics_admin: Information economics administration (v1.0)
 * Information value, adverse selection, moral hazard, signaling games, mechanism design
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} inf_t;
typedef struct{int n_iv,n_as,n_mh,n_sg,n_md,t_f1,t_f2,t_f3,t_f4,t_f5;} inf_state_t;
static inf_t ivs[N],ass[N-2],mhs[N-4],sgs[N-6],mds[N-6]; static inf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(inf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;inf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[INF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int inf_init(void){if(init)return -1;st.n_iv=0;st.n_as=0;st.n_mh=0;st.n_sg=0;st.n_md=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ivs[i].active=0;for(int i=0;i<N-2;i++)ass[i].active=0;for(int i=0;i<N-4;i++)mhs[i].active=0;for(int i=0;i<N-6;i++)sgs[i].active=0;for(int i=0;i<N-6;i++)mds[i].active=0;init=1;ps("[INF] Information economics initialized\n");return 0;}
int inf_value(int t,int c,int a,int b,int d,int e,int y){return add(ivs,&st.n_iv,&st.t_f1,N,t,c,a,b,d,e,y);}
int inf_adverse(int t,int c,int a,int b,int d,int e,int y){return add(ass,&st.n_as,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int inf_hazard(int t,int c,int a,int b,int d,int e,int y){return add(mhs,&st.n_mh,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int inf_signaling(int t,int c,int a,int b,int d,int e,int y){return add(sgs,&st.n_sg,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int inf_mechanism(int t,int c,int a,int b,int d,int e,int y){return add(mds,&st.n_md,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void inf_report(void){ps("[INF] Value: ");pi(st.n_iv);ps(" Info=");pi(st.t_f1);ps("\nAdverse: ");pi(st.n_as);ps(" Lemon=");pi(st.t_f2);ps("\nHazard: ");pi(st.n_mh);ps(" Insurance=");pi(st.t_f3);ps("\nSignaling: ");pi(st.n_sg);ps(" Edu=");pi(st.t_f4);ps("\nMechanism: ");pi(st.n_md);ps(" VCG=");pi(st.t_f5);ps("\n");}
void inf_state(void){ps("[INF] Iv=");pi(st.n_iv);ps(" As=");pi(st.n_as);ps(" Mh=");pi(st.n_mh);ps(" Sg=");pi(st.n_sg);ps(" Md=");pi(st.n_md);ps("\n");}
int main(void){
ps("=== Information Economics Admin Demo ===\n\n");inf_init();
ps("Information value...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;inf_value(t,c,84+(i*18),69+(i*15),49+(i*11),32+(i*7),2020+(i%5));}
ps("\nAdverse selection...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;inf_adverse(t,c,73+(i*16),59+(i*13),41+(i*9),28+(i*6),2021+(i%4));}
ps("\nMoral hazard...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;inf_hazard(t,c,65+(i*14),51+(i*11),35+(i*7),24+(i*4),2022+(i%3));}
ps("\nSignaling games...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;inf_signaling(t,c,57+(i*12),45+(i*9),31+(i*6),21+(i*3),2023+(i%2));}
ps("\nMechanism design...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;inf_mechanism(t,c,51+(i*10),40+(i*8),27+(i*5),19+(i*3),2024);}
ps("\n");inf_report();inf_state();ps("\n=== Demo Complete ===\n");return 0;}
